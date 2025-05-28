//
// Created by ParadoxFang on 2025/5/28.
//

#ifndef KEYBOARDREGISTRY_H
#define KEYBOARDREGISTRY_H

#include <vsg/ui/KeyEvent.h>

#include <VSGExtra/export.h>

namespace VSGExtra
{
    /**
     *  A simple class to monitor all pressed key
     */
    class VSGEXTRA_DECLSPEC KeyboardRegistry : public vsg::Inherit<vsg::Visitor, KeyboardRegistry>{
    public:
        void apply(vsg::KeyPressEvent&) override;
        void apply(vsg::KeyReleaseEvent&) override;
        void apply(vsg::FocusInEvent&) override;
        void apply(vsg::FocusOutEvent&) override;

        bool Idle() const;

        bool Pressed(vsg::KeySymbol key);
        bool PressedExclusive(vsg::KeySymbol key);

    protected:
        std::set<vsg::KeySymbol> key_pressed;
    };
}

EVSG_type_name(VSGExtra::KeyboardRegistry)

#endif //KEYBOARDREGISTRY_H
