//
// Created by ParadoxFang on 2025/6/4.
//

#ifndef VIEWERCAMERA_H
#define VIEWERCAMERA_H

#include <VSGExtra/app/XCamera.h>

namespace VSGExtra
{
    
    /**
     * Camera with explicit `LookAt` as view matrix.
     * Used as 3D CAD like Camera.
     */
    class VSGEXTRA_DECLSPEC ViewerCamera : public vsg::Inherit<XCamera, ViewerCamera>{
    public:
        ViewerCamera() = default;

        ViewerCamera(const vsg::ref_ptr<vsg::ProjectionMatrix>& in_projection_matrix,
                const vsg::ref_ptr<vsg::LookAt>& in_view_matrix,
                const vsg::ref_ptr<vsg::ViewportState>& in_viewport_state = {});

        void ToggleCamera();

        void Translate(const vsg::dvec3& translation) override;
        void Teleport(const vsg::dvec3& position) override;

        void Rotate(double angle_rad, const vsg::dvec3& axis) override;
        void Rotate(double angle_rad, const vsg::dvec3& axis, const vsg::dvec3& base) override;
        
        void Rotate(const vsg::dquat& rotation) override;
        void Rotate(const vsg::dquat& rotation, const vsg::dvec3& base) override;

        void FocusOn(const vsg::dvec3& position) override;

        vsg::dvec3 GetPosition() override;
        vsg::dvec3 GetFocal() override;

        vsg::dvec3 ScreenToWorld(const vsg::dvec2& ndc) override;
    };
}

EVSG_type_name(VSGExtra::ViewerCamera);

#endif //VIEWERCAMERA_H
