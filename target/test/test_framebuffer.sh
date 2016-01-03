#!/bin/sh

#
# Script to test Armadeus Software release
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

EXEC_NAME="fb-test-perf"

test_framebuffer()
{
	show_test_banner "Framebuffer"
	echo 0 > /sys/class/graphics/fb0/blank   # Leave FB sleep mode
	# Stop blinking cursor
	echo 0 > /sys/class/graphics/fbcon/cursor_blink

	is_package_installed $EXEC_NAME
	$EXEC_NAME 0 fb-test-perf.log
	if [ "$?" != 0 ]; then
		exit_failed
	fi

	EXEC_NAME="fb-test"
	is_package_installed $EXEC_NAME
	$EXEC_NAME &
	PID=$!
	sleep 1

	# Reactivate cursor
	echo 1 > /sys/class/graphics/fbcon/cursor_blink
	# Ask user for feedbacks
	if [ "$PID" != "" ]; then
		ask_user "Did all the tests show fine ? (y/N)"
		if [ "$response" == "y" ] || [ "$response" == "yes" ]; then
			echo_test_ok
			kill $PID
			exit 0
		fi
		kill $PID
	fi
	exit_failed
}

test_framebuffer
