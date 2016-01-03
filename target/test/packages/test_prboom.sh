#!/bin/sh

#
# Script to test a Buildroot package for Armadeus Software release
#
#  Copyright (C) 2008-2012 The Armadeus Project
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

source ./test_helpers.sh
source ./test_env.sh

# Validates: prboom & SDL_net & SDL_mixer
# Requires: SDL & ALSA OSS emulation
EXEC_NAME="/usr/games/prboom"

set_exec_options()
{
	if [ "$1" == "APF51" ]; then
		EXEC_OPTIONS="-width 640 -height 480 -vidmode 16bit &"
	else
		EXEC_OPTIONS="-width 320 -height 240 &"
	fi
}

test_prboom()
{
	show_test_banner "PrBoom (SDL port of Doom)"

	is_package_installed $EXEC_NAME

	if [ "$?" != 0 ]; then
		exit_failed
	fi

	execute_for_target set_exec_options

	# Launch it
	ask_user "After having press ENTER, PrBoom will be launched. Press ENTER at any time in serial console to stop."
	modprobe snd-pcm-oss
	modprobe snd-mixer-oss
	$EXEC_NAME $EXEC_OPTIONS &
	PID=$!
	read
	kill $PID
	sleep 1

	ask_user "Did you have good time ;-) ? (y/N)"
	if [ "$response" == "y" ] || [ "$response" == "yes" ]; then
		echo_test_ok
		exit 0
	fi

	exit_failed
}

test_prboom

