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
 * @file      driver_max44009.c
 * @brief     driver max44009 source file
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

#include "driver_max44009.h"
#include <math.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Analog Devices MAX44009"        /**< chip name */
#define MANUFACTURER_NAME         "Analog Devices"                 /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.7f                             /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                             /**< chip max supply voltage */
#define MAX_CURRENT               0.0016f                          /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                           /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            1000                             /**< driver version */

/**
 * @brief chip register definition
 */
#define MAX44009_REG_INTERRUPT_STATUS        0x00        /**< interrupt status register */
#define MAX44009_REG_INTERRUPT_ENABLE        0x01        /**< interrupt enable register */
#define MAX44009_REG_CONFIGURATION           0x02        /**< configuration register */
#define MAX44009_REG_LUX_READING_HIGH        0x03        /**< lux reading high register */
#define MAX44009_REG_LUX_READING_LOW         0x04        /**< lux reading low register */
#define MAX44009_REG_THRESHOLD_UPPER         0x05        /**< upper threshold register */
#define MAX44009_REG_THRESHOLD_LOWER         0x06        /**< lower threshold register */
#define MAX44009_REG_THRESHOLD_TIMER         0x07        /**< threshold timer register */

/**
 * @brief      read bytes
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_max44009_iic_read(max44009_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)        /* read the register */
    {
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_max44009_iic_write(max44009_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)        /* write the register */
    {
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief     set the address pin
 * @param[in] *handle pointer to a max44009 handle structure
 * @param[in] addr_pin chip address pins
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t max44009_set_addr_pin(max44009_handle_t *handle, max44009_address_t addr_pin)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }

    handle->iic_addr = addr_pin;        /* set iic addr */
    
    return 0;                           /* success return 0 */
}

