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
 * @file      driver_max44009.h
 * @brief     driver max44009 header file
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

#ifndef DRIVER_MAX44009_H
#define DRIVER_MAX44009_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup max44009_driver max44009 driver function
 * @brief    max44009 driver modules
 * @{
 */

/**
 * @addtogroup max44009_basic_driver
 * @{
 */

/**
 * @brief max44009 address enumeration definition
 */
typedef enum
{
    MAX44009_ADDRESS_0 = 0x94,        /**< addr low */
    MAX44009_ADDRESS_1 = 0x96,        /**< addr high */
} max44009_address_t;

/**
 * @brief max44009 bool enumeration definition
 */
typedef enum
{
    MAX44009_BOOL_FALSE = 0x00,        /**< false */
    MAX44009_BOOL_TRUE  = 0x01,        /**< true */
} max44009_bool_t;

/**
 * @brief max44009 integration time enumeration definition
 */
typedef enum
{
    MAX44009_INTEGRATION_TIME_800MS  = 0x00,        /**< 800ms, this is a preferred mode for boosting low-light sensitivity */
    MAX44009_INTEGRATION_TIME_400MS  = 0x01,        /**< 400ms */
    MAX44009_INTEGRATION_TIME_200MS  = 0x02,        /**< 200ms */
    MAX44009_INTEGRATION_TIME_100MS  = 0x03,        /**< 100ms, this is a preferred mode for high-brightness applications */
    MAX44009_INTEGRATION_TIME_50MS   = 0x04,        /**< 50ms, manual mode only */
    MAX44009_INTEGRATION_TIME_25MS   = 0x05,        /**< 25ms, manual mode only */
    MAX44009_INTEGRATION_TIME_12P5MS = 0x06,        /**< 12.5ms, manual mode only */
    MAX44009_INTEGRATION_TIME_6P25MS = 0x07,        /**< 6.25ms, manual mode only */
} max44009_integration_time_t;

/**
 * @brief max44009 handle structure definition
 */
typedef struct max44009_handle_s
{
    uint8_t iic_addr;                                                                  /**< iic device address */
    uint8_t (*iic_init)(void);                                                         /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                       /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);       /**< point to an iic_write function address */
    void (*delay_ms)(uint32_t ms);                                                     /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                   /**< point to a debug_print function address */
    uint8_t inited;                                                                    /**< inited flag */
} max44009_handle_t;

/**
 * @brief max44009 information structure definition
 */
typedef struct max44009_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} max44009_info_t;

/**
 * @}
 */

/**
 * @defgroup max44009_link_driver max44009 link driver function
 * @brief    max44009 link driver modules
 * @ingroup  max44009_driver
 * @{
 */

/**
 * @brief     initialize max44009_handle_t structure
 * @param[in] HANDLE pointer to a max44009 handle structure
 * @param[in] STRUCTURE max44009_handle_t
 * @note      none
 */
#define DRIVER_MAX44009_LINK_INIT(HANDLE, STRUCTURE)               memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to a max44009 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_MAX44009_LINK_IIC_INIT(HANDLE, FUC)                 (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to a max44009 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_MAX44009_LINK_IIC_DEINIT(HANDLE, FUC)               (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to a max44009 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_MAX44009_LINK_IIC_READ(HANDLE, FUC)                 (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to a max44009 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_MAX44009_LINK_IIC_WRITE(HANDLE, FUC)                (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a max44009 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_MAX44009_LINK_DELAY_MS(HANDLE, FUC)                 (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a max44009 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_MAX44009_LINK_DEBUG_PRINT(HANDLE, FUC)              (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup max44009_basic_driver max44009 basic driver function
 * @brief    max44009 basic driver modules
 * @ingroup  max44009_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a max44009 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max44009_info(max44009_info_t *info);

/**
 * @brief     set the address pin
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] addr_pin chip address pins
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t max44009_set_addr_pin(max44009_handle_t *handle, max44009_address_t addr_pin);

/**
 * @brief      get the address pin
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *addr_pin pointer to a chip address pins buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max44009_get_addr_pin(max44009_handle_t *handle, max44009_address_t *addr_pin);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a max44009 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t max44009_init(max44009_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a max44009 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 set low power mode failed
 * @note      none
 */
