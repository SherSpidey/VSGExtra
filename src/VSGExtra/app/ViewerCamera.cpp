//
// Created by ParadoxFang on 2025/6/4.
//

#include <iostream>

#include <VSGExtra/math/spatial.h>

#include <VSGExtra/app/ViewerCamera.h>

using namespace vsg;
using namespace VSGExtra;

ViewerCamera::ViewerCamera(
    const ref_ptr<ProjectionMatrix>& in_projection_matrix,
    const ref_ptr<LookAt>& in_view_matrix,
    const ref_ptr<ViewportState>& in_viewport_state):
    Inherit(in_projection_matrix, in_view_matrix, in_viewport_state)
{
    if (camera_type_ != ORTHOGRAPHIC && camera_type_ != PERSPECTIVE)
    {
        std::cerr << "Creating a ViewerCamera with the wrong type!\n";
    }
}

void ViewerCamera::ToggleCamera()
{
    auto look_at = viewMatrix.cast<LookAt>();
    auto focal = length(look_at->center - look_at->eye);

    if (camera_type_ == ORTHOGRAPHIC)
    {
        auto orthographic = projectionMatrix.cast<Orthographic>();

        double height = orthographic->top - orthographic->bottom;
        double width = orthographic->right - orthographic->left;
        double aspectRatio = width / height;

        auto perspective = Perspective::create();

        perspective->fieldOfViewY = 2.0 * degrees(atan2(height * 0.5, focal));
        perspective->aspectRatio = aspectRatio;
        perspective->nearDistance = orthographic->nearDistance;
        perspective->farDistance = orthographic->farDistance;

        projectionMatrix = perspective;
        camera_type_ = PERSPECTIVE;

        ProjectionDirty();
    }
    else if (camera_type_ == PERSPECTIVE)
    {
        auto perspective = projectionMatrix.cast<Perspective>();

        double height = 2.0 * focal * tan(radians(perspective->fieldOfViewY) * 0.5);
        double width = height * perspective->aspectRatio;

        auto orthographic = Orthographic::create();

        orthographic->left = -width * 0.5;
        orthographic->right = width * 0.5;
        orthographic->bottom = -height * 0.5;
        orthographic->top = height * 0.5;
        orthographic->nearDistance = perspective->nearDistance;
        orthographic->farDistance = perspective->farDistance;

        projectionMatrix = orthographic;
        camera_type_ = ORTHOGRAPHIC;

        ProjectionDirty();
    }
}

void ViewerCamera::Translate(const dvec3& translation)
{
    auto look_at = viewMatrix.cast<LookAt>();
    auto forward_dir = look_at->center - look_at->eye;

    look_at->eye += translation;
    look_at->center = look_at->eye + forward_dir;

    ViewDirty();
}

void ViewerCamera::Teleport(const dvec3& position)
{
    auto look_at = viewMatrix.cast<LookAt>();
    auto forward_dir = look_at->center - look_at->eye;

    look_at->eye = position;
    look_at->center = look_at->eye + forward_dir;

    ViewDirty();
}

void ViewerCamera::Rotate(double angle_rad, const dvec3& axis)
{
    auto look_at = viewMatrix.cast<LookAt>();
    auto base = look_at->center;

    Rotate(angle_rad, axis, base);
}

void ViewerCamera::Rotate(double angle_rad, const dvec3& axis, const dvec3& base)
{
    auto look_at = viewMatrix.cast<LookAt>();

    auto rotate_matrix = rotate(angle_rad, axis);
    auto view_matrix = GetViewMatrix();
    const auto& inverse_view = GetInverseViewMatrix();

    auto base_view_space = view_matrix * base;
    auto matrix = inverse_view * translate(base_view_space) *
        rotate_matrix * translate(-base_view_space) * view_matrix;

    // regular rotate
    look_at->up = normalize(matrix * (look_at->eye + look_at->up) - matrix * look_at->eye);
    look_at->center = matrix * look_at->center;
    look_at->eye = matrix * look_at->eye;

    ViewDirty();
}

void ViewerCamera::Rotate(const dquat& rotation)
{
    auto look_at = viewMatrix.cast<LookAt>();
    auto base = look_at->center;

    Rotate(rotation, base);
}

void ViewerCamera::Rotate(const dquat& rotation, const dvec3& base)
{
    auto look_at = viewMatrix.cast<LookAt>();

    auto rotate_matrix = rotate(rotation);
    auto view_matrix = GetViewMatrix();
    const auto& inverse_view = GetInverseViewMatrix();

    auto base_view_space = view_matrix * base;
    auto matrix = inverse_view * translate(base_view_space) *
        rotate_matrix * translate(-base_view_space) * view_matrix;

    // regular rotate
    look_at->up = normalize(matrix * (look_at->eye + look_at->up) - matrix * look_at->eye);
    look_at->center = matrix * look_at->center;
    look_at->eye = matrix * look_at->eye;

    ViewDirty();
}

void ViewerCamera::FocusOn(const dvec3& position)
{
    auto look_at = viewMatrix.cast<LookAt>();
    look_at->center = position;

    ViewDirty();
}

dvec3 ViewerCamera::GetPosition()
{
    const auto look_at = viewMatrix.cast<LookAt>();
    return look_at->eye;
}

dvec3 ViewerCamera::GetFocal()
{
    const auto look_at = viewMatrix.cast<LookAt>();
    return look_at->center;
}

dvec3 ViewerCamera::ScreenToWorld(const dvec2& ndc)
{
    const auto look_at = viewMatrix.cast<LookAt>();
    return ScreenToWorldInternal(
        ndc,
        GetInverseProjectionMatrix(),
        GetInverseViewMatrix(),
        look_at->eye,
        look_at->center
    );
}
