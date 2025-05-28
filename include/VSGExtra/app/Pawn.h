//
// Created by ParadoxFang on 2025/5/16.
//

#ifndef PAWN_H
#define PAWN_H

#include <vsg/app/Camera.h>
#include <vsg/app/Window.h>
#include <vsg/ui/Keyboard.h>

#include <VSGExtra/export.h>
#include <VSGExtra/ui/KeyboardRegistry.h>

namespace VSGExtra
{
    /**
     *  Base class for controlling
     */
    class VSGEXTRA_DECLSPEC Pawn : public vsg::Inherit<vsg::Visitor, Pawn>
    {
    public:
        Pawn();

        // control object
        virtual void PossessedBy(const vsg::ref_ptr<Object>& target) = 0;
        // release control
        virtual void UnPossessed() = 0;

    protected:
        // pawn position
        vsg::dvec3 position_;

        // pawn direction
        vsg::dvec3 forward_, right_;

        // pawn attitude
        double pitch_, yaw_, roll_;

    public:
        // vsg style: all input callback come from apply function
        // used to record key state
        void apply(vsg::KeyPressEvent&) override;
        void apply(vsg::KeyReleaseEvent&) override;
        void apply(vsg::FocusInEvent&) override;
        void apply(vsg::FocusOutEvent&) override;

    protected:
        // record all input key state info
        vsg::ref_ptr<vsg::Keyboard> keyboard_;
        vsg::ref_ptr<KeyboardRegistry> keyboard_registry_;

    public:
        // dynamic states change
        void AddMovementInput(const vsg::dvec3& dir, double scale = 1.0f);
        void AddPitchInput(double val);
        void AddYawInput(double val);
        void AddRollInput(double val);

    protected:
        // update info using pawn attitude
        void Update();

        // zoom viewport
        virtual void Zoom(double ratio, const vsg::dvec3& base);
    };
}

EVSG_type_name(VSGExtra::Pawn);

#endif //PAWN_H
