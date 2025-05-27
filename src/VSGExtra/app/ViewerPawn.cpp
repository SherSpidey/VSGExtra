//
// Created by ParadoxFang on 2025/5/16.
//

#include <VSGExtra/app/ViewerPawn.h>

using namespace vsg;
using namespace VSGExtra;

ViewerPawn::ViewerPawn(const ref_ptr<Camera>& camera) : Inherit(camera)
{
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