/**
 * @brief      get the address pin
 * @param[in]  *handle pointer to a max44009 handle structure
 * @param[out] *addr_pin pointer to a chip address pins buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max44009_get_addr_pin(max44009_handle_t *handle, max44009_address_t *addr_pin)
{
    if (handle == NULL)                                        /* check handle */
    {
        return 2;                                              /* return error */
    }

    *addr_pin = (max44009_address_t)(handle->iic_addr);        /*get iic address */
    
    return 0;                                                  /* success return 0 */
}

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
uint8_t max44009_get_interrupt_status(max44009_handle_t *handle, max44009_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_INTERRUPT_STATUS, &prev, 1);        /* read data */
    if (res != 0)                                                                      /* check error */
    {
        handle->debug_print("max44009: get interrupt status failed.\n");               /* get interrupt status failed */
        
        return 1;                                                                      /* return error */
    }
    *enable = (max44009_bool_t)((prev >> 0) & 0x01);                                   /* set bool */
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t max44009_set_interrupt(max44009_handle_t *handle, max44009_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_INTERRUPT_ENABLE, &prev, 1);        /* read data */
    if (res != 0)                                                                      /* check error */
    {
        handle->debug_print("max44009: get interrupt enable failed.\n");               /* get interrupt enable failed */
        
        return 1;                                                                      /* return error */
    }
    prev &= ~(1 << 0);                                                                 /* clear settings */
    prev |= ((uint8_t)(enable) & 0x01) << 0;                                           /* set bool */
    res = a_max44009_iic_write(handle, MAX44009_REG_INTERRUPT_ENABLE, &prev, 1);       /* write data */
    if (res != 0)                                                                      /* check error */
    {
        handle->debug_print("max44009: set interrupt enable failed.\n");               /* set interrupt enable failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t max44009_get_interrupt(max44009_handle_t *handle, max44009_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_INTERRUPT_ENABLE, &prev, 1);        /* read data */
    if (res != 0)                                                                      /* check error */
    {
        handle->debug_print("max44009: get interrupt enable failed.\n");               /* get interrupt enable failed */
        
        return 1;                                                                      /* return error */
    }
    *enable = (max44009_bool_t)((prev >> 0) & 0x01);                                   /* set bool */
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t max44009_set_continuous_mode(max44009_handle_t *handle, max44009_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 7);                                                               /* clear settings */
    prev |= ((uint8_t)(enable) & 0x01) << 7;                                         /* set bool */
    res = a_max44009_iic_write(handle, MAX44009_REG_CONFIGURATION, &prev, 1);        /* write data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: set configuration failed.\n");                /* set configuration failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_get_continuous_mode(max44009_handle_t *handle, max44009_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (max44009_bool_t)((prev >> 7) & 0x1);                                  /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_set_auto_range(max44009_handle_t *handle, max44009_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 6);                                                               /* clear settings */
    prev |= ((uint8_t)(!enable) & 0x01) << 6;                                        /* set bool */
    res = a_max44009_iic_write(handle, MAX44009_REG_CONFIGURATION, &prev, 1);        /* write data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: set configuration failed.\n");                /* set configuration failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_get_auto_range(max44009_handle_t *handle, max44009_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (max44009_bool_t)(!((prev >> 6) & 0x1));                               /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_set_current_div_8(max44009_handle_t *handle, max44009_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(1 << 3);                                                               /* clear settings */
    prev |= ((uint8_t)(enable) & 0x01) << 3;                                         /* set bool */
    res = a_max44009_iic_write(handle, MAX44009_REG_CONFIGURATION, &prev, 1);        /* write data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: set configuration failed.\n");                /* set configuration failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_get_current_div_8(max44009_handle_t *handle, max44009_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (max44009_bool_t)((prev >> 3) & 0x1);                                  /* set bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_set_integration_time(max44009_handle_t *handle, max44009_integration_time_t t)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    prev &= ~(7 << 0);                                                               /* clear settings */
    prev |= ((uint8_t)(t) & 0x7) << 0;                                               /* set time */
    res = a_max44009_iic_write(handle, MAX44009_REG_CONFIGURATION, &prev, 1);        /* write data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: set configuration failed.\n");                /* set configuration failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_get_integration_time(max44009_handle_t *handle, max44009_integration_time_t *t)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 1;                                                                    /* return error */
    }
    *t = (max44009_integration_time_t)((prev >> 0) & 0x7);                           /* set time */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_set_interrupt_high_threshold(max44009_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    prev = threshold;                                                                  /* set threshold */
    res = a_max44009_iic_write(handle, MAX44009_REG_THRESHOLD_UPPER, &prev, 1);        /* write data */
    if (res != 0)                                                                      /* check error */
    {
        handle->debug_print("max44009: set upper threshold failed.\n");                /* set upper threshold failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t max44009_get_interrupt_high_threshold(max44009_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_THRESHOLD_UPPER, &prev, 1);        /* read data */
    if (res != 0)                                                                     /* check error */
    {
        handle->debug_print("max44009: get upper threshold failed.\n");               /* get upper threshold failed */
        
        return 1;                                                                     /* return error */
    }
    *threshold = prev;                                                                /* set threshold */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t max44009_set_interrupt_low_threshold(max44009_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    prev = threshold;                                                                  /* set threshold */
    res = a_max44009_iic_write(handle, MAX44009_REG_THRESHOLD_LOWER, &prev, 1);        /* write data */
    if (res != 0)                                                                      /* check error */
    {
        handle->debug_print("max44009: set lower threshold failed.\n");                /* set lower threshold failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t max44009_get_interrupt_low_threshold(max44009_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_THRESHOLD_LOWER, &prev, 1);        /* read data */
    if (res != 0)                                                                     /* check error */
    {
        handle->debug_print("max44009: get lower threshold failed.\n");               /* get lower threshold failed */
        
        return 1;                                                                     /* return error */
    }
    *threshold = prev;                                                                /* set threshold */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t max44009_set_threshold_timer(max44009_handle_t *handle, uint8_t t)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    prev = t;                                                                          /* set threshold timer */
    res = a_max44009_iic_write(handle, MAX44009_REG_THRESHOLD_TIMER, &prev, 1);        /* write data */
    if (res != 0)                                                                      /* check error */
    {
        handle->debug_print("max44009: set threshold timer failed.\n");                /* set threshold timer failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t max44009_get_threshold_timer(max44009_handle_t *handle, uint8_t *t)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_THRESHOLD_TIMER, &prev, 1);        /* read data */
    if (res != 0)                                                                     /* check error */
    {
        handle->debug_print("max44009: get threshold timer failed.\n");               /* get threshold timer failed */
        
        return 1;                                                                     /* return error */
    }
    *t = prev;                                                                        /* set data */
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t max44009_threshold_convert_to_register(max44009_handle_t *handle, float lux, uint8_t *reg)
{
    uint16_t remain;
    int xp;
    float f;
    float fraction;
    
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    f = lux / 0.045f;                                             /* convert */
    fraction = frexpf(f, &xp);                                    /* run frexp */
    while ((xp > 0xF) || (fraction < 240.0f))                     /* adjust params */
    {
        if (xp == 0)                                              /* reach the min */
        {
            break;                                                /* break */
        }
        if (fraction * 2.0f > 240.0f)                             /* reach the max */
        {
            break;                                                /* break */
        }
        xp--;                                                     /* xp-- */
        fraction *= 2.0f;                                         /* fraction * 2 */
    }
    remain = (uint8_t)fraction;                                   /* get the integer part */
    remain >>= 4;                                                 /* right shift */
    *reg = (((uint8_t)(xp & 0xF)) << 4) | (remain & 0x0F);        /* convert real data to register data */
    
    return 0;                                                     /* success return 0 */
}

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
uint8_t max44009_threshold_convert_to_data(max44009_handle_t *handle, uint8_t reg, float *lux)
{
    uint8_t exponent;
    uint8_t fractional;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    exponent = (reg >> 4) & 0xF;                                              /* set exponent */
    fractional = (reg & 0xF) << 4;                                            /* set fractional */
    *lux = 0.045f * powf(2.0f, (float)exponent) * ((float)fractional);        /* calculate lux */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t max44009_timer_convert_to_register(max44009_handle_t *handle, float ms, uint8_t *reg)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *reg = (uint8_t)(ms / 100.0f);        /* convert */
    
    return 0;                             /* success return 0 */
}

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
uint8_t max44009_timer_convert_to_data(max44009_handle_t *handle, uint8_t reg, float *ms)
{
    if (handle == NULL)              /* check handle */
    {
        return 2;                    /* return error */
    }
    if (handle->inited != 1)         /* check handle initialization */
    {
        return 3;                    /* return error */
    }
    
    *ms = (float)(reg) * 100;        /* convert */
    
    return 0;                        /* success return 0 */
}

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
uint8_t max44009_init(max44009_handle_t *handle)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->debug_print == NULL)                                   /* check debug_print */
    {
        return 3;                                                      /* return error */
    }
    if (handle->iic_init == NULL)                                      /* check iic_init */
    {
        handle->debug_print("max44009: iic_init is null.\n");          /* iic_init is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->iic_deinit == NULL)                                    /* check iic_deinit */
    {
        handle->debug_print("max44009: iic_deinit is null.\n");        /* iic_deinit is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->iic_read == NULL)                                      /* check iic_read */
    {
        handle->debug_print("max44009: iic_read is null.\n");          /* iic_read is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->iic_write == NULL)                                     /* check iic_write */
    {
        handle->debug_print("max44009: iic_write is null.\n");         /* iic_write is null */
        
        return 3;                                                      /* return error */
    }
    if (handle->delay_ms == NULL)                                      /* check delay_ms */
    {
        handle->debug_print("max44009: delay_ms is null.\n");          /* delay_ms is null */
        
        return 3;                                                      /* return error */
    }
    
    if (handle->iic_init() != 0)                                       /* iic init */
    {
        handle->debug_print("max44009: iic init failed.\n");           /* iic init failed */
        
        return 1;                                                      /* return error */
    }
    
    handle->inited = 1;                                                /* flag finish initialization */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max44009_deinit(max44009_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_CONFIGURATION, &prev, 1);         /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: get configuration failed.\n");                /* get configuration failed */
        
        return 4;                                                                    /* return error */
    }
    prev &= ~(1 << 7);                                                               /* clear settings */
    res = a_max44009_iic_write(handle, MAX44009_REG_CONFIGURATION, &prev, 1);        /* write data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: set configuration failed.\n");                /* set configuration failed */
        
        return 4;                                                                    /* return error */
    }
    res = handle->iic_deinit();                                                      /* iic deinit */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: iic deinit failed.\n");                       /* iic deinit failed */
        
        return 1;                                                                    /* return error */
    }
    handle->inited = 0;                                                              /* flag closed */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_read(max44009_handle_t *handle, uint16_t *raw, float *lux)
{
    uint8_t res;
    uint8_t exponent;
    uint8_t mantissa;
    uint8_t buf[2];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = a_max44009_iic_read(handle, MAX44009_REG_LUX_READING_HIGH, buf, 2);        /* read data */
    if (res != 0)                                                                    /* check error */
    {
        handle->debug_print("max44009: read data failed.\n");                        /* read data failed */
        
        return 1;                                                                    /* return error */
    }
    *raw = (uint16_t)((uint16_t)(buf[0]) << 8) | buf[1];                             /* set raw output */
    exponent = (buf[0] >> 4) & 0xF;                                                  /* get exponent */
    mantissa = ((buf[0] & 0xF) << 4) | (buf[1] & 0x0F);                              /* get mantissa */
    *lux = powf(2.0f, (float)exponent) * (float)mantissa * 0.045f;                   /* convert to lux */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t max44009_set_reg(max44009_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    if (a_max44009_iic_write(handle, reg, buf, len) != 0)          /* iic write */
    {
        handle->debug_print("max44009: set reg failed.\n");        /* set reg failed */
        
        return 1;                                                  /* return error */
    }
    
    return 0;                                                      /* success return 0 */
}

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
uint8_t max44009_get_reg(max44009_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    if (a_max44009_iic_read(handle, reg, buf, len) != 0)           /* iic read */
    {
        handle->debug_print("max44009: get reg failed.\n");        /* get reg failed */
        
        return 1;                                                  /* return error */
    }
    
    return 0;                                                      /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a max44009 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max44009_info(max44009_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(max44009_info_t));                       /* initialize max44009 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set maximum temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set minimal temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
