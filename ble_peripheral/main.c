/**
 * Copyright (c) 2021, Miika Sikala
 *
 * All rights reserved.
 *
 */
/** @file
 *
 * @defgroup slumsense main.c
 * @{
 * @ingroup slumsense
 * @brief SlumSense BLE peripheral
 *
 * This file is something...
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Common nRF includes
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"

// Other nRF includes
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_timer.h"

// SoftDevice Handlers
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
// #include "fds.h" // flash data storage
// #include "sensorsim.h

// Application includes
#include "slumsense.h"
#include "bluetooth.h"
#include "sensors.h"


APP_TIMER_DEF(m_notification_timer_id);
uint16_t m_test_int = 0;

/* Declarations */
// ...


/******************************************************************************
 *  MISCELLANEOUS FUNCTIONS
 ******************************************************************************

/******************************************************************************
 *  HANDLER FUNCTIONS
 ******************************************************************************

/**@brief Function for handling the measurement timer timeout.
 *
 * @details This function will be called each time the measurement timer expires.
 *
 * @param[in] p_context  Pointer used for passing some arbitrary information (context) from the
 *                       app_start_timer() call to the timeout handler.
 */
static void notification_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    ret_code_t err_code;

    NRF_LOG_INFO("Reading data.");

    m_test_int++;

    // TODO: separate timer for sensor read, timeout e.g. 30 seconds or 2 minutes
    /*

    // maybe buffer is not needed, get only one (latest) value
    #define ENV_DATA_BUFFER_LEN 16

    struct env_data_s {
      float tempCelsius;
      float tempCelsius;
    };
    typedef struct env_data_s env_data_t;
    env_data_t m_data_buffer[ENV_DATA_BUFFER_LEN];
    m_data_buff_i = 0;

    // in env_data_timeout_handler:
    env_data_s env_data;
    read_env_data( &env_data );

    m_data_buffer[m_data_buff_i] = env_data;

    if (m_data_buff_i < ENV_DATA_BUFFER_LEN) {
      m_data_buff_i++;
    } else {
      m_data_buff_i = 0; // wrap to 0
    }

    // in notification_timeout_handler
    // loop the 

    // ............................ OR ..............................

    struct env_data_s {
      float tempCelsius; // could use integre format as well (see BME680 p.17)
      float presPascals;
      float humiPercents;
    };
    typedef struct env_data_s env_data_t;

    // in env_data_timeout_handler (replaces this, only enabled when notifications enabled):

    env_data_s env_data;
    read_env_data( &env_data ); // so no global data buffer needed

    // notify the peer (if there is one)
    err_code = ble_cus_custom_value_update(&m_cus, m_custom_value);
    APP_ERROR_CHECK(err_code);

    // ...
    */

    // Increment the value of m_custom_value before notifing it.
    // m_custom_value = m_sample;  // m_custom_value++;
    
    // err_code = ble_cus_custom_value_update(&m_cus, m_custom_value);
    // APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}


/******************************************************************************
 *  INIT FUNCTIONS
 ******************************************************************************

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing the SoftDevice.
 */
static void sdh_init(void)
{
    ret_code_t err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{
    // Initialize timer module.
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    
    // Create timers.
    err_code = app_timer_create(&m_notification_timer_id, APP_TIMER_MODE_REPEATED, notification_timeout_handler);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting timers.
 */
static void application_timers_start(void)
{
       ret_code_t err_code;
       err_code = app_timer_start(m_notification_timer_id, NOTIFICATION_INTERVAL, NULL);
       APP_ERROR_CHECK(err_code);
}


/**@brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    log_init();
    sdh_init();
    timers_init();
    power_management_init();
    sensors_init();

    NRF_LOG_INFO("Here we go!");

    // Just for fun, start the timer.
    application_timers_start();

    // Enter main loop.
    for (;;)
    {
        idle_state_handle();
    }
}

/**
 * @}
 */
