/**
 * Copyright (c) 2021, Miika Sikala
 *
 * All rights reserved.
 *
 */
/** @file
 *
 * @defgroup slumsense sensors.c
 * @{
 * @ingroup slumsense
 * @brief Sensor-specific part for SlumSense BLE peripheral.
 *
 * This file is something...
 */
#include "nordic_common.h"
#include "nrf.h"

#include "slumsense.h"
#include "sensors.h"
#include "nrfx_twi.h"
#include "nrf_delay.h"
#include "app_error.h"
#include "bme680.h"

#define TWI_INSTANCE_ID                                                                     // TWI instance ID.

/* Declarations */
void twi_init(void);
void user_delay_ms(uint32_t period);
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

/* Initializations */
//static volatile bool m_xfer_done = false;                                                   // Indicates if operation on TWI has ended.
static const nrfx_twi_t m_twi = NRFX_TWI_INSTANCE(TWI_INSTANCE_ID);                         // TWI instance.
struct bme680_dev env_sensor;                                                               // Configurations for bme680.
//static uint8_t m_sample;                                                                    // Buffer for samples read from temperature sensor.


void sensors_init(void)
{
    
    /* Init BME680 */

    env_sensor.dev_id   = BME680_I2C_ADDR_PRIMARY;
    env_sensor.intf     = BME680_I2C_INTF;
    env_sensor.read     = user_i2c_read;
    env_sensor.write    = user_i2c_write;
    env_sensor.delay_ms = user_delay_ms;
    /* amb_temp can be set to 25 prior to configuring the gas sensor 
     * or by performing a few temperature readings without operating the gas sensor.
     */
    env_sensor.amb_temp = 25;

    int8_t rslt = BME680_OK;
    rslt = bme680_init(&env_sensor);
    ASSERT(BME680_OK == NRF_SUCCESS);
    APP_ERROR_CHECK(rslt); // assumes that BME680_OK == NRF_SUCCESS

    twi_init();
}


void sensors_configure(void)
{
    // TODO:    This is BME-specific, hw should we do this?
    //          Pass struct containing the settings we want?
    
    uint8_t set_required_settings;

    /* Set the temperature, pressure and humidity settings */
    gas_sensor.tph_sett.os_hum  = BME680_OS_2X;
    gas_sensor.tph_sett.os_pres = BME680_OS_4X;
    gas_sensor.tph_sett.os_temp = BME680_OS_8X;
    gas_sensor.tph_sett.filter  = BME680_FILTER_SIZE_3;

    /* Set the remaining gas sensor settings and link the heating profile */
    gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    /* Create a ramp heat waveform in 3 steps */
    gas_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
    gas_sensor.gas_sett.heatr_dur = 150; /* milliseconds */

    /* Select the power mode */
    /* Must be set before writing the sensor configuration */
    gas_sensor.power_mode = BME680_FORCED_MODE; 

    /* Set the required sensor settings needed */
    set_required_settings = BME680_OST_SEL |
                            BME680_OSP_SEL |
                            BME680_OSH_SEL |
                            BME680_FILTER_SEL |
                            BME680_GAS_SENSOR_SEL;

    /* Set the desired sensor configuration */
    rslt = bme680_set_sensor_settings(set_required_settings, &env_sensor);
    ASSERT(BME680_OK == NRF_SUCCESS);
    APP_ERROR_CHECK(rslt); // assumes that BME680_OK == NRF_SUCCESS

    /* Set the power mode */
    rslt = bme680_set_sensor_mode(&gas_sensor);
    APP_ERROR_CHECK(rslt); // assumes that BME680_OK == NRF_SUCCESS
}


/**
 * @brief TWI events handler.
 */
/*
void twi_handler(nrfx_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRFX_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRFX_TWI_XFER_RX)
            {
                sensor_data_handler(m_sample);
            }
            m_xfer_done = true;
            break;
        default:
            break;
    }
}
*/


/**
 * @brief Function for TWI initialization.
 */
void twi_init(void)
{
    ret_code_t err_code;

    const nrfx_twi_config_t twi_bme680_config = {
       .scl                = THINGY91_SCL_PIN,
       .sda                = THINGY91_SDA_PIN,
       .frequency          = (nrf_twi_frequency_t)NRFX_TWI_DEFAULT_CONFIG_FREQUENCY,
       .interrupt_priority = NRFX_TWI_DEFAULT_CONFIG_IRQ_PRIORITY,
       .hold_bus_uninit    = NRFX_TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT
    };

    // NULL as the handler -> use blocking (synchronous) mode
    err_code = nrfx_twi_init(&m_twi, &twi_bme680_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrfx_twi_enable(&m_twi);
}


/**@brief Wait for a period of time.
 *
 * @details A wrapper for nRF's delay function, for BME680 lib to use.
 *
 * @param[in]  period       Delay in milliseconds.
 */
void user_delay_ms(uint32_t period)
{
    /*
     * Return control or wait,
     * for a period amount of milliseconds
     */
     nrf_delay_ms(period);
}


/**@brief Reading from the I2C bus.
 *
 * @details Performs a TX-RX...RX sequence, where the TX section sets th register address.
 *
 * @param[in]  dev_id       I2C address of the targte device.
 * @param[in]  reg_addr     Address of the target register
 * @param[out] reg_data     Pointer to the data to read to.
 * @param[in]  len          Length of the data to be read.
 */
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    ret_code_t err_code = NRF_SUCCESS;

    // TXRX: write tx1 and tx2 without STOP in between
    // FIXME: TXRX does NOT have stop in between!
    nrfx_twi_xfer_desc_t xfer = NRFX_TWI_XFER_DESC_TXRX	(
        BME680_I2C_ADDR_PRIMARY,  // slave address
        &reg_addr,                // tx buffer
        sizeof(reg_addr),         // tx buffer length
        reg_data,                 // rx buffer
        len                       // rx buffer length
    );

    //err_code = nrfx_twi_tx(&m_twi, BME680_ADDR, &id_reg, sizeof(id_reg), true);
    //APP_ERROR_CHECK(err_code);
    err_code = nrfx_twi_xfer(&m_twi, &xfer, NULL);
    APP_ERROR_CHECK(err_code);

    return 0; // checked for errors already, return 0 for success
}


/**@brief Writing to the I2C bus.
 *
 * @details
 *
 * @param[in] dev_id        I2C address of the targte device.
 * @param[in] reg_addr      Address of the target register
 * @param[in] reg_data      Pointer to the data to write.
 * @param[in] len           Length of the data to be written.
 */
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    ret_code_t err_code = NRF_SUCCESS;

    // TXTX: write tx1 and tx2 without STOP in between
    nrfx_twi_xfer_desc_t xfer = NRFX_TWI_XFER_DESC_TXTX(
        BME680_I2C_ADDR_PRIMARY,  // slave address
        &reg_addr,                // tx1 buffer
        sizeof(reg_addr),         // tx1 buffer length
        reg_data,                 // tx2 buffer
        len                      // tx2 buffer length
    )

    //err_code = nrfx_twi_tx(&m_twi, BME680_ADDR, &id_reg, sizeof(id_reg), true);
    //APP_ERROR_CHECK(err_code);
    err_code = nrfx_twi_xfer(&m_twi, &xfer, NULL);
    APP_ERROR_CHECK(err_code);

    return 0; // checked for errors already, return 0 for success
}

/**
 * @}
 */
