### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MAX44009

#### 3.1 Command Instruction

1. Show max44009 chip and driver information.

   ```shell
   max44009 (-i | --information)
   ```

2. Show max44009 help.

   ```shell
   max44009 (-h | --help)
   ```

3. Show max44009 pin connections of the current board.

   ```shell
   max44009 (-p | --port)
   ```

4. Run max44009 register test.

   ```shell
   max44009 (-t reg | --test=reg) [--addr=<0 | 1>]
   ```
   
5. Run max44009 read test, num means the test times.

   ```shell
   max44009 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
   ```

6. Run max44009 interrupt test, lux means the interrupt threshold, num means the test times.

   ```shell
   max44009 (-t int | --test=int) [--addr=<0 | 1>] [--low=<lux>] [--high=<lux>] [--times=<num>]
   ```
   
7. Run max44009 reading function, num means the read times.

   ```shell
   max44009 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
   ```

8. Run max44009 interrupt function, lux means the interrupt threshold, num means the test times.

   ```shell
   max44009 (-e int | --example=int) [--addr=<0 | 1>] [--low=<lux>] [--high=<lux>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
max44009 -i

max44009: chip is Analog Devices MAX44009.
max44009: manufacturer is Analog Devices.
max44009: interface is IIC.
max44009: driver version is 1.0.
max44009: min supply voltage is 1.7V.
max44009: max supply voltage is 3.6V.
max44009: max current is 0.00mA.
max44009: max temperature is 85.0C.
max44009: min temperature is -40.0C.
```

```shell
max44009 -p

max44009: SCL connected to GPIOB PIN8.
max44009: SDA connected to GPIOB PIN9.
```

```shell
max44009 -t reg --addr=0

max44009: chip is Analog Devices MAX44009.
max44009: manufacturer is Analog Devices.
max44009: interface is IIC.
max44009: driver version is 1.0.
max44009: min supply voltage is 1.7V.
max44009: max supply voltage is 3.6V.
max44009: max current is 0.002mA.
max44009: max temperature is 85.0C.
max44009: min temperature is -40.0C.
max44009: start register test.
max44009: max44009_set_interrupt/max44009_get_interrupt test.
max44009: set interrupt enable.
max44009: check interrupt ok.
max44009: set interrupt disable.
max44009: check interrupt ok.
max44009: max44009_set_continuous_mode/max44009_get_continuous_mode test.
max44009: enable continuous mode.
max44009: check continuous mode ok.
max44009: disable continuous mode.
max44009: check continuous mode ok.
max44009: max44009_set_auto_range/max44009_get_auto_range test.
max44009: enable auto range.
max44009: check auto range ok.
max44009: disable auto range.
max44009: check auto range ok.
max44009: max44009_set_current_div_8/max44009_get_current_div_8 test.
max44009: enable current div 8.
max44009: check current div 8 ok.
max44009: disable current div 8.
max44009: check current div 8 ok.
max44009: max44009_set_integration_time/max44009_get_integration_time test.
max44009: set integration time 800ms.
max44009: check integration time ok.
max44009: set integration time 400ms.
max44009: check integration time ok.
max44009: set integration time 200ms.
max44009: check integration time ok.
max44009: set integration time 100ms.
max44009: check integration time ok.
max44009: set integration time 50ms.
max44009: check integration time ok.
max44009: set integration time 25ms.
max44009: check integration time ok.
max44009: set integration time 12.5ms.
max44009: check integration time ok.
max44009: set integration time 6.25ms.
max44009: check integration time ok.
max44009: max44009_set_interrupt_high_threshold/max44009_get_interrupt_high_threshold test.
max44009: set interrupt high threshold 0xF7.
max44009: check interrupt high threshold ok.
max44009: max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test.
max44009: set interrupt low threshold 0xDE.
max44009: check interrupt low threshold ok.
max44009: max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test.
max44009: set threshold timer 0xB8.
max44009: check threshold timer ok.
max44009: max44009_get_interrupt_status test.
max44009: interrupt status is false.
max44009: max44009_threshold_convert_to_register/max44009_threshold_convert_to_data test.
max44009: set threshold 316.0.
max44009: check threshold 299.5.
max44009: max44009_timer_convert_to_register/max44009_timer_convert_to_data test.
max44009: set timer 2583ms.
max44009: check timer 2500.
max44009: finish register test.
```

```shell
max44009 -t read --addr=0 --times=3

max44009: chip is Analog Devices MAX44009.
max44009: manufacturer is Analog Devices.
max44009: interface is IIC.
max44009: driver version is 1.0.
max44009: min supply voltage is 1.7V.
max44009: max supply voltage is 3.6V.
max44009: max current is 0.002mA.
max44009: max temperature is 85.0C.
max44009: min temperature is -40.0C.
max44009: start read test.
max44009: data is 685.44lux.
max44009: data is 685.44lux.
max44009: data is 685.44lux.
max44009: finish read test.
```

```shell
max44009 -t int --addr=0 --low=100.0 --high=1000.0 --times=10

max44009: chip is Analog Devices MAX44009.
max44009: manufacturer is Analog Devices.
max44009: interface is IIC.
max44009: driver version is 1.0.
max44009: min supply voltage is 1.7V.
max44009: max supply voltage is 3.6V.
max44009: max current is 0.002mA.
max44009: max temperature is 85.0C.
max44009: min temperature is -40.0C.
max44009: start interrupt test.
max44009: data is 171.36lux, interrupt status is enable.
max44009: data is 195.84lux, interrupt status is disable.
max44009: data is 440.64lux, interrupt status is disable.
max44009: data is 1468.80lux, interrupt status is enable.
max44009: high threshold 1468.80lux and please check int pin.
max44009: finish interrupt test.
```

```shell
max44009 -e read --addr=0 --times=3

1/3.
636.48lux.
2/3.
636.48lux.
3/3.
636.48lux.
```

```shell
max44009 -e int --addr=0 --low=100.0 --high=1000.0 --times=10

max44009: low threshold is 100.00 lux.
max44009: high threshold is 1000.00 lux.
1/10.
293.76lux.
2/10.
1175.04lux.
max44009: high threshold 1175.04lux and please check int pin.
```

```shell
max44009 -h

Usage:
  max44009 (-i | --information)
  max44009 (-h | --help)
  max44009 (-p | --port)
  max44009 (-t reg | --test=reg) [--addr=<0 | 1>]
  max44009 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
  max44009 (-t int | --test=int) [--addr=<0 | 1>] [--low=<lux>] [--high=<lux>] [--times=<num>]
  max44009 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
  max44009 (-e int | --example=int) [--addr=<0 | 1>] [--low=<lux>] [--high=<lux>] [--times=<num>]

Options:
      --addr=<0 | 1>              Set the chip iic address.([default: 0])
  -e <read | int>, --example=<read | int>
                                  Run the driver example.
  -h, --help                      Show the help.
      --high=<lux>                Set the high threshold in lux.([default: 1000.0])
      --low=<lux>                 Set the low threshold in lux.([default: 100.0])
  -i, --information               Show the chip information.
  -p, --port                      Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                                  Run the driver test.
      --times=<num>               Set the running times.([default: 3])
```

