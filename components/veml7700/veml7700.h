/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Marc Luehr <marcluehr@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file veml7700.h
 * @defgroup veml7700 veml7700
 * @{
 *
 * ESP-IDF driver for VEML7700 brightness sensors for I2C-bus
 *
 * Copyright (c) 2022 Marc Luehr <marcluehr@gmail.com>
 *
 * MIT Licensed as described in the file LICENSE
 */
#ifndef __VEML7700_H__
#define __VEML7700_H__

#include <stddef.h>
#include <i2cdev.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VEML7700_I2C_ADDR 0x10

#define VEML7700_INTEGRATION_TIME_25MS 0b1100
#define VEML7700_INTEGRATION_TIME_50MS 0b1000
#define VEML7700_INTEGRATION_TIME_100MS 0b0000
#define VEML7700_INTEGRATION_TIME_200MS 0b0001
#define VEML7700_INTEGRATION_TIME_400MS 0b0010
#define VEML7700_INTEGRATION_TIME_800MS 0b0011

#define VEML7700_GAIN_1 0b00
#define VEML7700_GAIN_2 0b01
#define VEML7700_GAIN_DIV_8 0b10
#define VEML7700_GAIN_DIV_4 0b11

#define VEML7700_POWER_SAVING_MODE_500MS 0b00
#define VEML7700_POWER_SAVING_MODE_1000MS 0b01
#define VEML7700_POWER_SAVING_MODE_2000MS 0b10
#define VEML7700_POWER_SAVING_MODE_4000MS 0b11

#define VEML7700_PERSISTENCE_PROTECTION_1 0b00
#define VEML7700_PERSISTENCE_PROTECTION_2 0b01
#define VEML7700_PERSISTENCE_PROTECTION_4 0b10
#define VEML7700_PERSISTENCE_PROTECTION_8 0b11

/**
 * VEML configuration descriptor
 */
typedef struct
{
    uint16_t gain : 2; // control the sensitivity
    uint16_t integration_time : 4; // time to measure, more time leads to higher values
    uint16_t persistence_protect : 2; // sample count before the interrupt triggers
    uint16_t interrupt_enable : 1; // enable threshold interrupt
    uint16_t shutdown : 1; // set to 1 to shutdown the device, set to 0 to wakeup
    uint16_t threshold_high; // set the high threshold for the interrupt
    uint16_t threshold_low; // set the low threshold for the interrupt
    uint16_t power_saving_mode : 2;
    uint16_t power_saving_enable : 1; 
} veml7700_config_t;

/**
 * @brief Initialize device descriptor
 *
 * Default SCL frequency is 100kHz. The I2C address is fix.
 *
 * @param dev Pointer to I2C device descriptor
 * @param port I2C port number
 * @param sda_gpio SDA GPIO
 * @param scl_gpio SCL GPIO
 * @return `ESP_OK` on success
 */
esp_err_t veml7700_init_desc(i2c_dev_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);

/**
 * @brief Free device descriptor
 *
 * @param dev Pointer to I2C device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t veml7700_free_desc(i2c_dev_t *dev);

/**
 * @brief Free device descriptor
 *
 * @param dev Pointer to I2C device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t veml7700_probe(i2c_dev_t *dev);

/**
 * @brief Read GPIO port value
 *
 * @param dev Pointer to I2C device descriptor
 * @param val 8-bit GPIO port value
 * @return `ESP_OK` on success
 */
esp_err_t veml7700_config(i2c_dev_t *dev, veml7700_config_t *config);

/**
 * @brief Read GPIO port value
 *
 * @param dev Pointer to I2C device descriptor
 * @param config Pointer to the config descriptor
 * @param val 8-bit GPIO port value
 * @return `ESP_OK` on success
 */
esp_err_t veml7700_ambient_light(i2c_dev_t *dev, veml7700_config_t *config, uint32_t *value_lux);

/**
 * @brief Read GPIO port value
 *
 * @param dev Pointer to I2C device descriptor
 * @param config Pointer to the config descriptor
 * @param val 8-bit GPIO port value
 * @return `ESP_OK` on success
 */
esp_err_t veml7700_white_channel(i2c_dev_t *dev, veml7700_config_t *config, uint32_t *value_lux);

/**
 * @brief Read GPIO port value
 *
 * @param dev Pointer to I2C device descriptor
 * @param val 8-bit GPIO port value
 * @return `ESP_OK` on success
 */
esp_err_t veml7700_interrupt_status(i2c_dev_t *dev, bool *low_threshold, bool *high_threshold);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __VEML7700_H__ */
