//
// Created by ParadoxFang on 2025/6/4.
//

#ifndef XCAMERA_H
#define XCAMERA_H

#include <vsg/app/Camera.h>

#include <VSGExtra/export.h>
#include <VSGExtra/core/utility.h>

namespace VSGExtra
{
    /**
     * Extended camera class with properties management to ensure the consistency of
     * projection and view matrices by restricting direct external modifications.
     * 
     * IMPORTANT: All matrix updates should be done via the provided API to maintain the integrity
     * of inverse matrices!
     * 
     * If external modifications are necessary, use `Dirty` function to update before applying changes.
     */
    class VSGEXTRA_DECLSPEC XCamera : public vsg::Inherit<vsg::Camera, XCamera>
    {
    public:
        XCamera();

        XCamera(const vsg::ref_ptr<vsg::ProjectionMatrix>& in_projection_matrix,
                const vsg::ref_ptr<vsg::ViewMatrix>& in_view_matrix,
                const vsg::ref_ptr<vsg::ViewportState>& in_viewport_state = {});

        enum CameraType : std::uint8_t
        {
            ORTHOGRAPHIC = 0,
            PERSPECTIVE,
            RELATIVE_PROJECTION,
            ELLIPSOID_PERSPECTIVE,
            UNKNOWN,
        };

        CameraType get_camera_type() const;

    protected:
        CameraType camera_type_;

        void update_camera_type_();

    public:
        /** Matrix getter and setter **/
        vsg::dmat4 GetProjectionMatrix() const;
        const vsg::dmat4& GetInverseProjectionMatrix();
        vsg::dmat4 GetViewMatrix() const;
        const vsg::dmat4& GetInverseViewMatrix();

        // should only be used when camera is created without projection and view
        void SetProjectionMatrix(const vsg::ref_ptr<vsg::ProjectionMatrix>& in_projection_matrix);
        void SetViewMatrix(const vsg::ref_ptr<vsg::ViewMatrix>& in_view_matrix);

    protected:
        Matrix4DHolder inverse_projection_{};
        Matrix4DHolder inverse_view_{};

        void update_inverse_projection_();
        void update_inverse_view_();

    public:
        // IMPORTANT: XCamera does not implement these virtual functions itself.
        /** Manipulation functions **/
        virtual void Translate(const vsg::dvec3& translation) {}
        virtual void Teleport(const vsg::dvec3& position) {}

        virtual void Rotate(double angle_rad, const vsg::dvec3& axis) {}
        virtual void Rotate(double angle_rad, const vsg::dvec3& axis, const vsg::dvec3& base) {}
        
        virtual void Rotate(const vsg::dquat& rotation) {}
        virtual void Rotate(const vsg::dquat& rotation, const vsg::dvec3& base) {}

        virtual void FocusOn(const vsg::dvec3& position) {}

        /** Status **/
        virtual vsg::dvec3 GetPosition();
        virtual vsg::dvec3 GetFocal();

        // different from setter above, these functions are just copying value
        virtual void AssignWith(const vsg::ref_ptr<vsg::LookAt>& look_at) {}

        void ProjectionDirty();
        void ViewDirty();

        /** Utilities **/
        virtual vsg::dvec3 ScreenToWorld(const vsg::dvec2& ndc);
    };
}

EVSG_type_name(VSGExtra::XCamera);

#endif //XCAMERA_H
