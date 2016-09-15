Project structure:

/android/ - .apk for onboarding ESP8266 to WiFi Access Point. More at https://github.com/EspressifApp/EsptouchForAndroidForRTOS

/bin/ - precompiled binaries of this project

/driver/ - source code for support of GPIO, Hardware timer, I2C, SPI and UART

/include/ - header files

/ntp/ - Network Time Protocol implementation from Richard Burton https://github.com/raburton/esp8266 . TO BE USED IN FUTURE

/tcp_client/ - Networking part. Contains TCP and UDP cleint/server parts.

/user/ - Contains main program, main.c

combo_esp.sh - Shell script for fast testing. Compiles, flashes firmaware and launches catty.py serial monitor/logger. If you want to use it in your enviroment, please edit PATH variables and USB port number according to your system. Flashing mode is following: "boot=new app=1 spi_speed=40 spi_mode=QIO spi_size_map=3". See end of this file for more info.

Makefile - makefile for compiling. In case if want to add your libraries/sourcefiles, you can do that by creating a separate directory and adding it's name to "SUBDIRS=".

gen_misc.sh - original script for setting compilation attributes and simple compilation. You may want to edit SDK_PATH and BIN_PATH before executing it.

readme.txt - I guess you reading it right now, don't you? :)





Compilation options:

(1) COMPILE
    Possible value: xcc
    Default value:
    If not set, use gcc by default.

(2) BOOT
    Possible value: none/old/new
      none: no need boot
      old: use boot_v1.1
      new: use boot_v1.2
    Default value: new

(3) APP
    Possible value: 0/1/2
      0: original mode, generate eagle.app.v6.flash.bin and eagle.app.v6.irom0text.bin
      1: generate user1
      2: generate user2
    Default value: 0

(3) SPI_SPEED
    Possible value: 20/26.7/40/80
    Default value: 40

(4) SPI_MODE
    Possible value: QIO/QOUT/DIO/DOUT
    Default value: QIO

(4) SPI_SIZE_MAP
    Possible value: 0/2/3/4/5/6
    Default value: 0

For example:
    make COMPILE=gcc BOOT=new APP=1 SPI_SPEED=40 SPI_MODE=QIO SPI_SIZE_MAP=0

2¡¢You can also use gen_misc to make and generate specific bin you needed.
    Linux: ./gen_misc.sh
    Windows: gen_misc.bat
    Follow the tips and steps.
