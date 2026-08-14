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
 * @file      driver_max44009_interrupt_test.c
 * @brief     driver max44009 interrupt test source file
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

#include "driver_max44009_interrupt_test.h"
#include <stdlib.h>

static max44009_handle_t gs_handle;        /**< max44009 handle */

/**
 * @brief     interrupt test
 * @param[in] addr iic device address
 * @param[in] low_threshold_lux low threshold in lux
 * @param[in] high_threshold_lux high threshold in lux
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t max44009_interrupt_test(max44009_address_t addr, float low_threshold_lux, float high_threshold_lux, uint32_t times)
{
    uint8_t res;
    uint8_t reg;
    uint16_t raw;
    float lux;
    uint32_t i;
    max44009_bool_t enable;
    max44009_info_t info;
    
    /* link interface function */
    DRIVER_MAX44009_LINK_INIT(&gs_handle, max44009_handle_t);
    DRIVER_MAX44009_LINK_IIC_INIT(&gs_handle, max44009_interface_iic_init);
    DRIVER_MAX44009_LINK_IIC_DEINIT(&gs_handle, max44009_interface_iic_deinit);
    DRIVER_MAX44009_LINK_IIC_READ(&gs_handle, max44009_interface_iic_read);
    DRIVER_MAX44009_LINK_IIC_WRITE(&gs_handle, max44009_interface_iic_write);
    DRIVER_MAX44009_LINK_DELAY_MS(&gs_handle, max44009_interface_delay_ms);
    DRIVER_MAX44009_LINK_DEBUG_PRINT(&gs_handle, max44009_interface_debug_print);
    
    /* get information */
    res = max44009_info(&info);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        max44009_interface_debug_print("max44009: chip is %s.\n", info.chip_name);
        max44009_interface_debug_print("max44009: manufacturer is %s.\n", info.manufacturer_name);
        max44009_interface_debug_print("max44009: interface is %s.\n", info.interface);
        max44009_interface_debug_print("max44009: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        max44009_interface_debug_print("max44009: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        max44009_interface_debug_print("max44009: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        max44009_interface_debug_print("max44009: max current is %0.3fmA.\n", info.max_current_ma);
        max44009_interface_debug_print("max44009: max temperature is %0.1fC.\n", info.temperature_max);
        max44009_interface_debug_print("max44009: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start interrupt test */
    max44009_interface_debug_print("max44009: start interrupt test.\n");
    
    /* set addr pin */
    res = max44009_set_addr_pin(&gs_handle, addr);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set addr pin test.\n");
        
        return 1;
    }
    
    /* init */
    res = max44009_init(&gs_handle);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: init failed.\n");
        
        return 1;
    }
    
    /* enable auto range */
    res = max44009_set_auto_range(&gs_handle, MAX44009_BOOL_TRUE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set auto range failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable continuous mode */
    res = max44009_set_continuous_mode(&gs_handle, MAX44009_BOOL_TRUE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set continuous mode failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    
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
    res = max44009_timer_convert_to_register(&gs_handle, 500.0f, &reg);
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
    
    for (i = 0; i < times; i++)
    {
        res = max44009_read(&gs_handle, &raw, &lux);
        if (res != 0)
        {
            max44009_interface_debug_print("max44009: read failed.\n");
            (void)max44009_deinit(&gs_handle);
            
            return 1;
        }
        
        /* get interrupt status */
        res = max44009_get_interrupt_status(&gs_handle, &enable);
        if (res != 0)
        {
            max44009_interface_debug_print("max44009: get interrupt status failed.\n");
            (void)max44009_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        max44009_interface_debug_print("max44009: data is %0.2flux, interrupt status is %s.\n", lux, (enable == MAX44009_BOOL_TRUE) ? "enable" : "disable");
        
        if (lux < low_threshold_lux)
        {
            /* output */
            max44009_interface_debug_print("max44009: low threshold %0.2flux and please check int pin.\n", lux);
            
            if (enable == MAX44009_BOOL_TRUE)
            {
                break;
            }
        }
        if (lux > high_threshold_lux)
        {
            /* output */
            max44009_interface_debug_print("max44009: high threshold %0.2flux and please check int pin.\n", lux);
            
            if (enable == MAX44009_BOOL_TRUE)
            {
                break;
            }
        }
        
        /* delay 1000ms */
        max44009_interface_delay_ms(1000);
    }
    
    /* finish interrupt test */
    max44009_interface_debug_print("max44009: finish interrupt test.\n");
    (void)max44009_deinit(&gs_handle);
    
    return 0;
}
