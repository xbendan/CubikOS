#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#define __INLINE__ __attribute__((always_inline)) inline
#define __PACKED__ __attribute__((packed))
// Align the nearest _lower_ aligned address
// ex: 8 with align = 8 -> 8
// ex: 9 with align = 8 -> 16
// ex: 7 with align = 8 -> 0
#define ALIGN_DOWN(__addr, __align) ((__addr) & ~((__align)-1))

// Align the nearest _upper_ aligned address
// ex: 8 with align = 8 -> 8
// ex: 9 with align = 8 -> 16
// ex: 7 with align = 8 -> 8
#define ALIGN_UP(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))



