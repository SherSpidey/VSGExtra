//
// Created by ParadoxFang on 2025/5/16.
//

#ifndef PAWN_H
#define PAWN_H

#include <vsg/ui/Keyboard.h>

#include <VSGExtra/export.h>
#include <VSGExtra/ui/KeyboardRegistry.h>

namespace VSGExtra
{
    /**
     * Base class for controllable entities.
     * 
     * Provides fundamental movement, rotation, and interaction capabilities, making it suitable for cameras,
     * characters, and other interactive objects. This class handles input events and maintains object state, 
     * including position, direction, and orientation.
     * 
     * NOTE:
     * - Subclasses must implement control-specific logic to define behavior.
     */
    class VSGEXTRA_DECLSPEC Pawn : public vsg::Inherit<vsg::Visitor, Pawn>
    {
    public:
        Pawn();

        // controls the assigned object instance
        virtual void PossessedBy(const vsg::ref_ptr<Object>& target) = 0;
        // releases control of the currently possessed object
        virtual void UnPossessed() = 0;

    protected:
        // pawn position
        vsg::dvec3 position_;

        // pawn direction
        vsg::dvec3 forward_, right_;

        // pawn attitude
        double pitch_, yaw_, roll_;

    public:
        // vsg-style: All input callbacks are handled through the apply function
        // stores key state information
        void apply(vsg::KeyPressEvent&) override;
        void apply(vsg::KeyReleaseEvent&) override;
        void apply(vsg::FocusInEvent&) override;
        void apply(vsg::FocusOutEvent&) override;

    protected:
        // tracks all key input states
        vsg::ref_ptr<vsg::Keyboard> keyboard_;
        vsg::ref_ptr<KeyboardRegistry> keyboard_registry_;

    public:
        // handles dynamic state changes
        void AddMovementInput(const vsg::dvec3& dir, double scale = 1.0f);
        void AddPitchInput(double val);
        void AddYawInput(double val);
        void AddRollInput(double val);

    protected:
        // updates movement and rotation based on pawn attitude
        void Update();

        // adjusts viewport zoom level
        virtual void Zoom(double ratio, const vsg::dvec2& base);
    };
}

EVSG_type_name(VSGExtra::Pawn);

#endif //PAWN_H
