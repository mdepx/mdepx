# stm32f469i-disco-dsi
Driving DSI panel on STM32F469I-DISCO 

Connect UART pins as follows:

| STM32F469I-DISCO  | UART-to-USB adapter  |
| ----------------- | -------------------- |
| D0 (RX)           | TX                   |
| D1 (TX)           | RX                   |

UART baud rate: 115200

### Build under Linux/FreeBSD

    $ export CROSS_COMPILE=arm-none-eabi-
    $ make

### Program font
    $ sudo openocd -s /path/to/openocd/tcl -f interface/stlink-v2-1.cfg -f target/stm32f4x.cfg -c 'program fonts/ter-x24n.pcf reset 0x081b0000 exit'

### Program mdepx
    $ sudo openocd -s /path/to/openocd/tcl -f interface/stlink-v2-1.cfg -f target/stm32f4x.cfg -c 'program obj/stm32f469i-disco-dsi.bin reset 0x08000000 exit'

![alt text](https://raw.githubusercontent.com/machdep/mdepx/master/board/stm32f469i-disco/images/stm32f469i-disco-dsi.jpg)
