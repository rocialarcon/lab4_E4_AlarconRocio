/*********************************************************************************************************************
Copyright 2016-2025, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnologia
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

#ifndef BSP_H_
#define BSP_H_

/** @file bsp.h
 ** @brief Estructura que contiene los perifericos.
 ** @author Alarcon Rocio.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "digital.h"
#include "screen.h"
/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @brief Estructura que agrupa todos los perifericos de la placa.
 * Contiene los punteros a los objetos de hardware (entrdas y salidas)
 * que componen la placa, facilitando su acceso centralizado.
 */
typedef struct board_s {
    digital_output_t buzzer;

    digital_input_t f1;
    digital_input_t f2;
    digital_input_t f3;
    digital_input_t f4;
    digital_input_t aceptar;
    digital_input_t cancelar;

    display_t display;

} const * board_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */
/**
 * @brief Configura el hardware inicial y crea los objetos de los perifericos.
 * *Esta funcion inicializa los puertos de la EDU-CIAA, configura los multiplexores}
 * de pines y asigna la memoria necesaria  para controlar cada LED y tecla.
 * *@return board_t Puntero a la estructura que contiene los perifericos de la placa.
 */
board_t BoardCreate(void);
/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* BSP_H_ */