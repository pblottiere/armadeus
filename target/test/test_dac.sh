#!/bin/sh

#
# Script to test Armadeus Software release
#
#  Copyright (C) 2008-2015 The Armadeus Project - Armadeus systems
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

source ./test_helpers.sh
#source ./test_env.sh

check_platform()
{
	if [ "$1" == "APF28" ] || [ "$1" == "APF6" ]; then
		echo "Platform not supported by this test"
		exit 0
	fi
}

apf9328_27_setdac()
{
	echo "Setting $1 channels to $2"
	setDAC $1 $2
}

apf51_setdac()
{
	echo "Setting 0 & 1 channels to $1"
	echo $1 > out0_raw
	echo $1 > out1_raw
}

do_test()
{
	if [ "$1" == "APF9328" ] || [ "$1" == "APF27" ]; then
		apf9328_27_setdac AB 500
		if [ "$?" == 0 ]; then
			sleep 2
			apf9328_27_setdac AB 250
			sleep 2
			apf9328_27_setdac AB 0
			echo_test_ok
		else
			echo "Hardware not found !"
		fi
	elif [ "$1" == "APF51" ]; then
		modprobe mcp49x2
		cd /sys/bus/spi/devices/spi0.1/device0
		ls
		apf51_setdac 250
		sleep 2
		apf51_setdac 500
		sleep 2
		apf51_setdac 250
		sleep 2
		apf51_setdac 0
		echo_test_ok
	fi
}

test_DAC()
{
	show_test_banner "DAC"
	execute_for_target check_platform
	ask_user "This test is more probant with vumeters. Connect them and press ENTER."
	execute_for_target do_test
}

test_DAC

