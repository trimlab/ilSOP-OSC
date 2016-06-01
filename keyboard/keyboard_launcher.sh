#!/bin/bash

rocks run host tile-0-0 command="DISPLAY=tile-0-0:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave -0.5 0 -0.5  -0.25  2 141.219.28.17 9875" &
rocks run host tile-0-1 command="DISPLAY=tile-0-1:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave -0.5 0 -0.25  0     2 141.219.28.17 9875" &
rocks run host tile-0-2 command="DISPLAY=tile-0-2:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave -0.5 0 0       0.25 2 141.219.28.17 9875" &
rocks run host tile-0-3 command="DISPLAY=tile-0-3:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave -0.5 0 0.25   0.5   2 141.219.28.17 9875" &

rocks run host tile-0-4 command="DISPLAY=tile-0-4:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave 0 0.5 -0.5    -0.25 2 141.219.28.17 9875" &
rocks run host tile-0-5 command="DISPLAY=tile-0-5:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave 0 0.5 -0.25   0     2 141.219.28.17 9875" &
rocks run host tile-0-6 command="DISPLAY=tile-0-6:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave 0 0.5 0        0.25 2 141.219.28.17 9875" &
rocks run host tile-0-7 command="DISPLAY=tile-0-7:0.0 /research/emvasey/ilSoP-OSC/keyboard/KeyboardSlave 0 0.5 0.25    0.5   2 141.219.28.17 9875 > /research/emvasey/slave-0-7.txt" &

#./GestureResponseMaster FALSE 10.2.255.255 25884 OutputFile FlagObject ObjectsToTrack

./KeyboardMaster 141.219.28.17 9875 FootR FootL HandR HandL &

read -rsp $'Press any key to continue...\n' -n1 key

pkill KeyboardMaster
rocks run host tile-0-0 command="pkill KeyboardSlave" &
rocks run host tile-0-1 command="pkill KeyboardSlave" &
rocks run host tile-0-2 command="pkill KeyboardSlave" &
rocks run host tile-0-3 command="pkill KeyboardSlave" &
rocks run host tile-0-4 command="pkill KeyboardSlave" &
rocks run host tile-0-5 command="pkill KeyboardSlave" &
rocks run host tile-0-6 command="pkill KeyboardSlave" &
rocks run host tile-0-7 command="pkill KeyboardSlave" &
