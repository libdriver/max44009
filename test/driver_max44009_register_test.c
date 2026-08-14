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
 * @file      driver_max44009_register_test.c
 * @brief     driver max44009 register test source file
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

#include "driver_max44009_register_test.h"
#include <stdlib.h>

static max44009_handle_t gs_handle;        /**< max44009 handle */

/**
 * @brief     register test
 * @param[in] addr iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t max44009_register_test(max44009_address_t addr)
{
    uint8_t res;
    uint8_t threshold;
    uint8_t threshold_check;
    uint8_t tim;
    uint8_t tim_check;
    uint8_t reg;
    float lux;
    float lux_check;
    float ms;
    float ms_check;
    max44009_bool_t enable;
    max44009_integration_time_t t;
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
    
    /* start register test */
    max44009_interface_debug_print("max44009: start register test.\n");
    
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
    
    /* max44009_set_interrupt/max44009_get_interrupt test */
    max44009_interface_debug_print("max44009: max44009_set_interrupt/max44009_get_interrupt test.\n");
    
    /* enable */
    res = max44009_set_interrupt(&gs_handle, MAX44009_BOOL_TRUE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set interrupt failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set interrupt enable.\n");
    res = max44009_get_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get interrupt failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check interrupt %s.\n", (enable == MAX44009_BOOL_TRUE) ? "ok" : "error");
    
    /* disable */
    res = max44009_set_interrupt(&gs_handle, MAX44009_BOOL_FALSE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set interrupt failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set interrupt disable.\n");
    res = max44009_get_interrupt(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get interrupt failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check interrupt %s.\n", (enable == MAX44009_BOOL_FALSE) ? "ok" : "error");
    
    /* max44009_set_continuous_mode/max44009_get_continuous_mode test */
    max44009_interface_debug_print("max44009: max44009_set_continuous_mode/max44009_get_continuous_mode test.\n");
    
    /* enable continuous mode */
    res = max44009_set_continuous_mode(&gs_handle, MAX44009_BOOL_TRUE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set continuous mode failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: enable continuous mode.\n");
    res = max44009_get_continuous_mode(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get continuous mode failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check continuous mode %s.\n", (enable == MAX44009_BOOL_TRUE) ? "ok" : "error");
    
    /* disable continuous mode */
    res = max44009_set_continuous_mode(&gs_handle, MAX44009_BOOL_FALSE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set continuous mode failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: disable continuous mode.\n");
    res = max44009_get_continuous_mode(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get continuous mode failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check continuous mode %s.\n", (enable == MAX44009_BOOL_FALSE) ? "ok" : "error");
    
    /* max44009_set_auto_range/max44009_get_auto_range test */
    max44009_interface_debug_print("max44009: max44009_set_auto_range/max44009_get_auto_range test.\n");
    
    /* enable auto range */
    res = max44009_set_auto_range(&gs_handle, MAX44009_BOOL_TRUE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set auto range failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: enable auto range.\n");
    res = max44009_get_auto_range(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get auto range failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check auto range %s.\n", (enable == MAX44009_BOOL_TRUE) ? "ok" : "error");
    
    /* disable auto range */
    res = max44009_set_auto_range(&gs_handle, MAX44009_BOOL_FALSE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set auto range failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: disable auto range.\n");
    res = max44009_get_auto_range(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get auto range failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check auto range %s.\n", (enable == MAX44009_BOOL_FALSE) ? "ok" : "error");
    
    /* max44009_set_current_div_8/max44009_get_current_div_8 test */
    max44009_interface_debug_print("max44009: max44009_set_current_div_8/max44009_get_current_div_8 test.\n");
    
    /* enable current div 8 */
    res = max44009_set_current_div_8(&gs_handle, MAX44009_BOOL_TRUE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set current div 8 failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: enable current div 8.\n");
    res = max44009_get_current_div_8(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get current div 8 failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check current div 8 %s.\n", (enable == MAX44009_BOOL_TRUE) ? "ok" : "error");
    
    /* disable current div 8 */
    res = max44009_set_current_div_8(&gs_handle, MAX44009_BOOL_FALSE);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set current div 8 failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: disable current div 8.\n");
    res = max44009_get_current_div_8(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get current div 8 failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check current div 8 %s.\n", (enable == MAX44009_BOOL_FALSE) ? "ok" : "error");
    
    /* max44009_set_integration_time/max44009_get_integration_time test */
    max44009_interface_debug_print("max44009: max44009_set_integration_time/max44009_get_integration_time test.\n");
    
    /* set integration time 800ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_800MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 800ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_800MS) ? "ok" : "error");
    
    /* set integration time 400ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_400MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 400ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_400MS) ? "ok" : "error");
    
    /* set integration time 200ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_200MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 200ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_200MS) ? "ok" : "error");
    
    /* set integration time 100ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_100MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 100ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_100MS) ? "ok" : "error");
    
    /* set integration time 50ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_50MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 50ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_50MS) ? "ok" : "error");
    
    /* set integration time 25ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_25MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 25ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_25MS) ? "ok" : "error");
    
    /* set integration time 12.5ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_12P5MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 12.5ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_12P5MS) ? "ok" : "error");
    
    /* set integration time 6.25ms */
    res = max44009_set_integration_time(&gs_handle, MAX44009_INTEGRATION_TIME_6P25MS);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set integration time 6.25ms.\n");
    res = max44009_get_integration_time(&gs_handle, &t);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get integration time failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check integration time %s.\n", (t == MAX44009_INTEGRATION_TIME_6P25MS) ? "ok" : "error");
    
    /* max44009_set_interrupt_high_threshold/max44009_get_interrupt_high_threshold test */
    max44009_interface_debug_print("max44009: max44009_set_interrupt_high_threshold/max44009_get_interrupt_high_threshold test.\n");
    
    threshold = rand() % 0xFFFFU;
    res = max44009_set_interrupt_high_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set interrupt high threshold failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set interrupt high threshold 0x%02X.\n", threshold);
    res = max44009_get_interrupt_high_threshold(&gs_handle, &threshold_check);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get interrupt high threshold failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check interrupt high threshold %s.\n", (threshold == threshold_check) ? "ok" : "error");
    
    /* max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test */
    max44009_interface_debug_print("max44009: max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test.\n");
    
    threshold = rand() % 0xFFU;
    res = max44009_set_interrupt_low_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set interrupt low threshold failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set interrupt low threshold 0x%02X.\n", threshold);
    res = max44009_get_interrupt_low_threshold(&gs_handle, &threshold_check);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get interrupt low threshold failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check interrupt low threshold %s.\n", (threshold == threshold_check) ? "ok" : "error");
    
    /* max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test */
    max44009_interface_debug_print("max44009: max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test.\n");
    
    tim = rand() % 0xFFU;
    res = max44009_set_threshold_timer(&gs_handle, tim);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: set threshold timer failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set threshold timer 0x%02X.\n", tim);
    res = max44009_get_threshold_timer(&gs_handle, &tim_check);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get threshold timer failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check threshold timer %s.\n", (tim == tim_check) ? "ok" : "error");
    
    /* max44009_get_interrupt_status test */
    max44009_interface_debug_print("max44009: max44009_get_interrupt_status test.\n");
    
    /* get interrupt status */
    res = max44009_get_interrupt_status(&gs_handle, &enable);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: get interrupt status failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: interrupt status is %s.\n", (enable == MAX44009_BOOL_TRUE) ? "true" : "false");
    
    /* max44009_threshold_convert_to_register/max44009_threshold_convert_to_data test */
    max44009_interface_debug_print("max44009: max44009_threshold_convert_to_register/max44009_threshold_convert_to_data test.\n");
    
    lux = (float)(rand() % 10000) / 10.0f;
    res = max44009_threshold_convert_to_register(&gs_handle, lux, &reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: threshold convert to register failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set threshold %0.1f.\n", lux);
    res = max44009_threshold_convert_to_data(&gs_handle, reg, &lux_check);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: threshold convert to data failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check threshold %0.1f.\n", lux_check);
    
    /* max44009_timer_convert_to_register/max44009_timer_convert_to_data test */
    max44009_interface_debug_print("max44009: max44009_timer_convert_to_register/max44009_timer_convert_to_data test.\n");
    
    ms = (float)(rand() % 10000);
    res = max44009_timer_convert_to_register(&gs_handle, ms, &reg);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: timer convert to register failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: set timer %0.0fms.\n", ms);
    res = max44009_timer_convert_to_data(&gs_handle, reg, &ms_check);
    if (res != 0)
    {
        max44009_interface_debug_print("max44009: timer convert to data failed.\n");
        (void)max44009_deinit(&gs_handle);
        
        return 1;
    }
    max44009_interface_debug_print("max44009: check timer %0.0f.\n", ms_check);
    
    /* finish register test */
    max44009_interface_debug_print("max44009: finish register test.\n");
    (void)max44009_deinit(&gs_handle);
    
    return 0;
}
