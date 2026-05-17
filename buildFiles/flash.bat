@echo off
REM ATmega328P Bootloader Flash Script
 
set AVRDUDE=C:\Users\<username>\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\bin\avrdude.exe
set AVRDUDE_CONF=C:\Users\<username>\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf
set HEX=build\bootloader.hex
set MCU=atmega328p
set PROGRAMMER=usbasp
 
echo [1/3] Setting fuses...
%AVRDUDE% -C %AVRDUDE_CONF% -p %MCU% -c %PROGRAMMER% -U hfuse:w:0xDA:m
if errorlevel 1 goto error
 
echo [2/3] Flashing bootloader...
%AVRDUDE% -C %AVRDUDE_CONF% -p %MCU% -c %PROGRAMMER% -U flash:w:%HEX%:i
if errorlevel 1 goto error
 
echo [3/3] Setting lock bits...
%AVRDUDE% -C %AVRDUDE_CONF% -p %MCU% -c %PROGRAMMER% -U lock:w:0xEF:m
if errorlevel 1 goto error
 
echo All done! Bootloader installed successfully.
goto end
:error
echo FAILED! Check USBasp connection.
:end
pause
