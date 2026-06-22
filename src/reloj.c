

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

/** @file reloj.c
 ** @brief Implementacion de la biblioteca para la gestion del reloj.
 ** @author Rocio Alarcon
 **/

/* === Headers files inclusions ================================================================ */

#include <string.h>
#include "reloj.h"

/* === Macros definitions ====================================================================== */

/** @brief Cantidad total de segundos en un día (24h * 60m * 60s) */
#define SECONDS_PER_DAY 86400

/* === Private data type declarations ========================================================== */

/**
 * @brief Estructura interna del reloj (Descriptor)
 */
struct clock_s {
    uint32_t current_time;      /**< Segundos transcurridos desde la medianoche */
    uint16_t ticks_por_second;  /**< Velocidad configurada (ticks para 1 segundo) */
    uint16_t ticks_count;       /**< Ticks transcurridos desde el último segundo */
    uint32_t alarm_time;        /**< Segundos de la alarma desde medianoche */
    bool time_is_valid;         /**< Indica si el reloj fue puesto en hora */
    bool alarm_enabled;         /**< Indica si la alarma está encendida */
    void * alarm_handler;       /**< Puntero a la función de callback de la alarma */
};

/* === Private function declarations =========================================================== */

/**
 * @brief Convierte la hora en formato BCD a segundos totales.
 * @param time Hora a convertir
 * @return Segundos
 */
static uint32_t TimeToSeconds(const hora_t time);

/**
 * @brief Convierte los segundos totales a formato BCD.
 * @param seconds Segundos a convertir
 * @param time Hora a convertir
 */
static void SecondsToTime(uint32_t seconds, hora_t time);

/* === Private variable definitions ============================================================ */

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

static uint32_t TimeToSeconds(const hora_t time) {
    uint32_t horas = (time[0] * 10) + time[1];
    uint32_t minutos = (time[2] * 10) + time[3];
    uint32_t segundos = (time[4] * 10) + time[5];
    return (horas * 3600) + (minutos * 60) + segundos;
}

static void SecondsToTime(uint32_t seconds, hora_t time) {
    uint32_t horas = seconds / 3600;
    uint32_t resto = seconds % 3600;
    uint32_t minutos = resto / 60;
    uint32_t segundos = resto % 60;

    time[0] = horas / 10;
    time[1] = horas % 10;
    time[2] = minutos / 10;
    time[3] = minutos % 10;
    time[4] = segundos / 10;
    time[5] = segundos % 10;
}
/* === Public function implementation ========================================================== */

clock_t RelojCreate(unsigned int ticks_por_second, void * alarm_handler) {
    static struct clock_s instance = {0};
    clock_t self = &instance;

    self->ticks_por_second = ticks_por_second;
    self->ticks_count = 0; 
    self->current_time = 0;
    self->time_is_valid = false;
    self->alarm_handler = alarm_handler;
    
    self->alarm_time = 0;

    self->alarm_enabled = false;
    return self;
}

bool RelojGetCurrentTime(clock_t self, hora_t current_time) {
    SecondsToTime(self->current_time, current_time);
    return self->time_is_valid;
}

bool RelojSetupCurrentTime(clock_t self, const hora_t current_time) {
    self->current_time = TimeToSeconds(current_time);
    self->time_is_valid = true;
    return true;
}

void RelojNewTick(clock_t self) {
    self->ticks_count++;

    if (self->ticks_count < self->ticks_por_second)
    {
        return;
    }

    self->ticks_count = 0;
    self->current_time++;

    if (self->current_time >= SECONDS_PER_DAY)
    {
        self->current_time = 0;
    }
    
    if (self->alarm_enabled && (self->current_time == self->alarm_time))
    {
        if (self->alarm_handler)
        {
            void (*handler)(void) = self->alarm_handler;
            handler();
        }
        
    }
    
}

void RelojSetupAlarm(clock_t self, const hora_t alarm_time) {
    self->alarm_time = TimeToSeconds(alarm_time);
}

bool RelojGetAlarm(clock_t self, hora_t alarm_time) {
    SecondsToTime(self->alarm_time, alarm_time);
    return self->alarm_enabled;
}

bool RelojToggleAlarm(clock_t self) {
    self->alarm_enabled = !self->alarm_enabled;
    return self->alarm_enabled;
}

void RelojSnoozeAlarm(clock_t self, uint8_t minutos_demora) {
    self->alarm_time += (minutos_demora * 60);
    if (self->alarm_time >= SECONDS_PER_DAY)
    {
        self->alarm_time -= SECONDS_PER_DAY;
    }
    

}
/* === End of documentation ==================================================================== */