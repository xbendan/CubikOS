#pragma once

#include <stdint.h>
#define KEYMAP_SIZE 128

namespace Device::Input
{
    enum KeyModifier
    {
        KeyModifierAlt = 0x1,
        KeyModifierCtrl = 0x2,
        KeyModifierShift = 0x4
    };

    enum KeyCode
    {
        KeyCode_Invalid
    };
}