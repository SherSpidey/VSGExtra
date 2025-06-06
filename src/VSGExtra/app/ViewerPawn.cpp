//
// Created by ParadoxFang on 2025/5/16.
//

#include <vsg/ui/PointerEvent.h>
#include <vsg/ui/ScrollWheelEvent.h>

#include <VSGExtra/math/spatial.h>
#include <VSGExtra/app/ViewerCamera.h>

#include <VSGExtra/app/ViewerPawn.h>

using namespace vsg;
using namespace VSGExtra;

ViewerPawn::ViewerPawn(const ref_ptr<XCamera>& camera) :
    Inherit(camera),
    pawn_state_(INACTIVE),
    has_pointer_focus_(false),
    last_pointer_within_render_area_(false)
{
    if (camera->type_info() != typeid(ViewerCamera))
    {
        std::cerr << "Creating a ViewerPawn without a ViewerCamera!\n";
    }
}

ViewerPawn::ViewerPawnState ViewerPawn::get_pawn_state() const
{
    return pawn_state_;
}

void ViewerPawn::apply(KeyPressEvent& key_press_event)
{
    Inherit::apply(key_press_event);

    if (key_press_event.keyBase == KEY_T || key_press_event.keyBase == KEY_t)
    {
        if (auto view_camera = camera_->cast<ViewerCamera>())
        {
            view_camera->ToggleCamera();
        }
    }
}

void ViewerPawn::apply(ButtonPressEvent& button_press_event)
{
    Inherit::apply(button_press_event);

    last_pointer_within_render_area_ = has_pointer_focus_ = WithinRenderArea(button_press_event);

    // press event marks the beginning of a pawn action
    // it does not directly trigger rotate or pan
    // however, it must set the state to notify external systems
    // that an intersection calculation is required for subsequent move events
    if (keyboard_registry_->PressedExclusive(KEY_Shift_L) &&
        button_press_event.mask & BUTTON_MASK_2)
        pawn_state_ = ROTATE;
    else if (keyboard_registry_->Idle() &&
        button_press_event.mask & BUTTON_MASK_2)
        pawn_state_ = PAN;
    else
        pawn_state_ = INACTIVE;

    previous_pointer_event_ = &button_press_event;
}

void ViewerPawn::apply(ButtonReleaseEvent& button_release_event)
{
    if (button_release_event.handled || !EventRelevant(button_release_event)) return;

    last_pointer_within_render_area_ = WithinRenderArea(button_release_event);
    has_pointer_focus_ = false;

    // release event marks the end of a pawn action, so reset intersection
    ClearIntersection();

    previous_pointer_event_ = &button_release_event;
}

void ViewerPawn::apply(MoveEvent& move_event)
{
    if (!EventRelevant(move_event)) return;

    last_pointer_within_render_area_ = WithinRenderArea(move_event);

    if (move_event.handled || !has_pointer_focus_)
    {
        previous_pointer_event_ = &move_event;
        return;
    }

    // calculate trackball info
    auto ndc = NDC(move_event);
    auto tbc = TBC(move_event);

    // get previous info
    if (!previous_pointer_event_) previous_pointer_event_ = &move_event;
    auto pre_ndc = NDC(*previous_pointer_event_);
    auto pre_tbc = TBC(*previous_pointer_event_);

    // state machine
    if (keyboard_registry_->PressedExclusive(KEY_Shift_L) &&
        move_event.mask & BUTTON_MASK_2)
    {
        pawn_state_ = ROTATE;

        move_event.handled = true;

        auto axis = cross(normalize(tbc), normalize(pre_tbc));
        auto axis_len = length(axis);
        
        // rotation axis exits
        if (axis_len > 0.0)
        {
            auto rotate_angle = std::asin(axis_len);
            axis /= axis_len;

            Rotate(rotate_angle, axis);
        }
    }
    else if (keyboard_registry_->Idle() &&
        move_event.mask & BUTTON_MASK_2)
    {
        pawn_state_ = PAN;

        move_event.handled = true;

        auto pan_delta = ndc - pre_ndc;

        Pan(pan_delta);
    }
    else
        pawn_state_ = INACTIVE;

    previous_pointer_event_ = &move_event;
}

void ViewerPawn::apply(ScrollWheelEvent& scroll_wheel_event)
{
    if (scroll_wheel_event.handled
        || !EventRelevant(scroll_wheel_event)
        || !last_pointer_within_render_area_)
        return;

    scroll_wheel_event.handled = true;

    auto ndc = NDC(*previous_pointer_event_);

    Zoom(scroll_wheel_event.delta.y, ndc);
}

