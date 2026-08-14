[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX44009

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max44009/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MAX44009環境光感測器具有I2C數位輸出，非常適合智能手機、筆記型電腦和工業感測器等多種可擕式應用。 在小於1μA的工作電流下，它是業界功率最低的環境光感測器，具有0.045勒克斯至188000勒克斯的超寬22比特動態範圍。 低光操作允許在深色玻璃應用中輕鬆操作。 片上光電二極體的光譜響應經過優化，可以類比人眼對環境光的感知，並具有紅外和紫外阻斷能力。 自我調整增益塊會自動選擇正確的勒克斯範圍，以優化計數/勒克斯。

LibDriver MAX44009是LibDriver推出的MAX44009全功能驅動，該驅動提供亮度讀取、亮度中斷檢測等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MAX44009的源文件。

/interface目錄包含了LibDriver MAX44009與平台無關的IIC總線模板。

/test目錄包含了LibDriver MAX44009驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MAX44009編程範例。

/doc目錄包含了LibDriver MAX44009離線文檔。

/datasheet目錄包含了MAX44009數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/max44009/index.html](https://www.libdriver.com/docs/max44009/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。