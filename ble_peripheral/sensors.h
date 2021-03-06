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

#include "slumsense.h"
#include "nrf_gpio.h"

/* Pin configurations for Thingy91_nRF52840 */
#define THINGY91_SDA_PIN                    NRF_GPIO_PIN_MAP(1, 8)              /**< Used for TWI */
#define THINGY91_SCL_PIN                    NRF_GPIO_PIN_MAP(1, 9)              /**< Used for TWI */

/* Debugging */
#define SENSORS_LOG_TWI_TRAFFIC             0                                   /**< If set to 1, TWI writes and reads are output to NRF_LOG. */
#define SENSORS_LOG_DATA                    0                                   /**< If set to 1, sensor data is output to NRF_LOG. */

/* Public interface */
void sensors_init(void);
void sensors_configure(void);
ret_code_t sensors_read(env_data_t *env_data);

#endif // SLUM_SENSOR_H__

/** @} */