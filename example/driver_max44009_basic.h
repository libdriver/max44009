/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
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
 *
 * @file      driver_max44009_basic.h
 * @brief     driver max44009 basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2026-09-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2026/09/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_MAX44009_BASIC_H
#define DRIVER_MAX44009_BASIC_H

#include "driver_max44009_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup max44009_example_driver max44009 example driver function
 * @brief    max44009 example driver modules
 * @ingroup  max44009_driver
 * @{
 */

/**
 * @brief max44009 basic example default definition
 */
#define MAX44009_BASIC_DEFAULT_AUTO_RANGE             MAX44009_BOOL_TRUE                     /**< enable auto range */
#define MAX44009_BASIC_DEFAULT_CONTINUOUS_MODE        MAX44009_BOOL_TRUE                     /**< enable continuous mode */
#define MAX44009_BASIC_DEFAULT_CURRENT_DIV_8          MAX44009_BOOL_FALSE                    /**< disable current div 8 */
#define MAX44009_BASIC_DEFAULT_INTEGRATION_TIME       MAX44009_INTEGRATION_TIME_200MS        /**< 200ms */

/**
 * @brief     basic example init
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max44009_basic_init(max44009_address_t addr_pin);

/**
 * @brief      basic example read
 * @param[out] *lux pointer to a converted lux buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max44009_basic_read(float *lux);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max44009_basic_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
