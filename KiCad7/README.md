# KiCad projects

Folders:

**Adapters** projects to make FujiNet adapters for different machines.

**Modules/Base_Schematics** schematics to import (copy) into implementations/adapters for common functions e.g. SPI bus.

**Modules/Implementations** modules implemented as stand-alone PCBs with connectors

**ReferenceDesign** is an example of a completed adapter (schematic) for a ZX spectrum. This is the current concept design.

**Fabrication** Folder for preparing for CNC fabrication

**Production-ready** Adapter/Modules that have been fabricated and tested, working designs for production.

## New projects and path management

Some KiCad referencing information because managing paths is a tad annoying. Use relative pathing if you want anyone to open something you made. To do this all paths should start ${KIPRJMOD} which is the environment variable for the project folder ../ will take you back one layer as expected.

If you want to start a new Module or Adapter then you should open the template project and SAVE AS "name". This will copy the template and setting for you, and then there is less to set up.  Setting up from scratch requires libraries importing, Net classes, buses, PCB settings etc set up.

### Symbols

Name: esp32-wrover
Path: ${KIPRJMOD}/../../FujiNet_KiCadLibs/esp32-wrover-kicad-master/esp32-wrover.lib

Name: FujiNet_Memory_RAM
Path: ${KIPRJMOD}/../../FujiNet_KiCadLibs/FujiNet_Memory_RAM.kicad_sym

### Footprints
Name: esp32-wrover
Path: ${KIPRJMOD}/../../FujiNet_KiCadLibs/esp32-wrover-kicad-master/esp32-wrover.pretty