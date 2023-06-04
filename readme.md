# FujiNet Z80 simulation

The repo contains a SimulIDE simulator circuit with an Ardunio Mega to simulate Z80 and ESP signalling to the proposed bus interface.

Platform IO should be used within Visual Code to compile the project which is based on Arduino libraries.

## SimulIDE simulation settings

**SimulIDE Version0.4.15SR10**

To accurately simulate the SPI timing the properties of the simulation need to be edited before running. The "Simu Step ns" needs to be set to 10 in SimulIDE (right click empty diagram space, select properties from pop up menu). If simulation step time is not set correctly, then the ESP simulation will see bits missing when reading the Shift registers as the Clock and data signalling slips.

Don't forget to load the Firmware file into the Arduino! PlatformIO compiles the Arduino code to ./.pio/megaatmega2560/firmware.hex, when you click Build (not Build and program unless you are using real hardware!)



## KiCad ##

**KiCad version 7**

Going forward there are going to be different ways that the modules can be implemented and built. Therefore, a set of KiCad projects are in a modules folder, and then there is a folder for libraries for symbols/footprints; these are the building blocks. 

Each module project should have 2 schematics. The main project schematic has Pin headers etc. for a (test) PCB and a hierarchical schematic (not a project just a file) that contains the module's circuit. The module's hierarchical schematic file should not contain any connectors or "PCB stuff" just the parts needed for its function. So when making an adaptor the nested hierarchical schematic can be imported this hierarchical schematic without the extra "PCB stuff" cluttering the adaptor project PCBs. Module projects do not need a PCB design as they are imported into other projects that implement the PCB design and associated connectors etc. However, I like the idea of the modules being projects which have a PCB design to create a physical implementation for testing. In time all the modules could follow some common standard for PCB implementation these could be built and wired up ad-hoc.

A reference design is in the root KiCad folder, this shows how everything should connect up to make an adapter. There shouldn't really be a PCB for the reference design as the PCB design can (and did) influence the schematic. The reference design should be a pure, readable and simple expression of the requirements. So going forward a KiCad project should be created for the adaptor being built (in the adaptors folder), a simple way to copy all the settings is to save as from a similar project (e.g. template project). Now the modules schematics can be added to the adaptor project hierarchical schematics, add their hierarchical pins and wire/label as required.

If an adaptor uses a daughter board, then sub-projects in the adapter project need to be created (so they have their own PCB design). The sub-project is imported to the adaptor's main project as a hierarchical schematic so that the Electronic Rule Checker (ERC) can be used to validate the design.  Don't try to mark one-eye ball an adaptor design there are multiple busses and logic signals, and it's not far off a small computer; ERC is a valuable tool so use it. Another trick is to make use of the BUS net Class directives which apply styling to the connected bus, if a connection is missing there is no styling applied. I like to make it easy to see when things aren't connected as sometimes the ERC won't report a problem (more than one pin is connected makes it valid, even if it should be 3 pins connected).

## PCB fabrication with a 3018 CNC mill

The software pipeline:
- KiCad PCB design (export Gerber and Drill files)
- pcb2gcode (CAM, gerber to gcode)
- Universal Gcode Sender (runs the CNC mill controller)

The size and accuracy of the mill determine the PCB design rules to be used in the KiCad board setup. 

After trial and error, the following minimum limits have been found. A 0.5mm end mill (cutter) requires at a minimum, 0.4mm tracks and 0.6mm clearances. (This may change with pcb2gcode and some "git-gud"). With a 200-watt spindle motor, the following speeds and feeds for drilling and milling with a 0.5mm end mill.

Drilling
- Z cut -2mm
- Z Feedrate 30
- Spindle Speed 600 (~60% power)

Milling
- Z cut -0.15mm (board thickness varies)
- X-Y Feedrate 60 (20 or 40)
- Z Feedrate 30
- Spindle speed 800 (~80% power)

Going slower seems to be better for accuracy and success. Running the milling at max spindle speed and -1.5 cut resulted in a snapped 0.5mm end mill. Also, there is a lot of noise/vibration which makes the cutting just sound wrong.

The choice of tool for cutting the isolation paths seems to be split between V-bits and tiny straight-end mills. V-bits have a tip as narrow as 0.1mm but widen based on the angle (degrees) of the V. Assuming a perfectly flat board of even thickness, the tip + width at depth (0.1mm) isolation paths can be cut; assume a width of ~0.2mm approximately. However, perfect doesn't exist, the bit moves as Z height but the surface height changes (PCB thickness, bowing etc.). Therefore, the cut width changes as the V shape angle produces wider cuts as the tip moves deeper into the material. An end mill has a straight edge so cuts consistently at all depths below the surface. The only concern is cutting too deep and breaking the end mill, which is why a 0.5mm bit is used. End mills or Ball cutters go down to 0.2mm, but breakage risks increase. There is a similar problem with V bits, a 20-degree angle has a weaker tip than a 60-degree angle V bit. A wider angle V bit cut widens quicker with slight changes in depth. There is no perfect tool, just compromises and different workflows.

