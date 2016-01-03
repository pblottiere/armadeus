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
source ./test_env.sh
BITFILE_DIR="/usr/bin/testsuite/data/fpga"

load_led()
{
	if [ "$1" == "APF9328" ]; then
		load_fpga $BITFILE_DIR/blinking_led_apf9328_200k.bit
		RES=$?
	elif [ "$1" == "APF27" ]; then
		ask_user "Please connect pins 1 & 39 of J20 connector\
				 (3,3V supply for FPGA bank)\nThen press ENTER"
		load_fpga $BITFILE_DIR/blinking_led_apf27_200k.bit
		RES=$?
	elif [ "$1" == "APF51" ]; then
		ask_user "Please power on FPGA Bank 1\nThen press ENTER"
		load_fpga $BITFILE_DIR/top_wishbone_example_apf51_lx9.bin
		RES=$?
		if [ "$RES" == 0 ]; then
			for i in `seq 5`; do
				fpgaregs w 0x08 0x01
				usleep 500000
				fpgaregs w 0x08 0x00
				usleep 500000
			done
		fi
	else
		echo "$1 platform not supported by this test"
		exit 0
	fi
}

load_button()
{
	if [ "$1" == "APF9328" ]; then
		echo "Connect a 10K resistor on FPGA1 pin 1 and put to GND (pin 31) to simulate button"
		load_fpga $BITFILE_DIR/wishbone_example_apf9328_200k.bit
		RES=$?
	elif [ "$1" == "APF27" ]; then
		echo "I hope you have connected pins 1 & 39 of J20"
		load_fpga $BITFILE_DIR/wishbone_example_apf27_200k.bit
		RES=$?
	elif [ "$1" == "APF51" ]; then
		echo "I hope you have powered up Bank 1 with corresponding jumper"
		load_fpga $BITFILE_DIR/top_wishbone_example_apf51_lx9.bin
		RES=$?
	else
		echo "$1 platform not supported by this test"
		exit 0
	fi
}

test_fpga_load()
{
	show_test_banner "FPGA loading"

	execute_for_target load_led
	
	if [ "$RES" == 0 ]; then
		ask_user "Did you see the FPGA's LED blinking ? (y/N)"
		if [ "$response" == "y" ]; then
			echo_test_ok
		else
			echo_test_failed
		fi
	fi
}

test_fpga_it()
{
	show_test_banner "FPGA interrupts"

	execute_for_target load_button
	if [ "$RES" != 0 ]; then
		exit_failed
	fi

	modprobe irq_ocore && modprobe wb_example_irq_mngr && modprobe button_ocore && modprobe wb_example_buttons
	DEVICE_NODE=`cat /proc/devices | grep BUTTON | cut -d " " -f 1`
        if [ "$?" != 0 ] || [ "$DEVICE_NODE" == "" ]; then
                echo "Some modules failed to load"
                exit_failed
        fi
	if [ ! -c /dev/button0 ]; then
		mknod /dev/button0 c $DEVICE_NODE 0
	fi

	/usr/bin/testsuite/testbutton /dev/button0 &
	PID=$!

	if [ "$?" == 0 ]; then
		ask_user "Please press FPGA button. Did you see something on the console ? (y/N)"
		if [ "$response" == "y" ]; then
			echo_test_ok
		else
			echo_test_failed
		fi
	fi
	kill $PID
}

test_fpga_load
test_fpga_it

