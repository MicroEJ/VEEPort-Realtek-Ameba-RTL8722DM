..
  Copyright 2019-2023 MicroEJ Corp. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be found with this software.

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

.. _README MicroEJ BSP: ./Ameba-RTL8722DM-bsp/README.rst
.. _RELEASE NOTES: ./RELEASE_NOTES.rst
.. _CHANGELOG: ./CHANGELOG.rst

.. _release-notes:

========================================================
|VEEPORT| Release Notes for |MANUFACTURER| |BOARD_NAME|
========================================================

Description
===========

This is the release notes of the |VEEPORT| for |BOARD_NAME|.

Versions
========

|VEEPORT|
---------

|VEEPORT_VER|

Dependencies
------------

This |VEEPORT| contains the following dependencies:

.. list-table::
   :header-rows: 1
   
   * - Dependency Name
     - Version
   * - Architecture (flopi4I35)
     - 7.16.0
   * - NET Pack
     - 10.4.0
   * - NET-ADDONS Pack
     - 2.5.3
   * - FS Pack
     - 5.1.2
   * - WATCHDOG TIMER Pack
     - 2.0.1

Please refer to the |VEEPORT| `module description file <./Ameba-RTL8722DM-configuration/module.ivy>`_ 
for more details.

Board Support Package
---------------------

- BSP provider: |MANUFACTURER| (``sdk-amebad``)
- BSP version: v6.2C-RC, patch_221206_bf3b2a72+patch_mbedtls-2.27.0_221230_v01

Please refer to the |MANUFACTURER| ``sdk-amebad`` GitHub git repository
available `here
<https://github.com/ambiot/ambd_sdk>`__.

Third Party Software
--------------------

Here is a list of the most important ones:

.. list-table::
   :widths: 3 3 3

   * - RTOS 
     - FreeRTOS
     - 10.2.0
   * - TCP/IP stack 
     - LwIP
     - 2.0.2
   * - Cryptographic stack 
     - Mbed TLS
     - 2.27.0
   * - File System stack 
     - FatFS
     - R0.10c


Features
========

Network
-------

|VEEPORT| features a network interface with Wi-Fi as an
underlying hardware media.  A limited number of 10 sockets could be
used for TCP connections, 5 for TCP listening (server) connections
and 6 for UDP connections. A DHCP client can be activated to retrieve
a dynamic IP address. All DNS requests can be handled by a MicroEJ
software implementation or a native one.


SSL
-------

|VEEPORT| features a network secure interface. Available
secured protocols are  SSL 3.0, TLS 1.0, TLS 1.1, TLS 1.2. Supported
keys and certificates formats are PKCS#5 and PKCS#12, PEM or DER encoded.

File System
-----------

|VEEPORT| features a file system interface. The file system uses the
internal flash memory of the board. Up to 2 files can be opened simultaneously.



Known issues/limitations
========================

- On NET-LWIP: Enable LWIP_IPV6 causes build issue in net-lwip cco (specifically build error in lwip_util.c file).
  The code in this file uses ipv4 structs and it assumes that only IPV4 is enabled.

- On Windows, the toolchain doesn't support long path.  The build
  directory (named ``target~/``) can be moved closer to the root
  filesystem with the ``target`` property.  For example, set
  ``target=C:/tmp/`` in
  ``Ameba-RTL8722DM-configuration/module.properties``.



|VEEPORT| Memory Layout
=======================

Memory Sections
---------------

IAR linker configuration files provide memory section information and the memory layout. They can be found in the following directory: 
``xxx/Ameba-RTL8722DM-bsp/Projects/microej/project/EWARM-RELEASE/``
Each memory section is discribed in the IAR linker configuration file ``rtl8721dhp_image2_is.icf``.

The memory layout is stored in ``rtl8721d_memory_layout_is.icf``.

Memory Layout
-------------

.. list-table::
   :header-rows: 1
   
   * - Section Content
     - Section Source
     - Section Destination
     - Memory Type
   * - MicroEJ Application static
     - ``.bss.soar``
     - ``.ram_image2.bss``
     - internal SRAM
   * - MicroEJ Application threads stack blocks 
     - ``.bss.vm.stacks.java``
     - ``.ram_image2.bss``
     - internal SRAM
   * - MicroEJ Core Engine internal heap 
     - ``ICETEA_HEAP``
     - ``.ram_image2.bss``
     - internal SRAM
   * - MicroEJ Application heap 
     - ``_java_heap``
     - ``PSRAM_NS_region``
     - internal PSRAM
   * - MicroEJ Application Immortal Heap 
     - ``_java_immortals``
     - ``PSRAM_NS_region``
     - internal PSRAM
   * - MicroEJ Application resources 
     - ``.rodata.resources``
     - ``.rodata``
     - SPI Flash
   * - MicroEJ System Applications code and resources 
     - ``.rodata.soar.features``
     - ``N/A``
     - N/A
   * - MicroEJ System Application statics 
     - ``.bss.features.installed``
     - ``N/A``
     - N/A
   * - MicroEJ Shielded Plug data 
     - ``.shieldedplug``
     - ``N/A``
     - N/A
   * - MicroEJ Application and Library code 
     - ``.text.soar``
     - ``.rodata``
     - SPI Flash


Please also refer to the MicroEJ docs website page available `here
<https://docs.microej.com/en/latest/PlatformDeveloperGuide/coreEngine.html#link>`__
for more details.