uint8_t max44009_deinit(max44009_handle_t *handle);

/**
 * @brief      read data
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *raw pointer to a raw lux buffer
 * @param[out] *lux pointer to a converted lux buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_read(max44009_handle_t *handle, uint16_t *raw, float *lux);

/**
 * @brief      get interrupt status
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_interrupt_status(max44009_handle_t *handle, max44009_bool_t *enable);

/**
 * @brief     enable or disable interrupt
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_interrupt(max44009_handle_t *handle, max44009_bool_t enable);

/**
 * @brief      get interrupt status
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_interrupt(max44009_handle_t *handle, max44009_bool_t *enable);

/**
 * @brief     enable or disable continuous mode
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set continuous mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_continuous_mode(max44009_handle_t *handle, max44009_bool_t enable);

/**
 * @brief      get continuous mode stauts
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get continuous mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_continuous_mode(max44009_handle_t *handle, max44009_bool_t *enable);

/**
 * @brief     enable or disable auto range
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_auto_range(max44009_handle_t *handle, max44009_bool_t enable);

/**
 * @brief      get auto range
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto range failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_auto_range(max44009_handle_t *handle, max44009_bool_t *enable);

/**
 * @brief     enable or disable current div 8
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set current div 8 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_current_div_8(max44009_handle_t *handle, max44009_bool_t enable);

/**
 * @brief      get current div 8
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get current div 8 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_current_div_8(max44009_handle_t *handle, max44009_bool_t *enable);

/**
 * @brief     set integration time
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] t integration time
 * @return    status code
 *            - 0 success
 *            - 1 set integration time failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_integration_time(max44009_handle_t *handle, max44009_integration_time_t t);

/**
 * @brief      get integration time
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *t pointer to an integration time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get integration time failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_integration_time(max44009_handle_t *handle, max44009_integration_time_t *t);

/**
 * @brief     set interrupt high threshold
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] threshold high threshold
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt high threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_interrupt_high_threshold(max44009_handle_t *handle, uint8_t threshold);

/**
 * @brief      get interrupt high threshold
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *threshold pointer to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt high threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_interrupt_high_threshold(max44009_handle_t *handle, uint8_t *threshold);

/**
 * @brief     set interrupt low threshold
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] threshold low threshold
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt low threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_interrupt_low_threshold(max44009_handle_t *handle, uint8_t threshold);

/**
 * @brief      get interrupt low threshold
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *threshold pointer to a threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt low threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_interrupt_low_threshold(max44009_handle_t *handle, uint8_t *threshold);

/**
 * @brief     set threshold timer
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] t timer time
 * @return    status code
 *            - 0 success
 *            - 1 set threshold timer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_threshold_timer(max44009_handle_t *handle, uint8_t t);

/**
 * @brief      get threshold timer
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *t pointer to a timer time buffer
 * @return     status code
 *             - 0 success
 *             - 1 get threshold timer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_threshold_timer(max44009_handle_t *handle, uint8_t *t);

/**
 * @brief      convert the threshold to the register raw data
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[in]  lux input lux
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_threshold_convert_to_register(max44009_handle_t *handle, float lux, uint8_t *reg);

/**
 * @brief      convert the register raw data to the threshold
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[in]  reg register raw data
 * @param[out] *lux pointer to a lux buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_threshold_convert_to_data(max44009_handle_t *handle, uint8_t reg, float *lux);

/**
 * @brief      convert the timer to the register raw data
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[in]  ms input time in ms
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_timer_convert_to_register(max44009_handle_t *handle, float ms, uint8_t *reg);

/**
 * @brief      convert the register raw data to timer
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[in]  reg register raw data
 * @param[out] *ms pointer to a time buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_timer_convert_to_data(max44009_handle_t *handle, uint8_t reg, float *ms);

/**
 * @}
 */

/**
 * @defgroup max44009_extern_driver max44009 extern driver function
 * @brief    max44009 extern driver modules
 * @ingroup  max44009_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max44009_set_reg(max44009_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max44009_get_reg(max44009_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
