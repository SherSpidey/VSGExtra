//
// Created by ParadoxFang on 2025/5/27.
//

#ifndef DEFAULTPAWN_H
#define DEFAULTPAWN_H

#include <VSGExtra/app/Pawn.h>

namespace VSGExtra
{
    /**
     * Base class for direct camera control.
     * Manages fundamental camera parameters and viewport states without enforcing strict control.
     * 
     * IMPORTANT: Since this class modifies camera values directly,
     * external references to the projection or view matrix should be carefully managed 
     * to avoid inconsistencies.
     */
    class VSGEXTRA_DECLSPEC DefaultPawn : public vsg::Inherit<Pawn, DefaultPawn>
    {
    public:
        struct Viewpoint
        {
            vsg::ref_ptr<vsg::LookAt> look_at;
            double duration = 0.0;
        };

        enum CameraType : std::uint8_t
        {
            ORTHOGRAPHIC = 0,
            PERSPECTIVE,
            RELATIVE_PROJECTION,
            ELLIPSOID_PERSPECTIVE,
            UNKNOWN,
        };

        // must be created with a camera
        DefaultPawn() = delete;
        explicit DefaultPawn(const vsg::ref_ptr<vsg::Camera>& camera);

        // use this call to switch between cameras
        void PossessedBy(const vsg::ref_ptr<Object>& target) override;
        // IMPORTANT: only can be called when the pawn is not in use
        // AND YOU SHOULD NOT NEED TO USE THIS FUNCTION WITHIN DEFAULT PAWN!
        void UnPossessed() override;

        CameraType get_camera_type() const;

    protected:
        CameraType camera_type_;

        // possessed camera
        vsg::ref_ptr<vsg::Camera> camera_;

        // update possessed camera type
        void CheckCameraType();
        // get screen-space coordinates
        std::pair<int32_t, int32_t> CameraRenderAreaCoordinates(const vsg::PointerEvent& event) const;
        // whether point event is within render area
        bool WithinRenderArea(const vsg::PointerEvent& event) const;
        // checks if the event is related to the preserved window
        bool EventRelevant(const vsg::WindowEvent& event) const;

    public:
        // add a preserved window with an offset
        void AddWindow(const vsg::ref_ptr<vsg::Window>& window, const vsg::ivec2& offset = {});

        // add a preserved viewpoint-key binding
        void AddKeyViewpoint(vsg::KeySymbol key, const Viewpoint& viewpoint = {});
        void AddKeyViewpoint(vsg::KeySymbol key, const vsg::ref_ptr<vsg::LookAt>& look_at, double duration = 1.0);

        // set view to the input viewpoint
        void SetViewpoint(const Viewpoint& viewpoint);
        void SetViewpoint(const vsg::ref_ptr<vsg::LookAt>& look_at, double duration = 1.0);

        // vsg style core functions
        using Inherit::apply;
        void apply(vsg::KeyPressEvent&) override;
        void apply(vsg::ButtonPressEvent&) override;
        void apply(vsg::FrameEvent&) override;

    protected:
        // basic state info
        bool has_keyboard_focus_;
        // previous time point
        vsg::time_point previous_time_{};

        // animation state info
        bool in_animation_;
        vsg::time_point animation_start_time_{};
        vsg::ref_ptr<vsg::LookAt> start_look_at_{};
        vsg::ref_ptr<vsg::LookAt> end_look_at_{};
        double animation_time_stamp_;

        // preserved windows and corresponding offsets
        std::map<vsg::observer_ptr<vsg::Window>, vsg::ivec2> window_offsets;
        // maps key symbols to viewpoints that should be activated when pressed
        std::map<vsg::KeySymbol, Viewpoint> key_viewpoint_map;

        // get NDC, e.g. [-1, 1]
        // if aspect_fix is true, returns non [-1, 1] coordinates,
        // which can only be used in 2d screen space like trackball coordinates
        vsg::dvec2 NDC(const vsg::PointerEvent& event, bool aspect_fix = false) const;
    };
}

EVSG_type_name(VSGExtra::DefaultPawn);

#endif //DEFAULTPAWN_H
