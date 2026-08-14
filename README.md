[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX44009

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max44009/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The MAX44009 ambient light sensor features an I2C digital output that is ideal for a number of portable applications such as smartphones, notebooks, and industrial sensors. At less than 1μA operating current, it is the lowest power ambient light sensor in the industry and features an ultra-wide 22-bit dynamic range from 0.045 lux to 188,000 lux. Low-light operation allows easy operation in dark-glass applications. The on-chip photodiode’s spectral response is optimized to mimic the human eye’s perception of ambient light and incorporates IR and UV blocking capability. The adaptive gain block automatically selects the correct lux range to optimize the counts/lux.

LibDriver MAX44009 is a full-featured driver for MAX44009, launched by LibDriver.It provides brightness reading, brightness interrupt detection and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver MAX44009 source files.

/interface includes LibDriver MAX44009 IIC platform independent template.

/test includes LibDriver MAX44009 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver MAX44009 sample code.

/doc includes LibDriver MAX44009 offline document.

/datasheet includes MAX44009 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_max44009_basic.h"

uint8_t res;
uint32_t i;

/* basic init */
res = max44009_basic_init(MAX44009_ADDRESS_0);
if (res != 0)
{
    return 1;
}

...
    
/* delay 1000ms */
max44009_interface_delay_ms(1000);

/* loop */
for (i = 0; i < 3; i++)
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
    max44009_interface_debug_print("%d/%d.\n", i + 1, 3);
    max44009_interface_debug_print("%.2flux.\n", lux);

    /* delay 1000ms */
    max44009_interface_delay_ms(1000);
    
    ...
}

...
    
/* deinit */
(void)max44009_basic_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_max44009_interrupt.h"

uint8_t res;
uint32_t i;
float low_threshold = 100.0f;
float high_threshold = 1000.0f;

/* output */
max44009_interface_debug_print("max44009: low threshold is %0.2f lux.\n", low_threshold);
max44009_interface_debug_print("max44009: high threshold is %0.2f lux.\n", high_threshold);

...
    
/* interrupt init */
res = max44009_interrupt_init(MAX44009_ADDRESS_0, low_threshold, high_threshold, 500.0f);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
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
    max44009_interface_debug_print("%d/%d.\n", i + 1, 3);
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
    
    ...
}

...
    
/* deinit */
max44009_interrupt_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/max44009/index.html](https://www.libdriver.com/docs/max44009/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.