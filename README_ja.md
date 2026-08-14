[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX44009

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max44009/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MAX44009環境光センサーは、スマートフォン、ノートパソコン、産業用センサーなど、さまざまなポータブルアプリケーションに最適なI2Cデジタル出力を備えています。動作電流は1μA未満で、業界で最も低消費電力の環境光センサーであり、0.045ルクスから188,000ルクスまでの超広帯域22ビットダイナミックレンジを備えています。低照度動作により、暗色ガラスアプリケーションでも容易に動作します。オンチップフォトダイオードの分光応答は、人間の目の環境光認識を模倣するように最適化されており、IRおよびUV遮断機能を備えています。適応ゲインブロックは、カウント/ルクスを最適化するために適切なルクス範囲を自動的に選択します。

LibDriver MAX44009は、LibDriverによって起動されたMAX44009全機能ドライバーであり、輝度の読み取り、輝度の割り込み検出、およびその他の機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MAX44009のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MAX44009用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MAX44009ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MAX44009プログラミング例が含まれています。

/ docディレクトリには、LibDriver MAX44009オフラインドキュメントが含まれています。

/ datasheetディレクトリには、MAX44009データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/max44009/index.html](https://www.libdriver.com/docs/max44009/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。