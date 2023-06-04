#!/bin/sh
# Generate front+back mill + cut gcode
#
mkdir gCode
mkdir svgs
pcb2gcode \
  --back ../*-B_Cu.gbr \
  --front ../*-F_Cu.gbr \
  --outline ../*-Edge_Cuts.gbr \
  --drill ../*-PTH.drl \
  --config millproject

mv *.ngc ./gCode/
mv *.svg ./svgs/


  