To counter problems with Vbits and depth of cut, Universal Gcode Sender has a plugin called "AutoLeveler". This plugin probes the height of the PCB at some given intervals. A height map is generated and the Z positions in the Gcode are modified. Thus, adding to the process of milling per side, probing pass and transformation of gcode. The accuracy of the trace is subject to the resolution of the probing, high accuracy means probing more points (long time). To conclude, there is room to achieve thinner track and clearance but for now, a simpler reliable approach trumps mastery of PCB milling.

Milling two-sided PCBs require the board to be flipped. This is an immediate alignment challenge which needs some thought. There are limitations imposed by CAM software features and the size of the CNC mill. Typically, the method seems to be prescribed by the CAM software. FlatCAM does inbox flips, the board is in the same place on the bed. To do this FlatCAM provides support to flip/mirror one of the Gerbers, alignment drill holes can be easily added. However, FlatCAM is too manual and buggy. 

Pcb2gcode enables "millproject" config files to be set up and reused between projects. The flip method used is a page turn by default, which requires the bed to be twice the size of the PCB. With a small bed and a big board, an in-place flip can be achieved by re-zeroing the origin on the X-axis after the flip.  Ideally, a jig should be used to maintain X-Y alignment or alternatively a pair of alignment holes evenly spaced from the centre lines. To make the flip alignment accurate an alignment hole can be drilled at 0,0 on the front; when flipped re-align to the hole and reset X 0. If you don't want to make holes in a PCB that is cut accurately to size, then zero on the corners (with the same tool fitted each time).

### KiCad PCB design/settings

Before laying out the PCB you need to set up the board settings, Design Rules and Net Classes. these should be set based on the achievable tollerances of the milling process.

Auto routing (FreeRouting) ignores the Design Rules, it only seems to follow NetClass rules, Edges and Margins (Rule areas on the margin layer). Therefore, you should fully define the board before placing components!

The process sets are:
Configure Design Rules and Net Classes
Draw an Edge box to the size of the PCB
Set Origin (file/drill/sheet) in the top left of the Edge box - place a *VIA on this spot
Create Margin boxes (+2mm) on the edges (Rule Area)
Layout components
Route tracks
Export Gerber and drill files (include the origins)
Export SVG as a CNC reference. F.Cu, B.Cu, User.Drawings, Edge.Cuts, F.Fab, B.Fab

*Directly placing a VIA on the Origin is blocked if it is in the margin. To get round this place the VIA outside the margin and move it to the Origin, this is permitted. Else you could adjust the shape of the marging and expose the corner.

Your PCB should have an Edge box the size of the PCB (stock or cut out). Marked out margins for Freerouting, 2mm seems reasonable. The Origin for sheets and files needs to be set at the top left of the Edge box. Origin can be corrected in pcb2gcode and milling just saves effort to do it right. Export to Gerber (Fabrication output), make sure "Use drill/place file origin" is checked. Hit Plot and Generate Drill Files. If you want to be tidy, export only the F.Cu, B.CU, and Edge_Cuts. You have a PCB file set to run through pcb2gcode.

The KiCad process is done!

### pcb2gcode CAM 

A "millproject" needs to be configured which enables milling a PCB with the Design Rules defined in KiCad. Yep, this is a circular dependency! 

Each Gerber needs to be addressed directly in the millproject file. Given the KiCad naming scheme, a bash script with a command line config can be used to wildcard the files. E.g. "pcb2gcode --front=*-F_Cu.gbr".

Read the wiki to create the millproject file https://github.com/pcb2gcode/pcb2gcode/wiki/Manual

Examples can be found in the pcb2gcode folder.

### Milling procedure

Set up a jig on the bed to hold the PCB.

Start with the front processes, which all have positive X co-ordinates

**Alignment**
- Align the mill and zero on the origin TOP-LEFT (same as KiCad)
- Use the PCB corner (marker pen) OR drill an alignment hole for the origin

**Drilling - front**
- Use drill bits that are the same length butted up in the collet, save  zeroing the Z axis per bit.
- Fit the first Tool (drill bit) in drill.ngc
- Touch the bit on the surface of the board and set Z to zero
- Run the drill.ngc
- Change bits as prompted.

**Milling - front**
- Fit the 0.5mm end mill
- Touch bit on the PCB surface and 0 Z axis
- Run Front.ngc

**Flip on Y (left to right)**
- The default flip/mirror axis in pcb2gcode is along the left edge of the - PCB, "turning a page in a book".
- Measure the X-axis length, move the mill from 0,0 lenght+X
- The mill should now be aligned to the TOP-RIGHT corner, opposite the origin used for the front.
- Tune positioning
- RESET X to 0

All cuts on the Back.ngc have a negative X coordinate so the mill runs from right to left.

**Milling - back**
- Fit the 0.5mm end mill
- Touch bit on the PCB surface and 0 Z axis
- Run Back.ngc

**Cut out - optional**
- Fit a cutting end mill (3.127mm)
- Touch bit on the PCB surface and 0 Z axis
- Run Outline.ngc
