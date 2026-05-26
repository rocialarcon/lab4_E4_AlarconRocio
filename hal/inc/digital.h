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

#ifndef DIGITAL_H_
#define DIGITAL_H_

/** @file digital.h
 ** @brief Declaraciones de la biblioteca para gestion de entradas y salidas digitales.
 ** @author Alarcon Rocio. 
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/** 
 * @brief Puntero a la estructura que representa una salida digital. 
 */
typedef struct digital_output_s * digital_output_t; 

/**
 * @brief Puntero a la estructura que representa a una entrda digital.
 */
typedef struct digital_input_s * digital_input_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/** 
 * @brief Crea e inicializa una instancia de una salida digital.
 * @param puerto Numero de puerto GPIO del microcontrolador.
 * @param terminal Numero de pin/bit dentro del puerto GPIO.
 * @return Instancia de la salida digital creada o NULL en caso de error.
 */
digital_output_t DigitalOutputCreate(uint32_t puerto, uint8_t terminal);

/**
 * @brief Activa (pone en '1' logico) la salida digital especificada.
 * @param salida Instancia de la salida digital.
 */
void DigitalOutputActivate(digital_output_t salida);

/**
 * @brief Desactiva (pone en '0' logico) la salida digital especificada.
 */
void DigitalOutputDesactivate(digital_output_t salida);

/**
 * @brief Invierte o conmuta el estado actual de la salida digital especififcada.
 */
void DigitalOutputToggle(digital_output_t salida);

/**
 * @brief Crea e inicializa una instancia de una entrada digital.
 * @param puerto Numero de puerto GPIO del microcontrolador.
 * @param terminal Numero de pin/bit dentro del puerto GPIO.
 * @param invertido Instancia si la entrada posee logica invertida (true para pull-up, false para pull-down).
 * @return Instancia de la entrada digital creada o NULL en caso de error.
 */
digital_input_t DigitalInputCreate(uint32_t puerto, uint8_t terminal, bool invertido);

/**
 * @brief Devuelve el estado logico actual de la entrada digital.
 * @param entrada Instancia de la entrada digital.
 * @return true si la entrada esta activa logicamente, false si esta inactiva.
 */
bool DigitalInputGetState(digital_input_t entrada);

/**
 * @brief Determina si la entrada digital cambio de estado desde la ultima consulta.
 * @return true si hubo algun cambio de estado, false en caso contrario.
 */
int DigitalInputHasChanged(digital_input_t entrada);

/**
 * @brief Detecta si ocurrio un flanco de activacion (transicion de inactivo a activo) en la entrada.
 * @return true unicamente en el ciclo donde se detecta la transicion positiva.
 */
bool DigitalInputHasActivate(digital_input_t entrada);

/**
 * @brief Detecta si ocurrio un flanco de dessactivacion (transicion de activo a inactivo) en la entrada.
 * @return true unicamente en el ciclo donde se detecta la transicion de bajada.
 */
bool DigitalInputHasDesactivate(digital_input_t entrada);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_H_ */