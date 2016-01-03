#!/bin/sh

#
# Script to test Armadeus Software release
#
#  Copyright (C) 2010-2011 The Armadeus Project
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

source ./test_helpers.sh

DEFAULT_PIN_CODE="0000"
DEFAULT_PHONE_NUMBER="0954000000"
GSM_DEVICE=/dev/ttySMX1

send_at_cmd()
{
#	echo "($1)"
	echo -e -n "$1\015" > $GSM_DEVICE
	sleep 1
}

exit_failed_gsm()
{
	killall cat 2>/dev/null
	send_at_cmd ATE1	# ECHO ON
	exit_failed
}

prepare_gsm()
{
	if [ "$1" == "APF9328" ]; then
		echo
	elif [ "$1" == "APF27" ]; then
		echo
	elif [ "$1" == "APF51" ]; then
		ask_user "Please powerup FPGA's bank 3 AND put Wireless jumper\nThen press ENTER"
		load_fpga /lib/firmware/fpga/apf51_gsm_gps_firmware.bin
		modprobe irq_ocore
		modprobe 8250
		modprobe board_irq_mng
		modprobe 16750_ocore
		# bring GSM out of reset:
		modprobe wm831x-gpio
		echo 245 > /sys/class/gpio/export
		echo out > /sys/class/gpio/gpio245/direction
		echo 1 > /sys/class/gpio/gpio245/value
		ls -al /dev/ttyS*
		GSM_DEVICE=/dev/ttyS0
		GSM_SPEED=115200
	elif [ "$1" == "APF6" ]; then
		echo 205 > /sys/class/gpio/export
		echo out > /sys/class/gpio/gpio205/direction
		echo 1 > /sys/class/gpio/gpio205/value
		echo 4 > /sys/class/gpio/export
		echo out > /sys/class/gpio/gpio4/direction
		echo 1 > /sys/class/gpio/gpio4/value
		sleep 2
		echo 0 > /sys/class/gpio/gpio4/value
		GSM_DEVICE=/dev/ttymxc2
		GSM_SPEED=115200
	else
		echo "Platform not supported by this test"
	fi
}

test_gsm()
{
	show_test_banner "GSM"

	if [ "$1" != "-y" ]; then
		ask_user "Do you have an APF27Dev with an APW wireless extension board or an APF51Dev or APF6Dev with a GSM/3G modem and the corresponding Linux kernel ? (Y/n)"
		if [ "$response" == "n" ] || [ "$response" == "no" ]; then
			echo "Only APF27+APW or APF51Dev or APF6Dev are supported by this test yet !!"
			exit_failed
		fi
	fi

	execute_for_target prepare_gsm

	stty -F $GSM_DEVICE raw -echo -echoe -echok $GSM_SPEED
	if [ "$?" != 0 ]; then
		echo "Failed to setup serial port ($GSM_DEVICE)"
		exit_failed
	fi

	send_at_cmd ATE0	# ECHO OFF
	cat $GSM_DEVICE &
	sleep 1
	echo " --- Checking MoDem is responding:"
	send_at_cmd ATI3
	echo " --- Constructor:"
	send_at_cmd AT+CGMI
	echo " --- Model:"
	send_at_cmd AT+CGMM

	echo " --- Checking if PIN code is needed:"
	send_at_cmd AT+CPIN?
	ask_user "Did it tell you READY ? (N/y)"
	if [ "$response" != "y" ]; then
		ask_user "Please enter your PIN code (ENTER to skip)"
		if [ "$response" == "" ]; then
			PIN_CODE=$DEFAULT_PIN_CODE
		else
			PIN_CODE=$response
			DIGITS=`echo -n "$PIN_CODE" | wc -c`
			if [ "$DIGITS" == 4 ]; then
				echo -e -n "AT+CPIN=\"$PIN_CODE\"\015" > $GSM_DEVICE
				sleep 2
				send_at_cmd "AT+CPIN?"
			else
				echo "Bad PIN code ($PIN_CODE)"
				exit_failed_gsm
			fi
		fi
	fi

	echo " --- Checking signal quality (should be better than 12/99):"
	send_at_cmd AT+CSQ

	ask_user "Please enter a phone number, I will make it ring (default=$DEFAULT_PHONE_NUMBER)"
	if [ "$response" == "" ]; then
		PHONE_NUMBER=$DEFAULT_PHONE_NUMBER
	else
		PHONE_NUMBER=$response
	fi
	DIGITS=`echo -n "$PHONE_NUMBER" | wc -c`
	if [ "$DIGITS" != 10 ]; then
		echo "Bad phone number format ($PHONE_NUMBER)"
		exit_failed_gsm
	fi
	killall cat
	/usr/sbin/chat ABORT 'NO CARRIER' ECHO ON '' AT OK ATD"$PHONE_NUMBER"\; OK > $GSM_DEVICE < $GSM_DEVICE
	sleep 10
	send_at_cmd ATH
	ask_user "Did you ear it ring ? (Y/n)"
	if [ "$response" == "n" ] || [ "$response" == "no" ]; then
		echo "Probably no carrier ?"
		exit_failed_gsm
	fi
	
	echo_test_ok
	send_at_cmd ATE1	# ECHO ON
	exit 0
}

test_gsm $1
