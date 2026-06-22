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

/** @file test_reloj.c
 ** @brief Pruebas unitarias para la biblioteca del reloj.
 **/

/* === Headers files inclusions ================================================================ */

#include "unity.h" 
#include "reloj.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private function declarations =========================================================== */

/* === Private variable definitions ============================================================ */

static const hora_t DEFAUL_TIME = {0,0,0,0,0,0};
//static const hora_t INITIAL_TIME = {2,3,5,9,5,9};
static const hora_t INITIAL_TIME = {1,2,3,4,5,6};

/** @brief Variable global para comprobar si el reloj llamó al callback de la alarma */
static bool alarma_sono;

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/**
 * @brief Función simuladora (Callback) que pasaremos al reloj.
 * Cambia el estado de la variable global para confirmar que la alarma se disparó.
 */
void SimuladorAlarma(void) {
    alarma_sono = true;
}

/* === Public function implementation ========================================================== */

/**
 * @brief Prueba que al inicializar el reloj, la hora es 00:00:00 y es inválida.
 * También prueba que la función de creación no devuelve un puntero nulo.
 */
void test_reloj_inicia_invalido(void) {
    clock_t reloj;
    hora_t hora_actual = {1, 2, 3, 4, 5, 6};
    bool es_valida = true;

    reloj = RelojCreate(1, NULL);
    
    TEST_ASSERT_FALSE(RelojGetCurrentTime(reloj, hora_actual));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(DEFAUL_TIME, hora_actual, 6);
}

/**
 * @brief Prueba que al ajustar la hora, el reloj la guarda correctamente y la marca como válida.
 */
void test_ajuste_de_hora(void) {
    clock_t reloj;
    hora_t hora_actual;
    

    reloj = RelojCreate(1, NULL);
    TEST_ASSERT_TRUE(RelojSetupCurrentTime(reloj, INITIAL_TIME));
    //la funcion devuelve true porque la hroa es  valida.
    TEST_ASSERT_TRUE(RelojGetCurrentTime(reloj, hora_actual));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(INITIAL_TIME, hora_actual, 6);
}

/**
 * @brief Prueba que después de "n" ciclos de reloj configurados, la hora matemática avanza exactamente un segundo.
 */
void test_avanza_un_segundo(void) {
    clock_t reloj;
    hora_t hora_actual;
    static const hora_t EXPECTED_TIME = {1,2,3,4,5,9};
    
    reloj = RelojCreate(1, NULL);
    (void)RelojSetupCurrentTime(reloj, INITIAL_TIME);
    for (int i = 0; i < 3; i++)
    {
        RelojNewTick(reloj);
    }
    RelojGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED_TIME, hora_actual, 6);
}

/**
 * @brief Prueba que permite fijar la hora de la alarma y consultarla, devolviendo el mismo arreglo BCD.
 */
void test_fijar_y_consultar_hora_alarma(void) {
    clock_t reloj;
    hora_t hora_alarma;

    reloj = RelojCreate(1, NULL);

    //fijamos una alarma y la consultamos
    RelojSetupAlarm(reloj, INITIAL_TIME);
    RelojGetAlarm(reloj, hora_alarma);

    //verificamos si el arreglo guardado sea identico al enviado
    TEST_ASSERT_EQUAL_UINT8_ARRAY(INITIAL_TIME, hora_alarma, 6);
}

/**
 * @brief Prueba que al alternar el estado de la alarma (Toggle), esta se enciende y apaga correctamente.
 */
void test_alternar_estado_alarma(void) {
    clock_t reloj;
    reloj = RelojCreate(1, NULL);

    //al crear el reloj, la alarma debe estar deshabilitada por defecto.
    // el primer toggle deberia encenderla 8devolver true)

    TEST_ASSERT_TRUE(RelojToggleAlarm(reloj));

    // el segundo toggle deberia apagarala (devolver false)
    TEST_ASSERT_FALSE(RelojToggleAlarm(reloj));
}

/**
 * @brief Prueba que la alarma ejecuta el evento (callback) solo cuando está habilitada y la hora coincide.
 */
void test_alarma_suena_cuando_corresponde(void) {
    clock_t reloj;
    hora_t hora_actual = {1, 2, 3, 4, 5, 6};
    hora_t hora_alarma = {1, 2, 3, 4, 5, 7};

    alarma_sono = false;

    reloj = RelojCreate(1, SimuladorAlarma);

    RelojSetupCurrentTime(reloj, hora_actual);
    RelojSetupAlarm(reloj, hora_alarma);
    RelojToggleAlarm(reloj);

    RelojNewTick(reloj);

    TEST_ASSERT_TRUE(alarma_sono);
}

/**
 * @brief Prueba que al posponer la alarma una cantidad de minutos, la hora se actualiza correctamente.
 */
void test_posponer_alarma(void) {
    clock_t reloj;
    hora_t hora_alarma = {1, 2, 3, 4, 0, 0};
    hora_t hora_propuesta;
    hora_t hora_esperada = {1, 2, 3, 9, 0, 0};

    reloj = RelojCreate(1, NULL);
    RelojSetupAlarm(reloj, hora_alarma);

    RelojSnoozeAlarm(reloj, 5);
    RelojGetAlarm(reloj, hora_propuesta);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_esperada, hora_propuesta, 6);

}


/* === End of documentation ==================================================================== */