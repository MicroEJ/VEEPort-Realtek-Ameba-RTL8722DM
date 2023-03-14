@echo off

REM Copyright 2021-2023 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.
REM 

REM 'build.bat' implementation for AmebaD SDK (IAR compiler).

REM 'build.bat' is responsible for producing the executable file 
REM then copying this executable file to the current directory where it has been executed to a file named 'application.out'

CALL "%~dp0\set_project_env.bat"
IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

REM del /f "%IAREW_PROJECT_DIR%\Debug\Exe\km4_image\%IAREW_PROJECT_NAME%.axf"
IF %ERRORLEVEL% NEQ 0 (
  echo [Info] cannot delete "%IAREW_PROJECT_DIR%\Debug\Exe\km4_image\%IAREW_PROJECT_NAME%.axf"
)

REM del /f "%IAREW_PROJECT_DIR%\Debug\Exe\km4_image\%IAREW_PROJECT_NAME%.dbg.axf"
IF %ERRORLEVEL% NEQ 0 (
  echo [Info] cannot delete "%IAREW_PROJECT_DIR%\Debug\Exe\km4_image\%IAREW_PROJECT_NAME%.dbg.axf"
)

@echo on
"%IAREW_INSTALLATION_DIR%\common\bin\IarBuild.exe" "%IAREW_PROJECT_DIR%\%IAREW_PROJECT_NAME%.ewp" -make %IAREW_PROJECT_CONFIGURATION% -log all

@echo off
IF %ERRORLEVEL% NEQ 0 (
  echo [Error] IAR build failed.
  exit /B %ERRORLEVEL%
)

@echo on

copy /Y "%IAREW_PROJECT_DIR%\Debug\Exe\km4_image\%IAREW_PROJECT_NAME%.axf" application.out

echo Build successful.