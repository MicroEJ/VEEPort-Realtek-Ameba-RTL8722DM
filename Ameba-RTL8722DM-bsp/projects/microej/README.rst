.. Copyright 2020-2023 MicroEJ Corp. All rights reserved._
.. Use of this source code is governed by a BSD-style license that can be found with this software._

.. |BOARD_NAME| replace:: Ameba RTL8722DM
.. |BOARD_REVISION| replace:: RTL-AMEBAD_MB_3V0
.. |VEEPORT_VER| replace:: 1.0.1
.. |RCP| replace:: MICROEJ SDK
.. |VEEPORT| replace:: VEE Port
.. |VEEPORTS| replace:: VEE Ports
.. |SIM| replace:: MicroEJ Simulator
.. |ARCH| replace:: MicroEJ Architecture
.. |CIDE| replace:: MICROEJ SDK
.. |RTOS| replace:: FreeRTOS RTOS
.. |MANUFACTURER| replace:: Realtek

.. _README: ../README.rst
.. _RELEASE NOTES: ../RELEASE_NOTES.rst
.. _CHANGELOG: ../CHANGELOG.rst


================
|BOARD_NAME| BSP
================

This project contains the BSP sources of the |VEEPORT| for the
|BOARD_NAME|.  The BSP project is based on IAR IDE with realtedk sdk framework.

This document does not describe how to setup the |VEEPORT|.  Please
refer to the `README`_ for that.

Build & Flash Scripts
=====================

In the folder ``Project/microej/project/EWARM-RELEASE/`` are scripts that can be
used to build and flash the BSP.  The ``.bat`` scripts are
meant to run in a Windows environment.

- The ``build*`` scripts are used to compile and link the BSP with a
  MicroEJ Application to produce a MicroEJ Firmware
  (``application.out``) that can be flashed on a device.

  The ``build*`` scripts work out of the box, assuming the toolchain is
  installed in the default path.

- The ``run*`` scripts are used to flash a MicroEJ Firmware
  (``application.out``) on a device.


As documented by IAR, the build scripts need to be updated to your local path,
update the environment variables accordingly.

The environment variables can be defined globally by the user or in
the ``set_project_env*`` scripts.  When the ``.bat`` scripts
are executed, the ``set_project_env.bat`` script is executed if it exists.
Create and configure these files to customize the environment locally. 


How to Build Ameba RTL8722DM BSP Projects with IAR
==================================================

Overview
--------

Ameba RTL8722DM BSP project.

Compiler: IAR 8.3

Prerequisites
-------------

Install IAR Embedded Workbench for ARM version 8.32.1

MicroEJ is added to ``km4_application`` project.

Build KM4 Projects
------------------

- Open the workspace file ``Ameba-RTL8722DM-bsp\Projects\Projects\microej\project\EWARM-RELEASE\Project_hp_release.eww`` with IAR Workbench
- Right click on km4_application(is) project and click on Make.


SSL troubleshooting
-------------------

If you want to use ``SSL`` directly in C and not in Java, you have to remove the macro ``MBEDTLS_CONFIG_FILE`` in IAR project configuration.
This macro uses a custom MicroEJ configuration file for ``mbedtls`` and if you don't remove it, you may have ``hardfault`` issues.
Below are steps to remove this macro:

* Open ``km4_application.ewp`` file with a text editor
* Remove all occurrences of the line ``<state>MBEDTLS_CONFIG_FILE="microej_mbedtls_config.h"</state>``

Images Downloading and Debugging
================================

Ameba-D board supports using J-Link and SWD to download and debug. Image of each project can be download individually. 

Please refers to the documentation ``Ameba-RTL8722DM-bsp\Projects\sdk-amebad_v6.2C-RC\doc\AmebaD\AN0400 Ameba-D Application Note v12.pdf`` (section 6.2) to see how to setup the board and connect J-link for image downloading.

KM4 is power-on by KM0, you should make sure that KM0 has already boot up before debug KM4.

If the Flash memory is empty, the sequence to download images is:

- Download for ``km0_bootloader`` and ``km0_application`` projects
- Click Reset button of the board to make KM0 boots up
- Download for ``km4_bootloader`` and ``km4_application`` projects

During development, if Flash memory is not empty and KM0 can boot up successfully, then you can download updated images to KM4 directly, and there is no need to re-download for KM0.

Image Downloading
-----------------

The following steps show how to download image for the target project with IAR. 
Don't forget to download the image in the right order.

- Select the project you want to download
- Click ``Project`` > ``Download`` > ``Download active application`` to download the image.
- After a successful download, press the Reset button and you can see the device is booted with new image.

Image Debugging
---------------

- Select the project you want to debug
- Click ``Project`` > ``Download and Debug``

