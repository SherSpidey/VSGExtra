//
// Created by ParadoxFang on 2025/5/16.
//

#ifndef VIEWERPAWN_H
#define VIEWERPAWN_H

#include <VSGExtra/app/DefaultPawn.h>

namespace VSGExtra
{
    class VSGEXTRA_DECLSPEC ViewerPawn : public vsg::Inherit<DefaultPawn, ViewerPawn>
    {
    public:
        ViewerPawn() = delete;
        explicit ViewerPawn(const vsg::ref_ptr<vsg::Camera>& camera);

    protected:
        enum ViewerPawnState : std::uint8_t
        {
            INACTIVE = 0,
            ROTATE,
            PAN
        } pawn_state_;

    public:
        // core functions
        using Inherit::apply;
        void apply(vsg::ButtonPressEvent&) override;
        // void apply(vsg::ButtonReleaseEvent&) override;
        // void apply(vsg::MoveEvent&) override;
        // void apply(vsg::ScrollWheelEvent&) override;
        // void apply(vsg::FrameEvent&) override;
        
        // TODO: Touch pad interaction
    
    protected:
        // currently screen mouse pos
        std::pair<int32_t, int32_t> current_pos {};

        // basic state info
        bool has_pointer_focus_;
        bool last_pointer_within_render_area_;
        
        // get trackball coordinates
        vsg::dvec3 TBC(const vsg::PointerEvent& event) const;

        // manipulations
        void Rotate(double angle, const vsg::dvec3& axis, const vsg::dvec3& base);
        void Pan(const vsg::dvec2& delta);

        // override zoom function 
        void Zoom(double ratio, const vsg::dvec3& base) override;
    };
}

EVSG_type_name(VSGExtra::ViewerPawn)

#endif //VIEWERPAWN_H
