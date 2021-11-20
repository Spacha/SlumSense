/**
 * Copyright (c) 2021, Miika Sikala
 *
 * All rights reserved.
 *
 */
/** @file
 *
 * @defgroup slumsense sensor.h
 * @{
 * @ingroup slumsense
 * @brief Sensor-specific part for SlumSense BLE peripheral.
 *
 * This file is something...
 */

#ifndef SLUM_SENSOR_H__
#define SLUM_SENSOR_H__

#include "nrf_gpio.h"
#include "nrfx_twi.h"
#include "bme680.h"

// Pin configurations for Thingy91_nRF52840
#define THINGY91_SDA_PIN                  NRF_GPIO_PIN_MAP(1, 8) // Used for TWI
#define THINGY91_SCL_PIN                  NRF_GPIO_PIN_MAP(1, 9) // Used for TWI

#endif // SLUM_SENSOR_H__

/** @} */