It has been confirmed by Realtek that global performances are slightly lower in Debug mode, 13% on average.
When JTAG/SWD is connected, the AmebaD cpu must allocate time to process the request from jtag/swd driver or debug software so the performance will be reduced as compare to normal mode.
This is expected and unavoidable behavior to have slightly lower performance under IAR debug mode.


UART Programming
================

The Ameba-D board provides an alternative flashing mode, failsafe, using serial port.
It may be useful when the Jtag comunication stops to work. This may happen with
a bad memory mapping in the image or with a J-Link adapter malfunction.

Realtek ImageTool
-----------------

ImageTool is the UART programming software provided by Realtek, is part
of Realtek Ambiot preoject https://github.com/ambiot/ambd_sdk and can
be found in ``tools/AmebaD/Image_Tool`` folder in this repository, for use
ImageTool application is just needed to copy the content of this folder.

How to use ImageTool
--------------------

Please follow instructions in the section ``8.3 Download`` of the document ``AN0400 Ameba-D Application Note``
located in the folder ``Ameba-RTL8722DM-bsp/Projects/sdk-amebad_v6.2C-RC/doc/AmebaD``.
Since the rotary switch works only when MicroUI is used in your Java application, make sure the method ``MicroUI.start()`` is called in your Java main method.




Runtime thread analysis with SystemView
=======================================

Enable SystemView
-----------------

SystemView feature is not enabled by default in order to provide better performances with the |VEEPORT|.

To enable SystemView:

* You need to define the macro ``ENABLE_SYSTEM_VIEW`` in ``FreeRTOSConfig.h`` file located at ``"Ameba-RTL8722DM-bsp/Projects/microej/project/inc/inc_hp/FreeRTOSConfig.h"``. This macro is commented by default. Uncomment it to enable SystemView: ``#define ENABLE_SYSTEM_VIEW``

* Rebuild all the km4_application IAR project (``Right click on km4_application`` > ``Rebuild all``).


Install & start SystemView
--------------------------

* Check in the SystemView code which version of Systemview is implemented.
	It Can be checked for example in ``"Ameba-RTL8722DM-bsp/Projects/microej/thirdparty/systemview/inc/SEGGER_SYSVIEW_Conf.h"`` (can be something like this "SystemView version: V3.12").
* Go to Segger web page and download the same version V3.12 for SystemView web application (for example `SystemView_V312_Windows_x64 <https://www.segger.com/downloads/systemview/systemview_windows_installer_64>`_).
* Install or unzip the SystemView application and start it.


Receive SystemView logs from the board
--------------------------------------

In UART output:

* Once your application is running, you should see the address of the SEGGER RTT block address printed in your serial terminal (ex: SEGGER_RTT block address: 0x10060834)

In SystemView Application:

* Go to ``Target`` > ``Recorder Configuration``.
* Select ``Jlink`` and click on ``OK``.
* Set ``J-Link Connection`` to ``USB``; ``Target Connection`` to ``CORTEX-M33`` and ``Target Interface`` to ``SWD``.
* In ``RTT Control Block Detection``, select ``Address`` and put the SEGGER RTT block address printed in your serial terminal.
* Click on "Use Advanced Settings" and in J-Link Script File section, navigate to AP2_KM4.JLinkScript file. (i.e. ``"Ameba-RTL8722DM-bsp/Projects/microej/project/EWARM-RELEASE/settings/AP2_KM4.JLinkScript"``).
* Click on ``OK`` to accept changes.
* Now go to ``Target`` > ``Start Recording`` to start system view events recording.


CPU load
========

Usage
-----

The CPU load feature allows to measure the percentage of CPU used by all active applications/activities running on the |VEEPORT|.

To use the CPU load in your project, you need first to enable the CPU load feature in the BSP project by setting the macro ``CPULOAD_ENABLED`` in ``Ameba-RTL8722DM-bsp\projects\microej\core\inc\cpuload_conf.h`` file.

The full build of the BSP project is needed after the activation of the CPU load feature.

Once the feature is enabled, you need to add the MicroEJ Java library ``bspDebug`` to the ``module.ivy`` file of your project.

Here is the ivy dependency line to be added:

	``<dependency org="com.is2t.bsp" name="bspDebug" rev="1.2.2"/>``

This Java library provides the method ``com.is2t.debug.CPULoad.get()`` which allows to retrieve the CPU load measure of an activity.

Here is the suggested way to retrieve the CPU load measure:

	* call ``com.is2t.debug.CPULoad.get()`` at the beginning of the activity you want to track. This first call will reset previous calculated values. The value returned by this first call is not important.
	* call ``com.is2t.debug.CPULoad.get()`` again after you activity is over. The returned value of this second call is the CPU load measure calculated during the execution time of your activity.

Please note that the calculated value will reflect overall CPU load, not only the CPU load generated by the activity measured.

The Java library contains also an embedded test than can be run by calling ``com.is2t.debug.CPULoad.main(args)``.

The test is intended to evaluate activities that take less that 1s to execute.

