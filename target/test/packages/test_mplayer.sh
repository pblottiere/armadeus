#!/bin/sh

#
# Script to test mplayer's Buildroot package for Armadeus software release
#
#  Copyright (C) 2008-2015 The Armadeus Project
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

source ./test_helpers.sh
source ./test_env.sh

# Requires: Framebuffer & Internet access
# Validates: mplayer
#DEBUG=True
EXEC_NAME="mplayer"
FILE_WEB_ADDRESS="http://www.armadeus.com//assos_downloads/misc/apf27.mpg"
VIDEO_FILE_NAME="$TEMP_DIR/apf27.mpg"
FBDEV=fbdev

platform_specific_stuff()
{
	if [ "$1" == "APF28" ] || [ "$1" == "APF6" ]; then
		FBDEV=fbdev2
	fi
}

# for the end of the test:
remove_files()
{
	if [ "$1" == "APF9328" ]; then
		# We are a little short in memory on APF9328
		rm -f $VIDEO_FILE_NAME
	elif [ "$1" == "APF27" ] || [ "$1" == "APF51" ] || [ "$1" == "APF28" ]; then
		echo
	else
		echo "Platform not supported by this test"
	fi
}

test_mplayer()
{
	show_test_banner "Mplayer"

	is_package_installed $EXEC_NAME

	if [ "$?" != 0 ]; then
		exit_failed
	fi

	wake_up_lcd
	# Stop blinking cursor
	echo 0 > /sys/class/graphics/fbcon/cursor_blink

	if [ ! -f $VIDEO_FILE_NAME ]; then
		# Get video (suppose that network was correctly set before)
		echo "Downloading file from Internet"
		wget $FILE_WEB_ADDRESS -O $VIDEO_FILE_NAME
	        if [ "$?" != 0 ]; then
	                exit_failed
	        fi
	fi

	execute_for_target platform_specific_stuff
	# Launch it
	$EXEC_NAME -vo $FBDEV -nosound -fs $VIDEO_FILE_NAME
	RES=$?
	# Make cursor blink again
	echo 1 > /sys/class/graphics/fbcon/cursor_blink

	if [ "$RES" == 0 ]; then
		ask_user "Was video correctly displayed ? (y/N)"
		execute_for_target remove_files
		if [ "$response" == "y" ] || [ "$response" == "yes" ]; then
			echo_test_ok
			exit 0
		fi
	fi

	exit_failed
}

test_mplayer

