[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX44009

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max44009/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der Umgebungslichtsensor MAX44009 verfügt über einen digitalen I²C-Ausgang und eignet sich ideal für eine Vielzahl mobiler Anwendungen wie Smartphones, Notebooks und industrielle Sensoren. Mit einem Betriebsstrom von unter 1 µA ist er der energieeffizienteste Umgebungslichtsensor der Branche und bietet einen extrem weiten 22-Bit-Dynamikbereich von 0,045 Lux bis 188.000 Lux. Dank seiner Leistungsfähigkeit bei schwachem Licht ist er auch in Anwendungen mit dunklem Glas problemlos einsetzbar. Die spektrale Empfindlichkeit der integrierten Fotodiode ist so optimiert, dass sie die Wahrnehmung von Umgebungslicht durch das menschliche Auge nachbildet und IR- sowie UV-Strahlung blockiert. Die adaptive Verstärkungsregelung wählt automatisch den optimalen Lux-Bereich für maximale Messwerte pro Lux.

LibDriver MAX44009 ist der MAX44009-Vollfunktionstreiber, der von LibDriver gestartet wurde. Er bietet Helligkeitsmessung, Erkennung von Helligkeitsunterbrechungen und andere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MAX44009-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MAX44009 IIC.

/test enthält den Testcode des LibDriver MAX44009-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver MAX44009-Beispielcode.

/doc enthält das LibDriver MAX44009-Offlinedokument.

/Datenblatt enthält MAX44009-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/max44009/index.html](https://www.libdriver.com/docs/max44009/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.