!! Notice regarding CPU load returning negative or big values !!
----------------------------------------------------------------

In case where the method ``com.is2t.debug.CPULoad.get()`` returns a negative value, or a big value like 7-10 digits instead of a value between 0-100, then it means that CPU load initialization process was not run in needed condition.

The CPU load initialization task must be the only task that runs during the initialization process.

The initialization process must be performed before other tasks in the system start or continue their activity.

To do this, there are in the ``main.c`` file, two implemented functions ``cpuload_suspend_tasks()`` and ``cpuload_resume_tasks()`` which can be used to suspend other tasks when we start CPU load initialization process and resume them at the end of the process.

In the ``cpuload_suspend_tasks()`` function, the MicroJVM task and the task in charge of the WiFi initialization are suspended while we perform CPU load initialization process. These two tasks are resumed in ``cpuload_resume_tasks()`` function.

In case where a new FreeRTOS task is added, this can influence the result of CPU load feature and provide a wrong CPU load value. 

If this new task perform an intensive work from the start-up, you will need to suspend it in ``cpuload_suspend_tasks()`` function.

Make sure you also resume the task in ``cpuload_resume_tasks()`` function.

Please note that the first CPU load measured after the start-up may have a significantly higher value than expected due to the fact that, for CPU load initialization, we have suspended some of the tasks that run at start-up. 

These tasks will be resumed after initialization and can influence the first calculated value.

The CPU load initialization process takes ``~CPULOAD_SCHEDULE_TIME_MS/CPULOAD_INIT_SCHEDULE_FRACTION`` milliseconds. These macros can be found in ``Ameba-RTL8722DM-bsp\projects\microej\core\inc\cpuload.h`` file.



Watchdog timer feature
======================


Description
-----------

Watchdog timer feature allows user to activate a mechanism to prevent look-up in execution.

This feature allows user to add checkpoints in application.
This checkpoint needs to be checked periodically in order to confirm that application is still running as expected.

If a checkpoint is not checked during the watchdog timer period, then the watchdog will be triggered and this will cause a reset of the device.

Please note that the watchdog timer is disabled when the debugger has stopped execution.
See the macro function ```execUserExecutionStopped()`` defined in ``Ameba-RTL8722DM-bsp/sdk/sdk-amebad_v6.2C-RC/component/soc/realtek/amebad/misc/iar_utility/common/preload.mac`` for more information.


BSP Configuration
-----------------

With the macro ``WATCHDOG_TIMER_MODE`` in ``Ameba-RTL8722DM-bsp\projects\microej\watchdog-timer\inc\watchdog_timer_realtek_AmebaD_configuration.h`` you can set the behavior of Watchdog feature when Watchdog timer expires.
If you are in ``WATCHDOG_TIMER_INTERRUPT_MODE`` you can customize the behavior when watchdog timer interrupt triggers. 
To do so, you can modify the function ``watchdog_timer_irq_handler`` in ``Ameba-RTL8722DM-bsp\projects\microej\watchdog-timer\src\watchdog_timer_realtek_amebad_irq.c``.
By default, when the interrupt is running, it prints FreeRTOS task status, prints MicroEJ JVM task status and resets the board.

The full build of the BSP project is needed after the activation.

Enable watchdog FreeRTOS scheduler checkpoint
---------------------------------------------

You can enable a checkpoint that monitors the FreeRTOS scheduler in the BSP. It is a software timer that attests its activity to the watchdog timer regularly.
To activate this feature, uncomment the  line ``#define WATCHDOG_TIMER_FREERTOS_SCHEDULER_CHECKPOINT`` in ``/Ameba-RTL8722DM-bsp/Projects/sdk-amebad_v6.2C-RC/project/realtek_amebaD_va0_example/src/src_hp/main.c``.
Notice that when you activate this checkpoint, the watchdog timer is initialized since it is mandatory in order to start the FreeRTOS scheduler checkpoint.

You must set the Checkpoint period in milliseconds with the macro ``FREERTOS_TIMER_PERIOD_MS`` in ``Ameba-RTL8722DM-bsp\projects\microej\watchdog-timer\inc\watchdog_timer_freertos_configuration.h``.
Be sure to set the ``FREERTOS_TIMER_PERIOD_MS`` with a lower value than the macro ``WATCHDOG_TIMER_TIMEOUT_MS``.

The full build of the BSP project is needed after the activation.

Example of watchdog timer use case: 

* User wants to monitor a new created thread that is performing an important check.
* User will init and start WatchdogTimer feature and request a checkpoint id from Watchdog. 
* This check point will be updated in this new thread periodically using ``checkpoint`` function with the id retrieved in parameter.
* In case the new thread will be blocked at one point, then the checkpoint will not be checked and after the Watchdog timer will expire then a reset will be generated or some debug print based on the state of  ``WATCHDOG_TIMER_MODE`` macro.



