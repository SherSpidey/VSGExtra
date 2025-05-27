//
// Created by ParadoxFang on 2025/5/16.
//

#include <VSGExtra/app/Pawn.h>

using namespace vsg;
using namespace VSGExtra;

Pawn::Pawn() :
    position_(dvec3()),
    forward_(dvec3(0, 1, 0)),
    right_(dvec3(1, 0, 0)),
    pitch_(0),
    yaw_(0),
    roll_(0),
    keyboard_(Keyboard::create())
{
}

void Pawn::apply(KeyPressEvent& key_press_event)
{
    if (keyboard_) key_press_event.accept(*keyboard_);
}

void Pawn::apply(KeyReleaseEvent& key_release_event)
{
    if (keyboard_) key_release_event.accept(*keyboard_);
}

void Pawn::apply(FocusInEvent& focus_in_event)
{
    if (keyboard_) focus_in_event.accept(*keyboard_);
}

void Pawn::apply(FocusOutEvent& focus_out_event)
{
    if (keyboard_) focus_out_event.accept(*keyboard_);
}

void Pawn::AddMovementInput(const dvec3& dir, double scale)
{
    position_ += dir * scale;
}

void Pawn::AddPitchInput(double val)
{
    if (val != 0.0)
    {
        pitch_ += val;
    }
}

void Pawn::AddYawInput(double val)
{
    if (val != 0.0)
    {
        yaw_ += val;
    }
}

void Pawn::AddRollInput(double val)
{
    if (val != 0.0)
    {
        roll_ += val;
    }
}

void Pawn::Zoom(double ratio, const dvec3& base)
{
    // do nothing in base class
}
