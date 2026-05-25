/*********************************************************************************************************************
Copyright 2016-2025, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2016-2025, Esteban Volentini <evolentini@herrera.unt.edu.ar>

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

/** @file digital.c
 ** @brief Declaraciones de la biblioteca para gestion de netradas y salidas digitales
 **/


/* === Headers files inclusions ================================================================ */

#include "digital.h"
#include "chip.h"
#include <stdlib.h>

/* === Macros definitions ====================================================================== */
#define ACTIVATE_EVENT    1
#define DEACTIVATE_EVENT -1
/* === Private data type declarations ========================================================== */

struct digital_output_s {
    uint32_t puerto;
    uint8_t terminal;
};

struct digital_input_s {
    uint32_t puerto;
    uint8_t terminal;
    bool invertido;
    bool last_state;
};
/* === Private function declarations =========================================================== */

/* === Private variable definitions ============================================================ */

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */
digital_output_t DigitalOutputCreate(uint32_t puerto, uint8_t terminal) {
    digital_output_t self;
    self = malloc(sizeof(struct digital_output_s));
    if (self){
        self->puerto = puerto;
        self->terminal = terminal;
        DigitalOutputDesactivate(self);
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, false);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->puerto, self->terminal, true);
    }
    return self;
}

void DigitalOutputActivate(digital_output_t self) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, true);

}

void DigitalOutputDesactivate(digital_output_t self) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, false);

}

void DigitalOutputToggle(digital_output_t self) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self->puerto, self->terminal);
}


digital_input_t DigitalInputCreate(uint32_t puerto, uint8_t terminal, bool invertido) {
    digital_input_t self;
    self = malloc(sizeof(struct digital_input_s));
    if (self)
    {
        self->puerto = puerto;
        self->terminal = terminal;
        self->invertido = invertido;
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->puerto, self->terminal, false);
        self->last_state = DigitalInputGetState(self);
    }
    return self;
}
bool DigitalInputGetState(digital_input_t self) {
    bool state = false;
    if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->puerto, self->terminal) != 0)
    {
        state = true;
    }
    if (self->invertido)
    {
        state = !state;
    }
    return state;
}

int DigitalInputHasChanged(digital_input_t self) {
    int resultado = 0;
    bool actual = DigitalInputGetState(self);
    if (actual && !self->last_state)
    {
        resultado = ACTIVATE_EVENT;
    } else if (!actual && self->last_state)
    {
        resultado = DEACTIVATE_EVENT;
    }
    self->last_state = actual;
    return resultado;
}

bool DigitalInputHasActivate(digital_input_t self) {
    return DigitalInputHasChanged(self) == ACTIVATE_EVENT;
}

bool DigitalInputHasDesactivate(digital_input_t self) {
    return DigitalInputHasChanged(self) == DEACTIVATE_EVENT;
}
/* === End of documentation ==================================================================== */