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
source ./test_env.sh

load_driver()
{
	if [ "$1" == "APF9328" ] || [ "$1" == "APF27" ]; then
		modprobe tsc2102_ts
		sleep 1
		dmesg | tail | grep "TSC210x Touchscreen driver initialized"
		RES="$?"
	elif [ "$1" == "APF51" ]; then
		# driver is in-kernel/static
		grep "WM831x touchscreen" $(echo /sys/class/input/event*/device/name)
		RES="$?"
	elif [ "$1" == "APF28" ]; then
		# driver is in-kernel/static
		if [ "$LINUX_VERSION_2" == "2.6" ]; then
			dmesg | grep "MXS touch"
			RES="$?"
		else
			dmesg | grep "input: mxs-lradc"
			RES="$?"
		fi
	elif [ "$1" == "APF6" ]; then
		# driver is in-kernel/static
		grep "SX8674 Touchscreen" $(echo /sys/class/input/event*/device/name)
		RES="$?"
	else
		echo "$1 platform not supported by this test"
	fi
}

test_touchscreen()
{
	show_test_banner "Touchscreen"

	execute_for_target load_driver

	if [ "$RES" != 0 ]; then
		echo "Hardware not found !"
		exit_failed
	fi

	if [ ! -e "/etc/ts.conf" ]; then
		echo "Tslib misconfigured"
		exit_failed
	fi

	ts_finddev /dev/input/event0 10
	if [ "$?" != 1 ]; then
		echo "Can not communicate with Touchscreen"
		exit_failed
	fi

	echo "You can now calibrate the Touch..."
	sleep 1
	ts_calibrate

	echo_test_ok
}

test_touchscreen
