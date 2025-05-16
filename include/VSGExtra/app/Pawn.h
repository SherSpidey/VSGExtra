//
// Created by ParadoxFang on 2025/5/16.
//

#ifndef PAWN_H
#define PAWN_H

#include <vsg/app/Camera.h>
#include <vsg/app/Window.h>
#include <vsg/ui/Keyboard.h>

#include <VSGExtra/export.h>

namespace VSGExtra
{
    /**
     *  Base class for controlling
     */
    class VSGEXTRA_DECLSPEC Pawn : public vsg::Inherit<vsg::Visitor, Pawn>
    {
    };

    /**
     * Base class used to directly control camera
     */
    class VSGEXTRA_DECLSPEC DefaultPawn : public vsg::Inherit<Pawn, DefaultPawn>
    {
    public:
        struct Viewpoint
        {
            vsg::ref_ptr<vsg::LookAt> look_at;
            double duration = 0.0;
        };

        DefaultPawn() = delete;
        explicit DefaultPawn(const vsg::ref_ptr<vsg::Camera>& camera);

        // add a preserved window with a offset
        void AddWindow(const vsg::ref_ptr<vsg::Window>& window, const vsg::ivec2& offset = {});

        // add a preserved viewpoint-key binding
        void AddKeyViewpoint(vsg::KeySymbol key, const Viewpoint& viewpoint = {});
        void AddKeyViewpoint(vsg::KeySymbol key, const vsg::ref_ptr<vsg::LookAt>& look_at, double duration = 1.0);

        // set view to the input viewpoint
        void SetViewpoint(const Viewpoint& viewpoint);
        void SetViewpoint(const vsg::ref_ptr<vsg::LookAt>& look_at, double duration = 1.0);

        // core functions
        void apply(vsg::KeyPressEvent&) override;
        void apply(vsg::KeyReleaseEvent&) override;
        void apply(vsg::FocusInEvent&) override;
        void apply(vsg::FocusOutEvent&) override;
        void apply(vsg::ButtonPressEvent&) override;
        void apply(vsg::ButtonReleaseEvent&) override;
        void apply(vsg::MoveEvent&) override;
        void apply(vsg::ScrollWheelEvent&) override;
        void apply(vsg::TouchDownEvent&) override;
        void apply(vsg::TouchUpEvent&) override;
        void apply(vsg::TouchMoveEvent&) override;
        void apply(vsg::FrameEvent&) override;

    protected:
        vsg::ref_ptr<vsg::Camera> _camera;
        vsg::ref_ptr<vsg::LookAt> _look_at;
        vsg::ref_ptr<vsg::Keyboard> _keyboard;

        // preserved windows and corresponding offsets
        std::map<vsg::observer_ptr<vsg::Window>, vsg::ivec2> window_offsets;
        /// container that maps key symbol bindings with the Viewpoint
        /// that should move the LookAt to when pressed.
        std::map<vsg::KeySymbol, Viewpoint> key_viewpoint_map;

        // get screen-space coordinates
        std::pair<int32_t, int32_t> CameraRenderAreaCoordinates(const vsg::PointerEvent& event) const;
        // whether point event is within render area
        bool WithinRenderArea(const vsg::PointerEvent& event) const;
        // is the event is relevant with the preserved window
        bool EventRelevant(const vsg::PointerEvent& event) const;


        // get NDC, e.g. [-1, 1]
        // with aspect_fix equals to true will get a non [-1, 1] coords
        // which can only used in 2d screen space like TBC
        vsg::dvec2 NDC(const vsg::PointerEvent& event, bool aspect_fix = false) const;

        // get trackball coordinates
        vsg::dvec3 TBC(const vsg::PointerEvent& event) const;

        // manipulations
        virtual void Rotate(double angle, const vsg::dvec3& axis, const vsg::dvec3& base = {});
        virtual void Zoom(double ratio, const vsg::dvec3& base = {});
        virtual void Pan(const vsg::dvec2& delta);
    };
}

EVSG_type_name(VSGExtra::Pawn);
EVSG_type_name(VSGExtra::DefaultPawn);

#endif //PAWN_H
