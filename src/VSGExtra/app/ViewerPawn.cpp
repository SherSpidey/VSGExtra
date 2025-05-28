//
// Created by ParadoxFang on 2025/5/16.
//

#include <iostream>
#include <vsg/ui/PointerEvent.h>

#include <VSGExtra/app/ViewerPawn.h>

using namespace vsg;
using namespace VSGExtra;

ViewerPawn::ViewerPawn(const ref_ptr<Camera>& camera) :
    Inherit(camera),
    pawn_state_(INACTIVE),
    has_pointer_focus_(false),
    last_pointer_within_render_area_(false)
{
}

void ViewerPawn::apply(ButtonPressEvent& button_press_event)
{
    if (button_press_event.handled || !EventRelevant(button_press_event))
    {
        has_keyboard_focus_ = false;
        return;
    }

    last_pointer_within_render_area_ =
        has_pointer_focus_ =
        has_keyboard_focus_ = WithinRenderArea(button_press_event);

    // update pos
    current_pos = CameraRenderAreaCoordinates(button_press_event);

    // state machine
    if (keyboard_registry_->PressedExclusive(KEY_Shift_L) &&
        button_press_event.mask & BUTTON_MASK_2)
    {
        pawn_state_ = ROTATE;
    }
    else if (keyboard_registry_->Idle() &&
        button_press_event.mask & BUTTON_MASK_2)
    {
        pawn_state_ = PAN;
    }
    else
        pawn_state_ = INACTIVE;

    if (has_pointer_focus_) button_press_event.handled = true;

    // TODO: handle begin state data
    
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

void ViewerPawn::Rotate(double angle, const dvec3& axis, const dvec3& base)
{
}

void ViewerPawn::Pan(const dvec2& delta)
{
}

void ViewerPawn::Zoom(double ratio, const dvec3& base)
{
    Inherit<DefaultPawn, ViewerPawn>::Zoom(ratio, base);
}
