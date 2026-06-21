/*********************************************************************************************************************
Copyright 2016-2025, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2026, Alarcon Fajardo Rocio Belen <rocialarcon555@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file screen.c
 ** @brief Plantilla para la creación de archivos de código fuente en lenguaje C
 **/

/* === Headers files inclusions ================================================================ */

#include "screen.h"
#include <stdlib.h>
#include <string.h>

/* === Macros definitions ====================================================================== */

#ifndef DISPLAY_MAX_DIGITS 
#define DISPLAY_MAX_DIGITS 8
#endif

/* === Private data type declarations ========================================================== */

/**
 * @brief Descriptor interno de la pantalla multiplexada
 */
struct display_s {
    uint8_t digits;
    uint8_t active_digit;
    uint8_t flashing_from;
    uint8_t flashing_to;
    uint16_t flashing_frecuency;
    uint16_t flashing_count;
    uint8_t display_memory[DISPLAY_MAX_DIGITS];
    struct display_driver_s driver[1];
};
/* === Private function declarations =========================================================== */

static display_t DisplayAllocate(void);

/* === Private variable definitions ============================================================ */

static const uint8_t IMAGES[] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,             /**< 0 */
    SEGMENT_B | SEGMENT_C,                                                             /**< 1 */
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         /**< 2 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                         /**< 3 */
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                     /**< 4 */
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         /**< 5 */
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             /**< 6 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                 /**< 7 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, /**< 8 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                         /**< 9 */
};

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

static display_t DisplayAllocate(void) {
    static struct display_s instances[1] = {0};
    return &instances[0];
}

/* === Public function implementation ========================================================== */

display_t DisplayCreate(uint8_t digits, display_driver_t driver) {
    display_t display = DisplayAllocate();

    if(display != NULL) {
        display->digits = digits;
        display->active_digit = digits -1;

        display->flashing_from = 0;
        display->flashing_to = 0;
        display->flashing_frecuency = 0;
        display->flashing_count = 0;

        memset(display->display_memory, 0, sizeof(display->display_memory));

        memcpy(display->driver, driver, sizeof(struct display_driver_s));

        display->driver->UpdateSegments(0);
        display->driver->UpdateDigits(0);

    }
    return display;
}

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size) {
    memset(display->display_memory, 0, sizeof(display->display_memory));

    for(uint8_t i = 0; i < size && i < display->digits; i++) {
        if(number[i] <= 9) {
            display->display_memory[i] = IMAGES[number[i]];
        }
    }
}

void DisplayRefresh(display_t display) {
    display->driver->UpdateSegments(0);
    display->active_digit = (display->active_digit + 1) % display->digits;
    display->driver->UpdateDigits(1 << display->active_digit);
    
    uint8_t segmentos = display->display_memory[display->active_digit];

    if (display->flashing_frecuency > 0)
    {
        if (display->active_digit == 0)
        {
            display->flashing_count++;
            if (display->flashing_count >=  display->flashing_frecuency)
            {
                display->flashing_count = 0;
            }
            
        }
        if (display->active_digit >= display->flashing_from && display->active_digit <= display->flashing_to)
        {
            if (display->flashing_count >=(display->flashing_frecuency / 2))
            {
                segmentos = 0;
            }
        }
    }
    display->driver->UpdateSegments(segmentos);
    
}

void DisplayFlashDigits(display_t display, uint8_t from, uint8_t to, uint16_t frecuency) {
    display->flashing_from = from;
    display->flashing_to = to;
    display->flashing_frecuency = frecuency;
    display->flashing_count =0;
}

void DisplayToggleDots(display_t display, uint8_t from, uint8_t to) {

}
/* === End of documentation ==================================================================== */