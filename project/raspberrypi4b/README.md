### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(max44009 REQUIRED)
```

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
./max44009 -i

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
./max44009 -p

max44009: SCL connected to GPIO3(BCM).
max44009: SDA connected to GPIO2(BCM).
```

```shell
./max44009 -t reg --addr=0

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
max44009: set interrupt high threshold 0xF2.
max44009: check interrupt high threshold ok.
max44009: max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test.
max44009: set interrupt low threshold 0x97.
max44009: check interrupt low threshold ok.
max44009: max44009_set_interrupt_low_threshold/max44009_get_interrupt_low_threshold test.
max44009: set threshold timer 0xA2.
max44009: check threshold timer ok.
max44009: max44009_get_interrupt_status test.
max44009: interrupt status is false.
max44009: max44009_threshold_convert_to_register/max44009_threshold_convert_to_data test.
max44009: set threshold 691.5.
max44009: check threshold 645.1.
max44009: max44009_timer_convert_to_register/max44009_timer_convert_to_data test.
max44009: set timer 7793ms.
max44009: check timer 7700.
max44009: finish register test.
```

```shell
./max44009 -t read --addr=0 --times=3

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
max44009: data is 391.68lux.
max44009: data is 391.68lux.
max44009: data is 391.68lux.
max44009: finish read test.
```

```shell
./max44009 -t int --addr=0 --low=100.0 --high=1000.0 --times=10

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
max44009: data is 293.76lux, interrupt status is disable.
max44009: data is 269.28lux, interrupt status is disable.
max44009: data is 146.88lux, interrupt status is disable.
max44009: data is 45.90lux, interrupt status is enable.
max44009: low threshold 45.90lux and please check int pin.
max44009: finish interrupt test.
```

```shell
./max44009 -e read --addr=0 --times=3

1/3.
391.68lux.
2/3.
391.68lux.
3/3.
391.68lux.
```

```shell
./max44009 -e int --addr=0 --low=100.0 --high=1000.0 --times=10

max44009: low threshold is 100.00 lux.
max44009: high threshold is 1000.00 lux.
1/10.
48.96lux.
max44009: low threshold 48.96lux and please check int pin.
```

```shell
./max44009 -h

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

