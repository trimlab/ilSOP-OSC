#!/usr/bin/env bash
#./chromium ViconRelay 141.219.28.17 9875 worldmapoverlaid.jpg Wand

#!/bin/bash

rocks run host tile-0-0 command="DISPLAY=tile-0-0:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave -0.5 0 -0.5  -0.25  141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &
rocks run host tile-0-1 command="DISPLAY=tile-0-1:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave -0.5 0 -0.25  0     141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &
rocks run host tile-0-2 command="DISPLAY=tile-0-2:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave -0.5 0 0      0.25  141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &
rocks run host tile-0-3 command="DISPLAY=tile-0-3:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave -0.5 0 0.25   0.5   141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &

rocks run host tile-0-4 command="DISPLAY=tile-0-4:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave 0 0.5 -0.5    -0.25 141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &
rocks run host tile-0-5 command="DISPLAY=tile-0-5:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave 0 0.5 -0.25   0     141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &
rocks run host tile-0-6 command="DISPLAY=tile-0-6:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave 0 0.5 0       0.25  141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &
rocks run host tile-0-7 command="DISPLAY=tile-0-7:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave 0 0.5 0.25    0.5   141.219.28.17 9875 /research/emvasey/ilSoP-OSC/worldmap/worldmapoverlaid.jpg Wand" &

#./GestureResponseMaster FALSE 10.2.255.255 25884 OutputFile FlagObject ObjectsToTrack

./ViconRelayMaster 10.2.255.255 25884 Wand


#./GestureResponseMaster FALSE 10.2.255.255 25884 OutputFile FlagObject ObjectsToTrack

#./GestureResponseMaster FALSE 141.219.28.17 9875 testoutput.txt Wand FootR FootL HandL HandR

read -rsp $'Press any key to continue...\n' -n1 key

rocks run host tile-0-0 command="pkill ViconRelaySlave" &
rocks run host tile-0-1 command="pkill ViconRelaySlave" &
rocks run host tile-0-2 command="pkill ViconRelaySlave" &
rocks run host tile-0-3 command="pkill ViconRelaySlave" &
rocks run host tile-0-4 command="pkill ViconRelaySlave" &
rocks run host tile-0-5 command="pkill ViconRelaySlave" &
rocks run host tile-0-6 command="pkill ViconRelaySlave" &
rocks run host tile-0-7 command="pkill ViconRelaySlave" &
