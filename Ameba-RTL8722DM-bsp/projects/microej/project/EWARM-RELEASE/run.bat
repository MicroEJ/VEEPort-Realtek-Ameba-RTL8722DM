@echo off

REM Copyright 2021 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.
REM 

REM 'run.bat' implementation for AmebaD SDK (IAR compiler).

REM 'run.bat' is responsible for flashing the executable file on the target device 
REM then resetting target device

REM This file has concatenated and made portable options from 'settings/km4_application.is.general.xcl' and 'km4_application.is.driver.xcl'
REM Any changes in the IAR settings must be reported to the command line above.

CALL "%~dp0\set_project_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

@echo on 
cd %IAREW_PROJECT_DIR%
"%IAREW_INSTALLATION_DIR%\common\bin\cspybat" "%IAREW_INSTALLATION_DIR%\arm\bin\armproc.dll" "%IAREW_INSTALLATION_DIR%\arm\bin\armjlink2.dll" "%IAREW_PROJECT_DIR%\Debug\Exe\km4_image\km4_application.axf" --plugin="%IAREW_INSTALLATION_DIR%\arm\bin\armbat.dll" --macro="%IAREW_PROJECT_DIR%\run.mac" --flash_loader="%IAREW_PROJECT_DIR%\iar_utility\common\flashloader\FlashRTL8721D_hp_microej.board" --backend "--endian=little" "--cpu=Cortex-M33" "--fpu=VFPv5_SP" "-p" "%IAREW_PROJECT_DIR%\..\..\..\..\sdk\sdk-amebad_v6.2C-RC\component\soc\realtek\amebad\misc\iar_utility\common\8721dhp.ddf" "--semihosting=none" "--device=Cortex-M33" "--drv_communication=USB0" "--drv_interface_speed=auto" "--jlink_initial_speed=1000" "--jlink_reset_strategy=0,0" "--drv_interface=SWD" "--drv_catch_exceptions=0x00" "--jlink_script_file=%IAREW_PROJECT_DIR%\settings\AP2_KM4.JLinkScript"
