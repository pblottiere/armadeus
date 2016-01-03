#!/bin/sh

#
# Script to test a Buildroot package for Armadeus Software release
#
#  Copyright (C) 2011 The Armadeus Project
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

source ./test_helpers.sh
source ./test_env.sh

EXEC_NAME="fpgaregs"

blink_led()
{
	if [ "$1" == "APF9328" ]; then
		load_fpga ./data/fpga/wishbone_example_apf9328_200k.bit
		RES=$?
	elif [ "$1" == "APF27" ]; then
		echo "I hope you have connected pins 1 & 39 of J20"
		load_fpga ./data/fpga/wishbone_example_apf27_200k.bit
		RES=$?
	elif [ "$1" == "APF51" ]; then
		echo "I hope you have powered up Bank 1 with corresponding jumper"
		load_fpga ./data/fpga/top_wishbone_example_apf51_lx9.bin
		RES=$?
	else
		echo "Platform not supported by this test"
	fi

	if [ "$RES" == 0 ]; then
		for i in `seq 5`; do
			fpgaregs w 0x08 0x01
			usleep 500000
			fpgaregs w 0x08 0x00
			usleep 500000
		done
	fi
}

test_fpgaregs()
{
	show_test_banner "fpgaregs"
	is_package_installed $EXEC_NAME

	execute_for_target blink_led
	
	if [ "$RES" == 0 ]; then
		ask_user "Did you see the FPGA's LED blinking ? (y/N)"
		if [ "$response" == "y" ]; then
			echo_test_ok
		else
			echo_test_failed
		fi
	fi
}

test_fpgaregs

