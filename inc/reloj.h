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

#ifndef RELOJ_H_
#define RELOJ_H_

/** @file reloj.h
 ** @brief Definicion de la API de la biblioteca para la gestion del reloj.
 ** @author Rocio Alarcon.
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
 * @brief Puntero opaco a la estructura de datos interna del reloj.
 */
typedef struct clock_s * clock_t;

/**
 * @brief Tipo de dato para almacenar la hora en formato BCD (Binary-Coded Decimal) sin compactar.
 * * El arreglo almacena 6 dígitos en el siguiente orden:
 * - [0]: Decenas de la hora
 * - [1]: Unidades de la hora
 * - [2]: Decenas de los minutos
 * - [3]: Unidades de los minutos
 * - [4]: Decenas de los segundos
 * - [5]: Unidades de los segundos
 */
typedef uint8_t hora_t[6];

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Crea e inicializa una nueva instancia del reloj.
 * @param ticks_por_segundo Cantidad de llamadas a RelojNewTick() necesarias para que el reloj avance un segundo real.
 * @param alarm_handler     Puntero a la función de callback que se ejecutará cuando suene la alarma (puede ser NULL).
 * @return clock_t          Puntero a la instancia del reloj creado.
 */
clock_t RelojCreate(unsigned int ticks_por_segundo, void * alarm_handler);

/**
 * @brief Obtiene la hora actual del reloj.
 * @param clock        Puntero a la instancia del reloj.
 * @param current_time Arreglo donde se copiará la hora actual en formato BCD.
 * @return true        Si la hora ha sido configurada previamente (es válida).
 * @return false       Si la hora nunca fue configurada desde el arranque (es inválida).
 */
bool RelojGetCurrentTime(clock_t clock, hora_t current_time);

/**
 * @brief Configura la hora actual del reloj y la marca como válida.
 * @param clock        Puntero a la instancia del reloj.
 * @param current_time Arreglo constante con la nueva hora a configurar en formato BCD.
 * @return true        Si la hora se configuró y guardó correctamente.
 */
bool RelojSetupCurrentTime(clock_t clock, const hora_t current_time);

/**
 * @brief Avanza un tick en el contador interno del reloj.
 * * Esta función debe llamarse periódicamente según la frecuencia configurada en RelojCreate().
 * Al alcanzar la cantidad de ticks por segundo, avanzará la hora matemática. Si la nueva 
 * hora coincide con la hora de la alarma (y esta se encuentra habilitada), invocará la función de callback.
 * @param clock Puntero a la instancia del reloj.
 */
void RelojNewTick(clock_t clock);

/**
 * @brief Fija la hora a la que debe dispararse la alarma.
 * @param clock      Puntero a la instancia del reloj.
 * @param alarm_time Arreglo constante con la hora de la alarma en formato BCD.
 */
void RelojSetupAlarm(clock_t clock, const hora_t alarma_time);

/**
 * @brief Obtiene la hora actualmente configurada para la alarma.
 * @param clock      Puntero a la instancia del reloj.
 * @param alarm_time Arreglo donde se copiará la hora de la alarma en formato BCD.
 */
bool RelojGetAlarm(clock_t clock, hora_t alarm_time);

/**
 * @brief Alterna el estado de la alarma (si estaba apagada la enciende, y viceversa).
 * @param clock  Puntero a la instancia del reloj.
 * @return true  Si el nuevo estado de la alarma es encendido (habilitado).
 * @return false Si el nuevo estado de la alarma es apagado (deshabilitado).
 */
bool RelojToggleAlarm(clock_t clock);

/**
 * @brief Pospone la alarma, añadiendo una cantidad de minutos a la hora previamente configurada.
 * @param clock          Puntero a la instancia del reloj.
 * @param minutos_demora Cantidad de minutos arbitrarios a sumar a la alarma original.
 */
void RelojSnoozeAlarm(clock_t clock, uint8_t minutos_demora);
/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* RELOJ_H_ */