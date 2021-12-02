/**
 * Copyright (c) 2021, Miika Sikala
 *
 * All rights reserved.
 *
 */
/** @file
 *
 * @defgroup slumsense slumsense.h
 * @{
 * @ingroup slumsense
 * @brief SlumSense BLE peripheral
 *
 * This file is something...
 */

#include "app_error.h"

#ifndef SLUM_SLUMSENSE_H__
#define SLUM_SLUMSENSE_H__

// TODO: maybe we should just use floats?
// If we cannot use more than 1 byte, split it to MSB and LSB parts.

// Fixed-point representations, see the comment below
struct env_data_s {
    uint16_t temperature;   // temperature
    uint16_t pressure;      // pressure
    uint16_t humidity;      // relative humidity

    /** Calculations (fixed point):
     *
     * Temperature [-70.00, 70.00+]:
     *  real -> uint16_t: int  = (temp+70)*100
     *  uint16_t -> real: temp = int/100-70
     *
     * Pressure [0.0, 6553.6]:
     *  real -> uint16_t: int  = pres*10
     *  uint16_t -> real: pres = int/10
     *
     * Humidity [0.0, 100.0]:
     *  real -> uint16_t: int  = humi*10
     *  uint16_t -> real: humi = int/10
     */

/*
    float temperature;
    float pressure;
    float humidity;
*/
};

typedef struct env_data_s env_data_t;

#endif // SLUM_SENSOR_H__

/** @} */