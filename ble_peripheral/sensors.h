/**
 * Copyright (c) 2021, Miika Sikala
 *
 * All rights reserved.
 *
 */
/** @file
 *
 * @defgroup slumsense sensors.h
 * @{
 * @ingroup slumsense
 * @brief Sensor-specific part for SlumSense BLE peripheral.
 *
 * This file is something...
 */

#ifndef SLUM_SENSOR_H__
#define SLUM_SENSOR_H__

#include "nrf_gpio.h"

// Pin configurations for Thingy91_nRF52840
#define THINGY91_SDA_PIN                  NRF_GPIO_PIN_MAP(1, 8)    // Used for TWI
#define THINGY91_SCL_PIN                  NRF_GPIO_PIN_MAP(1, 9)    // Used for TWI

#define SENSORS_I2C_DEBUG 1                                         // If set to 1, I2C writes and reads are output to NRF_LOG.


/**@brief Function for sensor initializations.
 */
void sensors_init(void);

/**@brief Function for sensor configurations.
 */
void sensors_configure(void);

/**@brief Function for sensor initializations.
 */
void sensors_read(uint8_t * data_reg);

#endif // SLUM_SENSOR_H__

/** @} */