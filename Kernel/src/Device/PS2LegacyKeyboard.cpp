#include <Keyboard.h>

namespace Device::Input
{
    uint16_t keymapUsUnshifted[KEYMAP_SIZE] =
    {
        0,
        27, /* Escape */
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '-',
        '=',
        '\b', /* Backspace */ /* First line */
        '\t', /* Tab */
        'q',
        'w',
        'e',
        'r',
        't',
        'y',
        'u',
        'i',
        'o',
        'p',
        '[',
        ']',
        '\n', /* Enter */ /* Second line */
        272, /* Control */
        'a',
        's',
        'd',
        'f',
        'g',
        'h',
        'j',
        'k',
        'l',
        ';',
        '\'',
        '`',
        270, /* Left Shift */
        '\\',
        'z',
        'x',
        'c',
        'v',
        'b',
        'n',
        'm',
        ',',
        '.',
        '/',
        0, /* Right Shift */
        '*',
        271, /* Left Alt */
        ' ', /* Space */
        273, /* Caps Lock */
        256,
        257,
        258,
        259,
        260,
        261,
        262,
        263,
        264,
        265, /* F1 ~ F10 */
        0, /* Num Lock */
        0, /* Scroll Lock */
        276, /* Home */
        266, /* Arrow Up */
        0, /* Page Up */
        '-',
        267, /* Arrow Left */
        0,
        268, /* Arrow Right */
        '+',
        275, /* End */
        269, /* Arrow Down */
        0,
        0,
        274, /* Delete */
        0,
        0,
        0,
        0, /* F11 */
        0, /* F12 */
        0,
        0,
        0,
        0,
        280,
        280
    };

    uint16_t keymapUsShifted[KEYMAP_SIZE] =
    {
        0,
        27, /* Escape */
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '-',
        '=',
        '\b', /* Backspace */ /* First line */
        '\t', /* Tab */
        'q',
        'w',
        'e',
        'r',
        't',
        'y',
        'u',
        'i',
        'o',
        'p',
        '[',
        ']',
        '\n', /* Enter */ /* Second line */
        272, /* Control */
        'a',
        's',
        'd',
        'f',
        'g',
        'h',
        'j',
        'k',
        'l',
        ';',
        '\'',
        '`',
        270, /* Left Shift */
        '\\',
        'z',
        'x',
        'c',
        'v',
        'b',
        'n',
        'm',
        ',',
        '.',
        '/',
        0, /* Right Shift */
        '*',
        271, /* Left Alt */
        ' ', /* Space */
        273, /* Caps Lock */
        256,
        257,
        258,
        259,
        260,
        261,
        262,
        263,
        264,
        265, /* F1 ~ F10 */
        0, /* Num Lock */
        0, /* Scroll Lock */
        276, /* Home */
        266, /* Arrow Up */
        0, /* Page Up */
        '-',
        267, /* Arrow Left */
        0,
        268, /* Arrow Right */
        '+',
        275, /* End */
        269, /* Arrow Down */
        0,
        0,
        274, /* Delete */
        0,
        0,
        0,
        0, /* F11 */
        0, /* F12 */
        0,
        0,
        0,
        0,
        280,
        280
    };
}