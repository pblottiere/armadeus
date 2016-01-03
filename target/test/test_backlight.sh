#!/bin/sh

#
# Script to test Armadeus Software release
#
#  Copyright (C) 2008-2015 The Armadeus Project - Armadeus Systems
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

source ./test_helpers.sh

SYS_DIR=/sys/class/backlight/imx-bl
min_bright=0
mid_bright=50
LOOP=`seq 1 5 150`
LOOP_DELAY=100000

load_driver()
{
	if [ "$1" == "APF9328" ] || [ "$1" == "APF27" ]; then
                if [ "$LINUX_VERSION" == "2.6.29" ]; then
			modprobe imx_bl
		else
			SYS_DIR=/sys/class/backlight/imxfb-bl
		fi
	elif [ "$1" == "APF51" ]; then
		echo "Not supported yet"
		exit 0
	elif [ "$1" == "APF28" ]; then
		if [ "$LINUX_VERSION" == "2.6.35" ]; then
			SYS_DIR=/sys/class/backlight/mxs-bl
		else
			SYS_DIR=/sys/class/backlight/backlight
			min_bright=1
			mid_bright=4
			LOOP=`seq 1 1 8`
			LOOP_DELAY=500000
		fi
	elif [ "$1" == "APF6" ]; then
		SYS_DIR=/sys/class/backlight/backlight_lcd/
		min_bright=7
		mid_bright=4
		max_bright=0	# inverted polarity
		LOOP=`seq 1 1 8`
		LOOP_DELAY=500000
	else
		echo "Platform not supported by this test"
		exit 0
	fi
}

test_backlight()
{
	show_test_banner "Backlight"
	echo 0 > /sys/class/graphics/fb0/blank   # Leave FB sleep mode
	# Paint it white:
	clear > /dev/tty1
	cat /dev/zero | tr '\0' '\377' | dd of=/dev/fb0 bs=1K count=750 2>/dev/null

	if [ "$1" != "" ]; then
		SYS_DIR=/sys/class/backlight/$1
	else
		execute_for_target load_driver
		if [ "$?" != 0 ]; then
			echo "module not found"
			exit_failed
		fi
	fi
	current=`cat $SYS_DIR/actual_brightness`
	if [ "$max_bright" == "" ]; then
		max_bright=`cat $SYS_DIR/max_brightness`
	fi
	echo "Current brightness level: $current / $max_bright"

	echo $mid_bright > $SYS_DIR/brightness
	ask_user "Backlight set to MIDDLE, press ENTER"
	echo $max_bright > $SYS_DIR/brightness
	ask_user "Backlight set to MAX, press ENTER"
	echo $min_bright > $SYS_DIR/brightness
	ask_user "Backlight set to MIN, press ENTER"
	echo $current > $SYS_DIR/brightness

	ask_user "I will now loop through brightness, press ENTER when ready"
	for loop in $LOOP; do
		if [ "$loop" -lt "$max_bright" ]; then
			usleep $LOOP_DELAY
			echo $loop > $SYS_DIR/brightness
			echo -n "."
		fi
	done
	echo $current > $SYS_DIR/brightness
	ask_user "Did the test succeed ? (y/N)"
	if [ "$response" == "y" ] || [ "$response" == "yes" ]; then
		echo_test_ok
		exit 0
	fi
	exit_failed
}

test_backlight $1

