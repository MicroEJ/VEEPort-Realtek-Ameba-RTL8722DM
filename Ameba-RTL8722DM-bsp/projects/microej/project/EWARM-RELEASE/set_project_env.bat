@echo off

REM Copyright 2021 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.
REM 

REM 'set_project_env.bat' implementation for AmebaD SDK (IAR compiler).

REM 'set_project_env' is responsible for
REM - checking the availability of required environment variables 
REM - setting project local variables for 'build.bat' and 'run.bat' 

REM Required Environment Variable
REM IAREW_CHANGEIT_INSTALLATION_DIR
SET IAREW_CHANGEIT_INSTALLATION_DIR=C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.2

IF "%IAREW_CHANGEIT_INSTALLATION_DIR%" == "" (
    ECHO Please set the environment variable 'IAREW_CHANGEIT_INSTALLATION_DIR'
    exit /B -1
)

REM Set the project variables required by the external build script for IAR Embedded Workbench
SET IAREW_INSTALLATION_DIR=%IAREW_CHANGEIT_INSTALLATION_DIR%
SET IAREW_PROJECT_DIR=%~dp0
SET IAREW_PROJECT_NAME=km4_application
SET IAREW_PROJECT_CONFIGURATION=is

ECHO IAREW_INSTALLATION_DIR=%IAREW_INSTALLATION_DIR%
ECHO IAREW_PROJECT_DIR=%IAREW_PROJECT_DIR%
ECHO IAREW_PROJECT_NAME=%IAREW_PROJECT_NAME%
ECHO IAREW_PROJECT_CONFIGURATION=%IAREW_PROJECT_CONFIGURATION%
exit /B 0
