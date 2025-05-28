//
// Created by ParadoxFang on 2025/5/27.
//

#include <vsg/ui/PointerEvent.h>

#include <VSGExtra/app/DefaultPawn.h>

using namespace vsg;
using namespace VSGExtra;

DefaultPawn::DefaultPawn(const ref_ptr<Camera>& camera):
    camera_(camera),
    has_keyboard_focus_(false),
    in_animation_(false),
    animation_time_stamp_(0)
{
   CheckCameraType();
}

void DefaultPawn::PossessedBy(const ref_ptr<Object>& target)
{
    if (auto camera = target.cast<Camera>())
    {
        camera_ = camera;
        CheckCameraType();
    }
}

void DefaultPawn::UnPossessed()
{
    // release the ptr
    camera_ = nullptr;
}

void DefaultPawn::CheckCameraType()
{
    const auto& camera_info = camera_->projectionMatrix->type_info();
    if (camera_info == typeid(Perspective))
        camera_type_ = PERSPECTIVE;
    else if (camera_info == typeid(Orthographic))
        camera_type_ = ORTHOGRAPHIC;
    else if (camera_info == typeid(RelativeProjection))
        camera_type_ = RELATIVE_PROJECTION;
    else if (camera_info == typeid(EllipsoidPerspective))
        camera_type_ = ELLIPSOID_PERSPECTIVE;
    else
        camera_type_ = UNKNOWN;
}

std::pair<int32_t, int32_t> DefaultPawn::CameraRenderAreaCoordinates(const PointerEvent& event) const
{
    // find in preserved window first
    if (!window_offsets.empty())
    {
        auto itr = window_offsets.find(event.window);
        if (itr != window_offsets.end())
        {
            const auto& offset = itr->second;
            return {event.x + offset.x, event.y + offset.y};
        }
    }
    return {event.x, event.y};
}

bool DefaultPawn::WithinRenderArea(const PointerEvent& event) const
{
    const auto [offset, extent] = camera_->getRenderArea();
    auto [x, y] = CameraRenderAreaCoordinates(event);

    return x >= offset.x && x < static_cast<int32_t>(offset.x + extent.width) &&
        y >= offset.y && y < static_cast<int32_t>(offset.y + extent.height);
}

bool DefaultPawn::EventRelevant(const WindowEvent& event) const
{
    // if no windows have been associated with AddWindow()
    // then assume event is relevant and should be handled
    if (window_offsets.empty()) return true;

    return window_offsets.find(event.window) != window_offsets.end();
}

void DefaultPawn::AddWindow(const ref_ptr<Window>& window, const ivec2& offset)
{
    window_offsets[observer_ptr<Window>(window)] = offset;
}

void DefaultPawn::AddKeyViewpoint(KeySymbol key, const Viewpoint& viewpoint)
{
    key_viewpoint_map[key] = viewpoint;
}

void DefaultPawn::AddKeyViewpoint(KeySymbol key, const ref_ptr<LookAt>& look_at, double duration)
{
    key_viewpoint_map[key] = {look_at, duration};
}

void DefaultPawn::SetViewpoint(const Viewpoint& viewpoint)
{
    SetViewpoint(viewpoint.look_at, viewpoint.duration);
}

void DefaultPawn::SetViewpoint(const ref_ptr<LookAt>& look_at, double duration)
{
    if (!look_at)
        return;

    auto view = camera_->viewMatrix.cast<LookAt>();

    // instant mode
    if (duration <= 1.e-4)
    {
        // *view = *look_at;
        // more readable
        view->eye = look_at->eye;
        view->center = look_at->center;
        view->up = look_at->up;

        in_animation_ = false;
        start_look_at_ = nullptr;
        end_look_at_ = nullptr;
        animation_time_stamp_ = 0.0;
    }
    // set animation info
    else
    {
        in_animation_ = true;
        animation_start_time_ = previous_time_;
        start_look_at_ = LookAt::create(*view);
        end_look_at_ = look_at;
        animation_time_stamp_ = duration;
    }
}

void DefaultPawn::apply(KeyPressEvent& key_press_event)
{
    // tracking state
    Inherit::apply(key_press_event);

    // check whether to set viewport
    if (!has_keyboard_focus_ || key_press_event.handled || !EventRelevant(key_press_event)) return;

    if (auto itr = key_viewpoint_map.find(key_press_event.keyBase); itr != key_viewpoint_map.end())
    {
        previous_time_ = key_press_event.time;

        SetViewpoint(itr->second.look_at, itr->second.duration);

        key_press_event.handled = true;
    }
}

void DefaultPawn::apply(ButtonPressEvent& button_press_event)
{
    if (button_press_event.handled || !EventRelevant(button_press_event))
    {
        has_keyboard_focus_ = false;
        return;
    }

    has_keyboard_focus_ = WithinRenderArea(button_press_event);
}

void DefaultPawn::apply(FrameEvent& frame_event)
{
    // need to apply animation
    if (in_animation_)
    {
        auto play_time = std::chrono::duration<double>(frame_event.time - animation_start_time_).count();
        auto look_at = camera_->viewMatrix.cast<LookAt>();
        // still playing
        if (play_time < animation_time_stamp_)
        {
            auto lerp = smoothstep(0.0, 1.0, play_time / animation_time_stamp_);

            look_at->eye = mix(start_look_at_->eye, end_look_at_->eye, lerp);
            look_at->center = mix(start_look_at_->center, end_look_at_->center, lerp);

            double angle = acos(
                dot(start_look_at_->up, end_look_at_->up) /
                (length(start_look_at_->up) * length(end_look_at_->up))
            );

            if (angle > 1.0e-6)
            {
                auto rotation = rotate(angle * lerp,
                                       normalize(cross(start_look_at_->up, end_look_at_->up)));
                look_at->up = rotation * start_look_at_->up;
            }
            else
            {
                look_at->up = end_look_at_->up;
            }
        }
        // animation finished
        else
        {
            in_animation_ = false;

            look_at->eye = end_look_at_->eye;
            look_at->center = end_look_at_->center;
            look_at->up = end_look_at_->up;

            end_look_at_ = nullptr;
            animation_time_stamp_ = 0.0;
        }
    }

    previous_time_ = frame_event.time;
}

dvec2 DefaultPawn::NDC(const PointerEvent& event, bool aspect_fix) const
{
    const auto [offset, extent] = camera_->getRenderArea();
    const auto [x, y] = CameraRenderAreaCoordinates(event);

    const auto aspect_ratio = static_cast<double>(extent.width) / static_cast<double>(extent.height);
    const auto scale_x = (aspect_fix || extent.width > extent.height) ? 1.0 : aspect_ratio;
    const auto scale_y = (aspect_fix || extent.height > extent.width) ? 1.0 : 1.0 / aspect_ratio;

    auto ndc_x = extent.width > 0
                     ? (static_cast<double>(x - offset.x) / static_cast<double>(extent.width) * 2.0 - 1.0) * scale_x
                     : 0.0;
    auto ndc_y = extent.height > 0
                     ? (static_cast<double>(y - offset.y) / static_cast<double>(extent.height) * 2.0 - 1.0) * scale_y
                     : 0.0;

    return {ndc_x, ndc_y};
}
