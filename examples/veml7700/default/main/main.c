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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include <driver/i2c.h>

#include <veml7700.h>

static const char *TAG = "VEML7700main";

#define SDA_GPIO_NUM             CONFIG_EXAMPLE_SDA_GPIO_NUM
#define SCL_GPIO_NUM             CONFIG_EXAMPLE_SCL_GPIO_NUM

void app_main(void)
{
    i2c_dev_t veml7700_device;
    veml7700_config_t veml7700_configuration;
    ESP_LOGI(TAG, "initializing hardware");

    // remember to always init i2cdev first before using it
    ESP_ERROR_CHECK(i2cdev_init());

    // initialize the device struct
    ESP_ERROR_CHECK(veml7700_init_desc(&veml7700_device, I2C_NUM_0, SDA_GPIO_NUM, SCL_GPIO_NUM));

#ifdef CONFIG_EXAMPLE_I2C_MASTER_SDA
    veml7700_device.cfg.sda_pullup_en = true;
#endif

#ifdef CONFIG_EXAMPLE_I2C_MASTER_SCL
    veml7700_device.cfg.scl_pullup_en = true;
#endif
    // check if the device is available
    esp_err_t err = veml7700_probe(&veml7700_device);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C probing failed: %d", err);
    }
    
    // set configuration parameters
    // select gain 1/8 coastest resolution but the sensor will most likely not over-saturated
    veml7700_configuration.gain = VEML7700_GAIN_DIV_8;

    // set the time to integrate the light values. more time will lead to a finer resolution
    // but will be over-saturated earlier
    veml7700_configuration.integration_time = VEML7700_INTEGRATION_TIME_100MS;

    // interrupt is not used
    veml7700_configuration.persistence_protect = VEML7700_PERSISTENCE_PROTECTION_4;
    veml7700_configuration.interrupt_enable = 0;
    veml7700_configuration.shutdown = 0;
    
    // set power saving modes. This reduces the repeated measurement. When disabled
    // the sensors performs one measurement cycle after the other (here: 100ms intergration time)
    // setting the power save mode add a 1000ms sleep during measurements.
    veml7700_configuration.power_saving_mode = VEML7700_POWER_SAVING_MODE_1000MS;
    veml7700_configuration.power_saving_enable = 1;
    
    // write the configuration to the device
    ESP_ERROR_CHECK(veml7700_config(&veml7700_device, &veml7700_configuration));
    
    while(1)
    {
        uint32_t als = 0;
        uint32_t white = 0;
        
        veml7700_ambient_light(&veml7700_device, &veml7700_configuration, &als);
        veml7700_white_channel(&veml7700_device, &veml7700_configuration, &white);

        ESP_LOGI(TAG, "ALS: %lu lx", als);
        ESP_LOGI(TAG, "WHITE: %lu lx", white);
        vTaskDelay(pdMS_TO_TICKS(1200)); 
    }

    
}
