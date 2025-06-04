//
// Created by ParadoxFang on 2025/5/16.
//

#ifndef VIEWERPAWN_H
#define VIEWERPAWN_H

#include <VSGExtra/app/DefaultPawn.h>

namespace VSGExtra
{
    /**
     * Standard camera control utility for a CAD 3D viewer.
     * 
     * Provides interaction mechanisms for manipulating the camera using pointer and key events.
     * Supports rotation, panning, and zooming within the 3D viewport, ensuring a smooth user experience.
     * 
     * IMPORTANT:
     * - Since `ViewerPawn` modifies the camera's internal state dynamically, 
     *   external references to the projection or view matrix instance should not be persistently stored.
     */
    class VSGEXTRA_DECLSPEC ViewerPawn : public vsg::Inherit<DefaultPawn, ViewerPawn>
    {
    public:
        enum ViewerPawnState : std::uint8_t
        {
            INACTIVE = 0,
            ROTATE,
            PAN
        };

        ViewerPawn() = delete;
        explicit ViewerPawn(const vsg::ref_ptr<XCamera>& camera);

        // get current pawn state
        ViewerPawnState get_pawn_state() const;

    protected:
        ViewerPawnState pawn_state_;

    public:
        // core functions
        using Inherit::apply;

        void apply(vsg::KeyPressEvent&) override;

        void apply(vsg::ButtonPressEvent&) override;
        void apply(vsg::ButtonReleaseEvent&) override;
        void apply(vsg::MoveEvent&) override;
        void apply(vsg::ScrollWheelEvent&) override;
        // void apply(vsg::FrameEvent&) override;

        // TODO: Touch pad interaction

    protected:
        // basic state info
        // indicates whether the pointer is currently active (pressed)
        bool has_pointer_focus_;
        // determines if the pointer is within the render area for zooming
        bool last_pointer_within_render_area_;
        vsg::ref_ptr<vsg::PointerEvent> previous_pointer_event_;

        // get trackball coordinates
        vsg::dvec3 TBC(const vsg::PointerEvent& event) const;

    public:
        // set current intersection point
        void SetIntersectionPoint(const vsg::dvec3& point);
        // clear intersection
        void ClearIntersection();

    protected:
        // ViewerPawn should not have direct scene access
        // all intersection info must be provided externally
        // however, intersection is not always required

        // the current intersection point data
        QuantumVec3D intersect_;

        // manipulations
        // rotates the view around a given axis from a specified base point
        void Rotate(double angle, const vsg::dvec3& axis, const vsg::dvec3& base) const;
        // pans the view in screen space based on the provided delta
        void Pan(const vsg::dvec2& delta) const;

        // override zoom function 
        void Zoom(double ratio, const vsg::dvec2& base) override;
    };
}

EVSG_type_name(VSGExtra::ViewerPawn)

#endif //VIEWERPAWN_H
