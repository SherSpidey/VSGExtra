//
// Created by ParadoxFang on 2025/5/29.
//

#include <vsg/app/ViewMatrix.h>
#include <VSGExtra/math/spatial.h>

vsg::dvec3 VSGExtra::ScreenToWorld(const vsg::dvec2& ndc,
                                   const vsg::dmat4& projection,
                                   const vsg::ref_ptr<vsg::LookAt>& look_at)
{
    return ScreenToWorld(ndc, projection, look_at->transform(), look_at->eye, look_at->center);
}
