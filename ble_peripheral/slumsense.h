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

#include "nrf_error.h"

#ifndef SLUM_SLUMSENSE_H__
#define SLUM_SLUMSENSE_H__

// TODO: maybe we should just use floats?
struct env_data_s {
    uint16_t temperature;   // temperature x10 (1 decimal), degC
    uint16_t pressure;      // pressure x10 (1 decimal), hPa
    uint16_t humidity;      // relative humidity x10 (1 decimal), %
};

typedef struct env_data_s env_data_t;

#endif // SLUM_SENSOR_H__

/** @} */