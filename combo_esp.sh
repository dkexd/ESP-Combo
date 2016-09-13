#!/bin/bash

export PATH=/home/diko/Desktop/esp-open-sdk/xtensa-lx106-elf/bin/:$PATH
export SDK_PATH=/home/diko/Desktop/ESP8266_RTOS_SDK-1.4.0/
export BIN_PATH=/home/diko/Desktop/ESP8266_RTOS_SDK-1.4.0/bin/
export FIRM_PATH=/home/diko/Desktop/ESP8266_RTOS_SDK-1.4.0/bin/upgrade/
export CATTY_PATH=/home/diko/Desktop/catty_script/

echo "combo_esp.sh version 12.09.2016"

boot=new
app=1
spi_speed=40
spi_mode=QIO
spi_size_map=3

make clean

make BOOT=$boot APP=$app SPI_SPEED=$spi_speed SPI_MODE=$spi_mode SPI_SIZE_MAP=$spi_size_map

cd $FIRM_PATH
esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash --flash_freq 40m --flash_mode qio --flash_size 16m 0x0000 "boot_v1.5.bin" 0x01000 user1.2048.new.3.bin 0x1FC000 esp_init_data_default.bin 0x1FE000 blank.bin

cd $CATTY_PATH
python -u catty_fast.py
#gnome-terminal nc 10.10.2.63 7008
#minicom -D /dev/ttyUSB0 -C minicom_log.txt
