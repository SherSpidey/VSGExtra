//
// Created by ParadoxFang on 2025/6/4.
//

#include <VSGExtra/app/XCamera.h>

using namespace vsg;
using namespace VSGExtra;

XCamera::XCamera():
    camera_type_(ORTHOGRAPHIC)
{
}

XCamera::XCamera(
    const ref_ptr<ProjectionMatrix>& in_projection_matrix,
    const ref_ptr<ViewMatrix>& in_view_matrix,
    const ref_ptr<ViewportState>& in_viewport_state):
    Inherit(in_projection_matrix, in_view_matrix, in_viewport_state)
{
    update_camera_type_();
}

XCamera::CameraType XCamera::get_camera_type() const
{
    return camera_type_;
}

void XCamera::update_camera_type_()
{
    const auto& camera_info = projectionMatrix->type_info();
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

dmat4 XCamera::GetProjectionMatrix() const
{
    return projectionMatrix->transform();
}

const dmat4& XCamera::GetInverseProjectionMatrix()
{
    // need to update data
    if (inverse_projection_.dirty)
    {
        update_inverse_projection_();
    }

    return inverse_projection_.Get();
}

const dmat4& XCamera::GetInverseViewMatrix()
{
    // need to update data
    if (inverse_view_.dirty)
    {
        update_inverse_view_();
    }

    return inverse_view_.Get();
}

dmat4 XCamera::GetViewMatrix() const
{
    return viewMatrix->transform();
}

void XCamera::SetProjectionMatrix(const ref_ptr<ProjectionMatrix>& in_projection_matrix)
{
    projectionMatrix = in_projection_matrix;
    update_camera_type_();

    // Mark data dirty
    ProjectionDirty();
}

void XCamera::SetViewMatrix(const ref_ptr<ViewMatrix>& in_view_matrix)
{
    viewMatrix = in_view_matrix;

    // Mark data dirty
    ViewDirty();
}

void XCamera::update_inverse_projection_()
{
    // calculate up-date inverse projection matrix
    auto projection = GetProjectionMatrix();

    // set new value
    inverse_projection_.Set(inverse(projection));
}

void XCamera::update_inverse_view_()
{
    // calculate up-date inverse view matrix
    auto view = GetViewMatrix();

    // set new value
    inverse_view_.Set(inverse(view));
}

dvec3 XCamera::GetPosition()
{
    return {};
}

void XCamera::ProjectionDirty()
{
    inverse_projection_.Dirty();
}

void XCamera::ViewDirty()
{
    inverse_view_.Dirty();
}

dvec3 XCamera::ScreenToWorld(const dvec2& ndc)
{
    return {};
}
