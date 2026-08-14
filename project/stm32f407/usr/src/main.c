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
 * @file      main.c
 * @brief     main source file
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
#include "driver_max44009_read_test.h"
#include "driver_max44009_interrupt_test.h"
#include "driver_max44009_basic.h"
#include "driver_max44009_interrupt.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */

/**
 * @brief     max44009 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t max44009(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"high", required_argument, NULL, 2},
        {"low", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    float low_threshold = 100.0f;
    float high_threshold = 1000.0f;
    max44009_address_t addr = MAX44009_ADDRESS_0;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr */
                if (strcmp("0", optarg) == 0)
                {
                    addr = MAX44009_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = MAX44009_ADDRESS_1;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* high */
            case 2 :
            {
                /* set the high threshold */
                high_threshold = (float)atof(optarg);
                
                break;
            }
            
            /* low */
            case 3 :
            {
                /* set the low threshold */
                low_threshold = (float)atof(optarg);
                
                break;
            }
            
            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);
    
    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run register test */
        if (max44009_register_test(addr) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (max44009_read_test(addr, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        /* run interrupt test */
        if (max44009_interrupt_test(addr, low_threshold, high_threshold, times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* basic init */
        res = max44009_basic_init(addr);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 1000ms */
        max44009_interface_delay_ms(1000);
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            float lux;
            
            /* basic read */
            res = max44009_basic_read(&lux);
            if (res != 0)
            {
                (void)max44009_basic_deinit();
                
                return 1;
            }
            
            /* output */
            max44009_interface_debug_print("%d/%d.\n", i + 1, times);
            max44009_interface_debug_print("%.2flux.\n", lux);
            
            /* delay 1000ms */
            max44009_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)max44009_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* output */
        max44009_interface_debug_print("max44009: low threshold is %0.2f lux.\n", low_threshold);
        max44009_interface_debug_print("max44009: high threshold is %0.2f lux.\n", high_threshold);
        
        /* interrupt init */
        res = max44009_interrupt_init(addr, low_threshold, high_threshold, 500.0f);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            float lux;
            
            /* interrupt read */
            res = max44009_interrupt_read(&lux);
            if (res != 0)
            {
                (void)max44009_interrupt_deinit();
                
                return 1;
            }
            
            /* output */
            max44009_interface_debug_print("%d/%d.\n", i + 1, times);
            max44009_interface_debug_print("%.2flux.\n", lux);
            
            /* check the result */
            if (lux < low_threshold)
            {
                /* output */
                max44009_interface_debug_print("max44009: low threshold %0.2flux and please check int pin.\n", lux);
                
                break;
            }
            if (lux > high_threshold)
            {
                /* output */
                max44009_interface_debug_print("max44009: high threshold %0.2flux and please check int pin.\n", lux);
                
                break;
            }
            
            /* delay 1000ms */
            max44009_interface_delay_ms(1000);
        }
        
        /* deinit */
        max44009_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        max44009_interface_debug_print("Usage:\n");
        max44009_interface_debug_print("  max44009 (-i | --information)\n");
        max44009_interface_debug_print("  max44009 (-h | --help)\n");
        max44009_interface_debug_print("  max44009 (-p | --port)\n");
        max44009_interface_debug_print("  max44009 (-t reg | --test=reg) [--addr=<0 | 1>]\n");
        max44009_interface_debug_print("  max44009 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]\n");
        max44009_interface_debug_print("  max44009 (-t int | --test=int) [--addr=<0 | 1>] [--low=<lux>] [--high=<lux>] [--times=<num>]\n");
        max44009_interface_debug_print("  max44009 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]\n");
        max44009_interface_debug_print("  max44009 (-e int | --example=int) [--addr=<0 | 1>] [--low=<lux>] [--high=<lux>] [--times=<num>]\n");
        max44009_interface_debug_print("\n");
        max44009_interface_debug_print("Options:\n");
        max44009_interface_debug_print("      --addr=<0 | 1>              Set the chip iic address.([default: 0])\n");
        max44009_interface_debug_print("  -e <read | int>, --example=<read | int>\n");
        max44009_interface_debug_print("                                  Run the driver example.\n");
        max44009_interface_debug_print("  -h, --help                      Show the help.\n");
        max44009_interface_debug_print("      --high=<lux>                Set the high threshold in lux.([default: 1000.0])\n");
        max44009_interface_debug_print("      --low=<lux>                 Set the low threshold in lux.([default: 100.0])\n");
        max44009_interface_debug_print("  -i, --information               Show the chip information.\n");
        max44009_interface_debug_print("  -p, --port                      Display the pin connections of the current board.\n");
        max44009_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        max44009_interface_debug_print("                                  Run the driver test.\n");
        max44009_interface_debug_print("      --times=<num>               Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        max44009_info_t info;
        
        /* print max44009 info */
        max44009_info(&info);
        max44009_interface_debug_print("max44009: chip is %s.\n", info.chip_name);
        max44009_interface_debug_print("max44009: manufacturer is %s.\n", info.manufacturer_name);
        max44009_interface_debug_print("max44009: interface is %s.\n", info.interface);
        max44009_interface_debug_print("max44009: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        max44009_interface_debug_print("max44009: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        max44009_interface_debug_print("max44009: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        max44009_interface_debug_print("max44009: max current is %0.2fmA.\n", info.max_current_ma);
        max44009_interface_debug_print("max44009: max temperature is %0.1fC.\n", info.temperature_max);
        max44009_interface_debug_print("max44009: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        max44009_interface_debug_print("max44009: SCL connected to GPIOB PIN8.\n");
        max44009_interface_debug_print("max44009: SDA connected to GPIOB PIN9.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register max44009 function */
    shell_init();
    shell_register("max44009", max44009);
    uart_print("max44009: welcome to libdriver max44009.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("max44009: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("max44009: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("max44009: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("max44009: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("max44009: param is invalid.\n");
            }
            else
            {
                uart_print("max44009: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
