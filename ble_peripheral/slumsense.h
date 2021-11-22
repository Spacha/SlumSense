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
struct env_data_s {
    uint8_t temperature;   // temperature x10 (1 decimal), degC
    uint8_t pressure;      // pressure x10 (1 decimal), hPa
    uint8_t humidity;      // relative humidity x10 (1 decimal), %
/*
    float temperature;   // temperature x10 (1 decimal), degC
    float pressure;      // pressure x10 (1 decimal), hPa
    float humidity;      // relative humidity x10 (1 decimal), %
*/
};

typedef struct env_data_s env_data_t;

#endif // SLUM_SENSOR_H__

/** @} */