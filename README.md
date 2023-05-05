# STM32_Boot_Loader
Configurable UART boot loader for STM32

This project gives the ability of programming STM32 using UART interface. (see 1st release)

# Flash Over The Air (FOTA):
Added ability of connecting to FTP server, checking if update is available, downloading and flashing updates.
(uses esp8266 as WiFi module), (see current version of the repo).

* "Server" program runs on the computer that works as a version update server.
* "MCU" program is flashed on the STM32f103.
