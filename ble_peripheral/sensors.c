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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrfx_twi.h"
#include "nrf_delay.h"

#include "slumsense.h"
#include "sensors.h"
#include "bme680_defs.h"
#include "bme680.h"


/* Definitions */
#define TWI_INSTANCE_ID 0                                                       /**< TWI instance ID. */

/* Declarations */
static void twi_init(void);
static void twi_handler(nrfx_twi_evt_t const *p_event, void *p_context);
void user_delay_ms(uint32_t period);
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

/* Initializations */
static volatile bool m_xfer_done = false;                                       /**< Indicates if operation on TWI has ended. */
static const nrfx_twi_t m_twi = NRFX_TWI_INSTANCE(TWI_INSTANCE_ID);             /**< TWI instance. */
struct bme680_dev m_env_sensor;                                                 /**< Configurations for bme680. */
static uint8_t m_sample;                                                        /**< Buffer for samples read from temperature sensor. */


/******************************************************************************
 *  PUBLIC FUNCTIONS
 *****************************************************************************/

/**@brief Function for sensor initializations.
 */
void sensors_init(void)
{
    // init TWI (maybe should control from main?)
    twi_init();

    /* Init BME680 */
    m_env_sensor.dev_id   = BME680_I2C_ADDR_PRIMARY;
    m_env_sensor.intf     = BME680_I2C_INTF;
    m_env_sensor.read     = user_i2c_read;
    m_env_sensor.write    = user_i2c_write;
    m_env_sensor.delay_ms = user_delay_ms;
    /* amb_temp can be set to 25 prior to configuring the gas sensor 
     * or by performing a few temperature readings without operating the gas sensor.
     */
    m_env_sensor.amb_temp = 25;

    int8_t rslt = BME680_OK;
    rslt = bme680_init(&m_env_sensor);
    ASSERT(BME680_OK == NRF_SUCCESS);
    APP_ERROR_CHECK(rslt); // assumes that BME680_OK == NRF_SUCCESS

    // m_env_sensor.read(BME680_I2C_ADDR_PRIMARY, 0xD0, &m_sample, sizeof(m_sample));
    // NRF_LOG_INFO("Initialized BME680, chip ID: 0x%x", m_sample);
    NRF_LOG_INFO("Initialized BME680.");
}


/**@brief Function for sensor configurations.
 */
void sensors_configure(void)
{
    // TODO:    This is BME-specific, hw should we do this?
    //          Pass struct containing the settings we want?
    
    int8_t rslt = BME680_OK;
    uint8_t set_required_settings;

    /* Set the temperature, pressure and humidity settings */
    m_env_sensor.tph_sett.os_hum  = BME680_OS_2X;
    m_env_sensor.tph_sett.os_pres = BME680_OS_4X;
    m_env_sensor.tph_sett.os_temp = BME680_OS_8X;
    m_env_sensor.tph_sett.filter  = BME680_FILTER_SIZE_3;

    /* Set the remaining gas sensor settings and link the heating profile */
    m_env_sensor.gas_sett.run_gas = BME680_DISABLE_GAS_MEAS; // BME680_ENABLE_GAS_MEAS;
    /* Create a ramp heat waveform in 3 steps */
    // m_env_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
    // m_env_sensor.gas_sett.heatr_dur = 150; /* milliseconds */

    /* Select the power mode */
    /* Must be set before writing the sensor configuration */
    m_env_sensor.power_mode = BME680_FORCED_MODE;

    /* Set the required sensor settings needed */
    set_required_settings = BME680_OST_SEL       | // temp oversampling
                            BME680_OSP_SEL       | // pres oversampling
                            BME680_OSH_SEL       | // humi oversampling
                            BME680_FILTER_SEL    | // filter
                            BME680_GAS_SENSOR_SEL; // all gas sensor related

    /* Set the desired sensor configuration */
    rslt = bme680_set_sensor_settings(set_required_settings, &m_env_sensor);
    ASSERT(BME680_OK == NRF_SUCCESS);
    APP_ERROR_CHECK(rslt); // assumes that BME680_OK == NRF_SUCCESS

    /* Configure oversampling by hand, for some reason */
    uint8_t config[1]    = {0x8C};
    uint8_t reg1[1]      = {BME680_CONF_T_P_MODE_ADDR};
    uint8_t reg2[1]      = {BME680_CONF_OS_H_ADDR};
    //uint8_t regs[1]     = {0x73};

    bme680_set_regs(reg1, config, 1, &m_env_sensor);
    bme680_set_regs(reg2, config, 1, &m_env_sensor);
}


/**@brief Function for reading the environmental sensor data.
 *
 * @details Triggers a measurement in bmr680 and reads the data.
 *
 * @param[out] env_data                 Pointer to the location where the sensor data is to be written.
 *
 * @retval NRF_SUCCESS                  The procedure is successful.
 * @retval NRF_ERROR_INVALID_DATA       The sensor couldn't be read or the data was invalid.
 */
