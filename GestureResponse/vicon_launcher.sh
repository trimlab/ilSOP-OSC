#!/bin/bash

rocks run host tile-0-0 command="DISPLAY=tile-0-0:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave -0.5 0 -0.5  -0.25  2 FALSE 0" &
rocks run host tile-0-1 command="DISPLAY=tile-0-1:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave -0.5 0 -0.25  0     2 FALSE 1" &
rocks run host tile-0-2 command="DISPLAY=tile-0-2:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave -0.5 0 0       0.25 2 FALSE 2" &
rocks run host tile-0-3 command="DISPLAY=tile-0-3:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave -0.5 0 0.25   0.5   2 FALSE 3" &

rocks run host tile-0-4 command="DISPLAY=tile-0-4:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave 0 0.5 -0.5    -0.25 2 FALSE 4" &
rocks run host tile-0-5 command="DISPLAY=tile-0-5:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave 0 0.5 -0.25   0     2 FALSE 5" &
rocks run host tile-0-6 command="DISPLAY=tile-0-6:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave 0 0.5 0        0.25 2 FALSE 6" &
rocks run host tile-0-7 command="DISPLAY=tile-0-7:0.0 /research/emvasey/ilSoP-OSC/GestureResponse/GestureResponseSlave 0 0.5 0.25    0.5   2 FALSE 7" &

#./GestureResponseMaster FALSE 10.2.255.255 25884 OutputFile FlagObject ObjectsToTrack

./GestureResponseMaster FALSE 141.219.28.17 9875 testoutput.txt Wand FootR FootL HandL HandR
