# FujiNet Z80 simulation

The repo contains a SimulIDE simulator circuit with an Ardunio Mega to simulate Z80 and ESP signalling to the proposed bus interface.

Platform IO should be used within Visual Code to compile the project which is based on Arduino libraries.

## SimulIDE simulation settings

** SimulIDE Version0.4.15SR10**

To accurately simulate the SPI timing the properties of the simulation need to be edited before running "Simu Step ns" needs to be set to 10. If this is not set then the ESP simulation will see bits missing when reading the Shift registers as the Clock and data signalling slips.

Don't forget to load the Firmware file into the Arduino! PlatformIO compliles the Arduino code to ./.pio/megaatmega2560/firmware.hex, when you click Build (not Build and program unless you are using real hardware!)