ret_code_t sensors_read(env_data_t *env_data)
{
    int8_t rslt = BME680_OK;

    /* Get the total measurement duration so as to sleep or wait till the
     * measurement is complete */
    uint16_t meas_period;
    bme680_get_profile_dur(&meas_period, &m_env_sensor);

    struct bme680_field_data data;

    /* Trigger the next measurement */
    if (m_env_sensor.power_mode == BME680_FORCED_MODE) {
        rslt = bme680_set_sensor_mode(&m_env_sensor);
    }

    user_delay_ms(meas_period); /* Delay till the measurement is ready */

    rslt = bme680_get_sensor_data(&data, &m_env_sensor);

#if SENSORS_LOG_DATA
    // no decimals
    NRF_LOG_INFO("T: %d degC, P: %d hPa, H: %d %%rH ", data.temperature,
        data.pressure / 100.0f, data.humidity );
    /*
    NRF_LOG_INFO("T: %d degC, P: %d hPa, H: %d %%rH ", data.temperature * 10,
        data.pressure / 100.0f, data.humidity * 10 );
    */

    /* Avoid using measurements from an unstable heating setup */
    if(data.status & BME680_GASM_VALID_MSK)
        NRF_LOG_INFO(", G: %d ohms", data.gas_resistance);
    NRF_LOG_INFO("\r\n");
#endif // SENSORS_LOG_DATA

    // TODO: some data validation (sanity check) and return false if nonsense

    if (rslt != BME680_OK)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    // set sensor data for return
    env_data->temperature   = data.temperature * 10;
    env_data->pressure      = data.pressure / 10.0f;
    env_data->humidity      = data.humidity * 10;

    return NRF_SUCCESS;
}


/******************************************************************************
 *  PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * @brief Function for TWI initialization.
 */
static void twi_init(void)
{
    ret_code_t err_code;

    const nrfx_twi_config_t twi_bme680_config = {
       .scl                = THINGY91_SCL_PIN,
       .sda                = THINGY91_SDA_PIN,
       .frequency          = (nrf_twi_frequency_t)NRFX_TWI_DEFAULT_CONFIG_FREQUENCY,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH, // NRFX_TWI_DEFAULT_CONFIG_IRQ_PRIORITY,
       .hold_bus_uninit    = NRFX_TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT
    };

    // NULL as the handler -> use blocking (synchronous) mode
    err_code = nrfx_twi_init(&m_twi, &twi_bme680_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrfx_twi_enable(&m_twi);
}


/**
 * @brief TWI events handler.
 */
static void twi_handler(nrfx_twi_evt_t const *p_event, void *p_context)
{
    switch (p_event->type)
    {
        case NRFX_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRFX_TWI_XFER_RX)
            {
                // NRF_LOG_INFO("Dataa tuli: %x", m_sample);
            }
            m_xfer_done = true;
            break;
        default:
            break;
    }
}


/* Used and required by bme680 driver. */


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

#if 0
    uint8_t msg[] = {reg_addr, *reg_data}; // construct the message

    // set slave register address & receive from the register
    m_xfer_done = false;
    nrfx_twi_rx(&m_twi, dev_id, msg, len + 1);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false); // wait for ACK
#endif

    // set slave register address
    m_xfer_done = false;
    nrfx_twi_tx(&m_twi, dev_id, &reg_addr, sizeof(reg_addr), false);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false); // wait for ACK

    // receive from the register
    m_xfer_done = false;
    nrfx_twi_rx(&m_twi, dev_id, reg_data, len);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false); // wait for ACK

#if SENSORS_LOG_TWI_TRAFFIC
    NRF_LOG_INFO("Read from 0x%x, %u bytes:", reg_addr, len);
    for(int i = 0; i < len; i++)
    {
        NRF_LOG_INFO("\t0x%x", reg_data[i]);
    }
#endif // SENSORS_LOG_TWI_TRAFFIC

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

#if SENSORS_LOG_TWI_TRAFFIC
    NRF_LOG_INFO("Write to 0x%x, %u bytes:", reg_addr, len);
    for(int i = 0; i < len; i++)
    {
        NRF_LOG_INFO("\t0x%x", reg_data[i]);
    }
#endif // SENSORS_LOG_TWI_TRAFFIC

    ret_code_t err_code = NRF_SUCCESS;
    uint8_t msg[] = {reg_addr, *reg_data}; // construct the message

    // set the register address & write to the register
    m_xfer_done = false;
    nrfx_twi_tx(&m_twi, dev_id, msg, len + 1, false);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false); // wait for ACK

    return 0; // checked for errors already, return 0 for success
}

/**
 * @}
 */