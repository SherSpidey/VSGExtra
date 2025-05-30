//
// Created by ParadoxFang on 2025/5/29.
//

#ifndef SPATIAL_H
#define SPATIAL_H

#include <vulkan/vulkan_core.h>

#include <vsg/maths/transform.h>

#include <VSGExtra/export.h>

namespace VSGExtra
{
    /// get ndc, e.g. [-1, 1]
    template <typename T>
    vsg::t_vec2<T> NDC(const VkRect2D& area, int32_t x, int32_t y)
    {
        const auto [offset, extent] = area;

        auto ndc_x = extent.width > 0
                         ? (static_cast<T>(x - offset.x) / static_cast<T>(extent.width) * 2.0 - 1.0)
                         : 0.0;

        auto ndc_y = extent.height > 0
                         ? (static_cast<T>(y - offset.y) / static_cast<T>(extent.height) * 2.0 - 1.0)
                         : 0.0;

        return vsg::t_vec2<T>(ndc_x, ndc_y);
    }

    /// screen position to world position
    template <typename T>
    vsg::t_vec3<T> ScreenToWorldInternal(
        const vsg::t_vec2<T>& ndc,
        const vsg::t_mat4<T>& inv_projection,
        const vsg::t_mat4<T>& inv_view,
        const vsg::t_vec3<T>& eye,
        const vsg::t_vec3<T>& center)
    {
        bool ortho = std::abs(inv_projection[2][3]) < T(1e-6);

        // plane we get the target point
        auto focal_dir = center - eye;

        // vulkan use [0, 1] for ndc z instead of [-1, 1] in opengl
        auto clip_near_pos = vsg::t_vec4<T>(ndc.x, ndc.y, 0, 1);
        auto view_near_pos = inv_projection * clip_near_pos;

        if (!ortho)
        {
            // perspective camera requires homogeneous division to obtain correct 3D coordinates
            view_near_pos /= view_near_pos.w;

            // with w = 0, we ensure that only rotation and scaling (which is generally 1 in view space) are applied.
            // so camera is still in {0, 0, 0}
            auto ray_dir = vsg::normalize((inv_view * vsg::t_vec4<T>(view_near_pos.xyz, 0)).xyz);

            // standard ray-plane intersection scale
            auto forward = vsg::normalize(focal_dir);
            auto denominator = vsg::dot(ray_dir, forward);
            // returns eye if nearly parallel(almost can not happen)
            if (std::abs(denominator) < T(1e-6))
            {
                return eye;
            }
            auto t = vsg::dot(focal_dir, forward) / denominator;

            return eye + ray_dir * t;
        }

        // ortho-camera
        auto world_new_pos = inv_view * view_near_pos;
        return world_new_pos.xyz + focal_dir;
    }

    /// screen position to world position
    template <typename T>
    vsg::t_vec3<T> ScreenToWorld(
        const vsg::t_vec2<T>& ndc,
        const vsg::t_mat4<T>& projection,
        const vsg::t_mat4<T>& view,
        const vsg::t_vec3<T>& eye,
        const vsg::t_vec3<T>& center)
    {
        auto inv_projection = vsg::inverse(projection);
        auto inv_view = vsg::inverse(view);

        return ScreenToWorldInternal(ndc, inv_projection, inv_view, eye, center);
    }

    // common 'ScreenToWorld' function
    vsg::dvec3 VSGEXTRA_DECLSPEC ScreenToWorld(const vsg::dvec2& ndc,
                                               const vsg::dmat4& projection,
                                               const vsg::ref_ptr<vsg::LookAt>& look_at);
}

#endif //SPATIAL_H
