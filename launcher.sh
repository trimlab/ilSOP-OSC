#!/usr/bin/env bash

case "$1" in
	'gestureartwork')
		cd GestureResponse
		./vicon_launcher.sh
	;;

	'keyboard')
		cd keyboard-chromium-ver
		./keyboard_launcher.sh
	;;

	'worldmap')
		cd vicon-relay
		./worldmap_launcher.sh
	;;

esac
