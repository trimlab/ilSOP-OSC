# ilSOP-OSC
This project requires the following libraries:

  * Boost 1.5.3 - http://www.boost.org/users/history/
  * Vicon DataStream SDK - https://www.vicon.com/downloads/utilities-and-sdks/datastream-sdk/vicon-linux-datastream-sdk-15
  * Oscpack - https://code.google.com/archive/p/oscpack/downloads

To run the simulations, SSH into ivs.research.mtu.edu by running `ssh -Y ivs.research.mtu.edu`, go into the ilSoP-OSC directory, and run "./launcher.sh <name>".

Here are the simulations the launcher supports:
  - Gesture Artwork: `./launcher.sh gestureartwork`
  - Keyboard: `./launcher.sh keyboard`
  - World Map: `./launcher.sh worldmap`

###Info for Gesture Artwork
When GestureArtwork is run, it sends two OSC messages to the Windows computer (at IP 141.219.28.17).
One is sent on port 6448, and is formatted as follows:
`<vicon_object_name>/<position/velocity/acceleration>/ X Y Z`

The other is a list of all the positions sent on port 6449, formatted as follows:
`positions/ X1 Y1 Z1 X2 Y2 Z2 ... Xn Yn Zn`


