#!/bin/bash

# Get a project name and make folder structure
pushd ..
project=$(basename "$(pwd)")
popd
gCodeFolder=gCode-$project
svgFolder=$gCodeFolder/svgs

mkdir $gCodeFolder

# Svg folders
mkdir $svgFolder
mkdir $svgFolder/drill
mkdir $svgFolder/front
mkdir $svgFolder/back
mkdir $svgFolder/outline

# File names for each gCode program in the milling process
drillOutput=./$gCodeFolder/1-drill_$project.ngc
frontOutput=./$gCodeFolder/2-front_$project.ngc
backOutput=./$gCodeFolder/3-back_$project.ngc
outlineOutput=./$gCodeFolder/4-outline_$project.ngc

frontWaypointed=./$gCodeFolder/2-front_waypointed_$project.ngc
backWaypointed=./$gCodeFolder/3-back_waypointed_$project.ngc

# Convert gerbers to gCode using config in a millproject file (next to script)
pcb2gcode \
  --config millproject \
  --back ../*-B_Cu.gbr \
  --front ../*-F_Cu.gbr \
  --outline ../*-Edge_Cuts.gbr \
  --drill ../*-PTH.drl \
  --config millproject \
  --drill-output $drillOutput \
  --front-output $frontOutput \
  --back-output $backOutput \
  --outline-output $outlineOutput \
  > pcb2gcode_log.txt
 
# Clean up the svgs
mv *_drill*.svg ./$svgFolder/drill/
mv *_front*.svg ./$svgFolder/front/
mv *_back*.svg ./$svgFolder/back/
mv *_outline*.svg ./$svgFolder/outline/

# Copy any SVG board exports from KiCad that show the full board
cp ../*-brd.svg ./$gCodeFolder/

# Alter the comments in the front and back gCode files to create waypoints for restarting after mill stops
sed -e '/^G00 Z/ s/retract/waypoint/' -e 's/( retract )//' -e 's/( rapid move to begin. )//' -e 's/( Mill.*)//' -e 's/( dwell.*)//' $frontOutput \
| awk '{gsub(/\( waypoint \)/, "M3 ; restart from line "NR);print}' > $frontWaypointed

sed -e '/^G00 Z/ s/retract/waypoint/' -e 's/( retract )//' -e 's/( rapid move to begin. )//' -e 's/( Mill.*)//' -e 's/( dwell.*)//' $backOutput \
| awk '{gsub(/\( waypoint \)/, "M3 ; restart from line "NR);print}' > $backWaypointed

# Report 
printf "\n========================================="
printf "\nProject: $project"
printf "\ngCode folder: $gCodeFolder"
printf "\n\nPCB2GCODE Version: $(pcb2gcode -V)"
printf "\n\nngc files output: \n $drillOutput \n $frontOutput \n $backOutput \n $outlineOutput \n $frontWaypointed \n $backWaypointed"
printf "\n=========================================\n\n"
