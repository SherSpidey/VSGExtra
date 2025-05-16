//
// Created by ParadoxFang on 2025/5/16.
//

#include <vsg/ui/PointerEvent.h>

#include <VSGExtra/app/Pawn.h>

using namespace vsg;
using namespace VSGExtra;

DefaultPawn::DefaultPawn(const ref_ptr<Camera>& camera):
    _camera(camera),
    _look_at(camera->viewMatrix.cast<LookAt>()),
    _keyboard(Keyboard::create())
{
    if (!_look_at)
    {
        _look_at = new LookAt;
    }
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

    // _thrown = false;

    // instant mode
    if (duration <= 1.e-4)
    {
        // *_look_at = *look_at;
        // more readable
        _look_at->eye = look_at->eye;
        _look_at->center = look_at->center;
        _look_at->up = look_at->up;

        // _startLookAt = nullptr;
        // _endLookAt = nullptr;
        // _animationDuration = 0.0;
    }
    else
    {
    }
}

void DefaultPawn::apply(KeyPressEvent& event)
{
    // record keyboard states
    if (_keyboard) event.accept(*_keyboard);
}

void DefaultPawn::apply(KeyReleaseEvent& event)
{
    // record keyboard states
    if (_keyboard) event.accept(*_keyboard);
}

void DefaultPawn::apply(FocusInEvent& event)
{
}

void DefaultPawn::apply(FocusOutEvent& event)
{
}

void DefaultPawn::apply(ButtonPressEvent& event)
{
}

void DefaultPawn::apply(ButtonReleaseEvent& event)
{
}

void DefaultPawn::apply(MoveEvent& event)
{
}

void DefaultPawn::apply(ScrollWheelEvent& event)
{
}

void DefaultPawn::apply(TouchDownEvent& event)
{
}

void DefaultPawn::apply(TouchUpEvent& event)
{
}

void DefaultPawn::apply(TouchMoveEvent& event)
{
}

void DefaultPawn::apply(FrameEvent& event)
{
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
    const auto [offset, extent] = _camera->getRenderArea();
    auto [x, y] = CameraRenderAreaCoordinates(event);

    return x >= offset.x && x < static_cast<int32_t>(offset.x + extent.width) &&
        y >= offset.y && y < static_cast<int32_t>(offset.y + extent.height);
}

bool DefaultPawn::EventRelevant(const PointerEvent& event) const
{
    // if no windows have been associated with Trackball with a Trackball::addWindow()
    // then assume event is relevant and should be handled
    if (window_offsets.empty()) return true;

    return window_offsets.find(event.window) != window_offsets.end();
}

dvec2 DefaultPawn::NDC(const PointerEvent& event, bool aspect_fix) const
{
    const auto [offset, extent] = _camera->getRenderArea();
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

dvec3 DefaultPawn::TBC(const PointerEvent& event) const
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

void DefaultPawn::Rotate(double angle, const dvec3& axis, const dvec3& base)
{
}

void DefaultPawn::Zoom(double ratio, const dvec3& base)
{
}

void DefaultPawn::Pan(const dvec2& delta)
{
}
