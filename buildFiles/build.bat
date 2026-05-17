@echo off
REM ATmega328P Bootloader Build Script

set AVR=C:\Users\<username>\AppData\Local\Arduino15\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2\bin
set SRC=src\main.c
set BUILD=build
set OUT=bootloader
set MCU=atmega328p
set F_CPU=16000000UL
set BOOT_ADDR=0x7C00

REM ── Create build folder if it doesn't exist ──
if not exist %BUILD% (
    mkdir %BUILD%
    echo Created build\ folder.
)

echo [1/4] Compiling...
%AVR%\avr-gcc.exe -mmcu=%MCU% -DF_CPU=%F_CPU% -Os -std=c99 ^
    -Wl,--section-start=.text=%BOOT_ADDR% ^
    -o %BUILD%\%OUT%.elf %SRC%
if errorlevel 1 goto error

echo [2/4] Creating .hex file...
%AVR%\avr-objcopy.exe -O ihex -R .eeprom %BUILD%\%OUT%.elf %BUILD%\%OUT%.hex
if errorlevel 1 goto error

echo [3/4] Checking binary size...
%AVR%\avr-size.exe --format=avr --mcu=%MCU% %BUILD%\%OUT%.elf

echo [4/4] Done!
echo Output: %BUILD%\%OUT%.hex
goto end

:error
echo BUILD FAILED!
:end
pause