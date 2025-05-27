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
    public:
        Pawn();

        /***** control *****/
        virtual void PossessedBy(const vsg::ref_ptr<Object>& target) = 0;
        virtual void UnPossessed() = 0;
        /***** control *****/

    protected:
        // pawn position
        vsg::dvec3 position_;

        // pawn direction
        vsg::dvec3 forward_, right_;

        // pawn attitude
        double pitch_, yaw_, roll_;

    public:
        /***** input *****/
        // vsg style: all input callback come from apply function
        void apply(vsg::KeyPressEvent&) override;
        void apply(vsg::KeyReleaseEvent&) override;
        void apply(vsg::FocusInEvent&) override;
        void apply(vsg::FocusOutEvent&) override;
        /***** input *****/

    protected:
        // record all input key state info
        vsg::ref_ptr<vsg::Keyboard> keyboard_;

    public:
        /***** movement *****/
        void AddMovementInput(const vsg::dvec3& dir, double scale = 1.0f);
        void AddPitchInput(double val);
        void AddYawInput(double val);
        void AddRollInput(double val);
        /***** movement *****/


        /***** camera *****/
        virtual void Zoom(double ratio, const vsg::dvec3& base = {});
        /***** camera *****/
    };
}

EVSG_type_name(VSGExtra::Pawn);

#endif //PAWN_H
