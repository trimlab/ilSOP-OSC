#!/usr/bin/env bash

case "$1" in
	'phase3')
		cd gestureartwork
		./vicon_launcher.sh
	;;

	'phase2')
		cd keyboard
		./keyboard_launcher.sh
	;;

	'phase1')
		cd worldmap
		./worldmap_launcher.sh
	;;

esac