dvec3 ViewerPawn::TBC(const PointerEvent& event) const
{
    // need to patch aspect_fix as we need to get accurate screen shape
    const auto ndc = NDC(event, true);

    // get distance to the origin
    const auto len = length(ndc);

    // if the point is inside the trackball
    if (len < 1.f)
    {
        // get height
        const auto height = 0.5 + cos(len * PI) * 0.5;
        return {ndc.x, -ndc.y, height};
    }
    return {ndc.x, -ndc.y, 0};
}

void ViewerPawn::SetIntersectionPoint(const dvec3& point)
{
    intersect_.Collapse(point);
}

void ViewerPawn::ClearIntersection()
{
    intersect_.Superpose();
}

void ViewerPawn::Rotate(double angle, const dvec3& axis) const
{
    auto look_at = camera_->viewMatrix.cast<LookAt>();
    
    auto base = look_at->center;
    if (intersect_.observed)
    {
        base = intersect_.Reveal();
    }
    
    camera_->Rotate(angle, axis, base);
}

void ViewerPawn::Pan(const dvec2& delta) const
{
    auto pre_ndc = NDC(*previous_pointer_event_);
    auto ndc = pre_ndc + delta;

    auto world_pos = camera_->ScreenToWorld(ndc);
    auto world_pos_pre = camera_->ScreenToWorld(pre_ndc);

    // TODO: not tested yet
    if (GetCameraType() == XCamera::PERSPECTIVE && intersect_.observed)
    {
        auto position = camera_->GetPosition();

        const auto& intersect_point = intersect_.value;

        auto pre_ray_dir = world_pos_pre - position;
        auto pre_intersect_dir = intersect_point - position;
        auto ray_dir = world_pos - position;

        auto intersect_dir = ray_dir * (length(pre_intersect_dir) / length(pre_ray_dir));

        world_pos = intersect_dir + position;
        world_pos_pre = intersect_point;
    }

    auto movement = world_pos - world_pos_pre;

    // perform update
    camera_->Translate(-movement);
}

void ViewerPawn::Zoom(double ratio, const dvec2& base)
{
    const auto& inv_projection = camera_->GetInverseProjectionMatrix();
    const auto& inv_view = camera_->GetInverseViewMatrix();

    // vulkan use [0, 1] for ndc z instead of [-1, 1] in opengl
    auto clip_near_pos = dvec4(base.x, base.y, 0, 1);
    auto view_near_pos = inv_projection * clip_near_pos;

    const auto camera_type = GetCameraType();

    if (camera_type == XCamera::ORTHOGRAPHIC)
    {
        auto orthographic = camera_->projectionMatrix.cast<Orthographic>();

        // TODO: this should be a class member
        auto factor = ratio < 0 ? 1.2 : 1 / 1.2;

        auto view_height = abs(orthographic->top - orthographic->bottom);

        auto pre_height = view_height;
        view_height *= factor;

        // TODO: we should perform a clamp here
        // view_height = clamp(...)
        auto real_factor = view_height / pre_height;

        // we don't care view-space z in ortho-camera
        auto view_offset = dvec3(view_near_pos.x, view_near_pos.y, 0);
        // with w = 0, we ensure that only rotation and scaling (which is generally 1 in view space) are applied.
        // so camera is still in {0, 0, 0}
        auto world_offset = (inv_view * dvec4(view_offset, 0)).xyz;

        // with world_offset, we can calculate the camera's adjust translation
        auto translation = world_offset * (1 - real_factor);

        // calculate new viewport width
        auto viewport = camera_->getViewport();
        auto aspect_ratio = static_cast<double>(viewport.width) / static_cast<double>(viewport.height);
        auto view_width = view_height * aspect_ratio;

        // update projection
        orthographic->left = -view_width / 2.0;
        orthographic->right = view_width / 2.0;
        orthographic->bottom = -view_height / 2.0;
        orthographic->top = view_height / 2.0;

        // mark dirty
        camera_->ProjectionDirty();

        // perform update
        camera_->Translate(translation);
    }
    else if (camera_type == XCamera::PERSPECTIVE)
    {
        // perspective camera requires homogeneous division to obtain correct 3D coordinates
        view_near_pos /= view_near_pos.w;

        // with w = 0, we ensure that only rotation and scaling (which is generally 1 in view space) are applied.
        // so camera is still in {0, 0, 0}
        auto ray_dir = normalize((inv_view * dvec4(view_near_pos.xyz, 0)).xyz);

        // pixel-screen fix
        auto left_up = camera_->ScreenToWorld({-1, 1});
        auto left_down = camera_->ScreenToWorld({-1, -1});
        auto world_height = length(left_down - left_up);
        // one out of ten speed
        auto scale = world_height / 10;

        auto movement = ray_dir * ratio * scale;

        // perform updates
        camera_->Translate(movement);
    }
}
