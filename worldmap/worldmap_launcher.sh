#!/usr/bin/env bash

rocks run host tile-0-0 command="DISPLAY=tile-0-0:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-0-3.jpg" &
rocks run host tile-0-1 command="DISPLAY=tile-0-1:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-0-2.jpg" &
rocks run host tile-0-2 command="DISPLAY=tile-0-2:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-0-1.jpg" &
rocks run host tile-0-3 command="DISPLAY=tile-0-3:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-0-0.jpg" &

rocks run host tile-0-4 command="DISPLAY=tile-0-4:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-1-3.jpg" &
rocks run host tile-0-5 command="DISPLAY=tile-0-5:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-1-2.jpg" &
rocks run host tile-0-6 command="DISPLAY=tile-0-6:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-1-1.jpg" &
rocks run host tile-0-7 command="DISPLAY=tile-0-7:0.0 /research/emvasey/ilSoP-OSC/worldmap/ViconRelaySlave /research/emvasey/ilSoP-OSC/worldmap/images/worldmapoverlaid-1-0.jpg" &

./ViconRelayMaster 141.219.28.17 9875 Wand &

read -rsp $'Press any key to continue...\n' -n1 key

pkill ViconRelayMaste
rocks run host tile-0-0 command="pkill ViconRelaySlave" &
rocks run host tile-0-1 command="pkill ViconRelaySlave" &
rocks run host tile-0-2 command="pkill ViconRelaySlave" &
rocks run host tile-0-3 command="pkill ViconRelaySlave" &
rocks run host tile-0-4 command="pkill ViconRelaySlave" &
rocks run host tile-0-5 command="pkill ViconRelaySlave" &
rocks run host tile-0-6 command="pkill ViconRelaySlave" &
rocks run host tile-0-7 command="pkill ViconRelaySlave" &
