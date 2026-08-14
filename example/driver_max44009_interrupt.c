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
 * @file      driver_max44009_interrupt.c
 * @brief     driver max44009 interrupt source file
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

#include "driver_max44009_interrupt.h"

static max44009_handle_t gs_handle;        /**< max44009 handle */

/**
 * @brief     interrupt example init
 * @param[in] addr_pin iic device address
 * @param[in] low_threshold_lux low threshold in lux
 * @param[in] high_threshold_lux high threshold in lux
 * @param[in] time_ms threshold time in ms
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max44009_interrupt_init(max44009_address_t addr_pin, float low_threshold_lux, float high_threshold_lux, float time_ms)
{
    uint8_t res;
    uint8_t reg;
    
    /* link interface function */
    DRIVER_MAX44009_LINK_INIT(&gs_handle, max44009_handle_t);
    DRIVER_MAX44009_LINK_IIC_INIT(&gs_handle, max44009_interface_iic_init);
    DRIVER_MAX44009_LINK_IIC_DEINIT(&gs_handle, max44009_interface_iic_deinit);
    DRIVER_MAX44009_LINK_IIC_READ(&gs_handle, max44009_interface_iic_read);
    DRIVER_MAX44009_LINK_IIC_WRITE(&gs_handle, max44009_interface_iic_write);
    DRIVER_MAX44009_LINK_DELAY_MS(&gs_handle, max44009_interface_delay_ms);
    DRIVER_MAX44009_LINK_DEBUG_PRINT(&gs_handle, max44009_interface_debug_print);
    
    /* set the addr pin */
    res = max44009_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init */
    res = max44009_init(&gs_handle);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: init failed.\n");
       
        return 1;
    }
    
    /* set default auto range */
    res = max44009_set_auto_range(&gs_handle, MAX44009_INTERRUPT_DEFAULT_AUTO_RANGE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set auto range failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default continuous mode */
    res = max44009_set_continuous_mode(&gs_handle, MAX44009_INTERRUPT_DEFAULT_CONTINUOUS_MODE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set continuous mode failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
#if (MAX44009_BASIC_DEFAULT_AUTO_RANGE != MAX44009_BOOL_FALSE)
    /* set default current div 8 */
    res = max44009_set_current_div_8(&gs_handle, MAX44009_INTERRUPT_DEFAULT_CURRENT_DIV_8);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set current div 8 failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default integration time */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTERRUPT_DEFAULT_INTEGRATION_TIME);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
#endif
    
    /* threshold convert to register */
    res = max44009_threshold_convert_to_register(&gs_handle, low_threshold_lux, &reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: threshold convert to register failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt low threshold */
    res = max44009_set_interrupt_low_threshold(&gs_handle, reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set interrupt low threshold failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* threshold convert to register */
    res = max44009_threshold_convert_to_register(&gs_handle, high_threshold_lux, &reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: threshold convert to register failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt high threshold */
    res = max44009_set_interrupt_high_threshold(&gs_handle, reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set interrupt high threshold failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* timer convert to register */
    res = max44009_timer_convert_to_register(&gs_handle, time_ms, &reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: timer convert to register failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set threshold timer */
    res = max44009_set_threshold_timer(&gs_handle, reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set threshold timer failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable interrupt */
    res = max44009_set_interrupt(&gs_handle, MAX44009_BOOL_TRUE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set interrupt failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      interrupt example read
 * @param[out] *lux pointer to a converted lux buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max44009_interrupt_read(float *lux)
{
    uint8_t res;
    uint16_t raw;
    
    /* read */
    res = max44009_read(&gs_handle, &raw, lux);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max44009_interrupt_deinit(void)
{
    uint8_t res;
    
    /* deinit */
    res = max44009_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
