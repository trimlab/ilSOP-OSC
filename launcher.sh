#!/usr/bin/env bash

case "$1" in
	'gestureartwork')
		cd gestureartwork
		./vicon_launcher.sh
	;;

	'keyboard')
		cd keyboard
		./keyboard_launcher.sh
	;;

	'worldmap')
		cd worldmap
		./worldmap_launcher.sh
	;;

esac
