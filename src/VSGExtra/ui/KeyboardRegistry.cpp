//
// Created by ParadoxFang on 2025/5/28.
//

#include <VSGExtra/ui/KeyboardRegistry.h>

using namespace vsg;
using namespace VSGExtra;

void KeyboardRegistry::apply(KeyPressEvent& key_press_event)
{
    key_pressed.insert(key_press_event.keyBase);
}

void KeyboardRegistry::apply(KeyReleaseEvent& key_release_event)
{
    key_pressed.erase(key_release_event.keyBase);
}

void KeyboardRegistry::apply(FocusInEvent& focus_in_event)
{
}

void KeyboardRegistry::apply(FocusOutEvent& focus_out_event)
{
    focus_out_event.handled = true;
    key_pressed.clear();
}

bool KeyboardRegistry::Idle() const
{
    return key_pressed.empty();
}

bool KeyboardRegistry::Pressed(KeySymbol key)
{
    auto it = key_pressed.find(key);
    return it != key_pressed.end();
}

bool KeyboardRegistry::PressedExclusive(KeySymbol key)
{
    return Pressed(key) && key_pressed.size() == 1;
}
