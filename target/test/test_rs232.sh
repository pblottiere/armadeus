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


echo_jumper_help()
{
	if [ "$HELP" == "NO" ]; then
		return
	fi
	if [ "$1" == "APF9328" ]; then
		echo " (Pin 1 & 3 of UART connector)"
	elif [ "$1" == "APF27" ]; then
		echo " (Pin 2 & 3 of J21 connector (between RS and J8))"
	elif [ "$1" == "APF51" ]; then
		if [ "$2" == "0" ]; then
			echo
			echo " (Pin 2 & 3 of RS-232 connector \ o x x o o /)"
			echo "                                 \ o o o o /"
		else
			echo " (No consign yet for this port)"
		fi
	elif [ "$1" == "APF28" ]; then
		if [ "$2" == "0" ]; then
			echo " (Pin 21 & 23 (Rx/Tx) and 25 & 27 (CTS/RTS) of J9 connector)"
		elif [ "$2" == "1" ]; then
			echo " (Pin 26 & 28 (Rx/Tx) and 22 & 24 (CTS/RTS) of J9 connector)"
		elif [ "$2" == "2" ]; then
			echo " (Pin 12 & 14 (Rx/Tx) and 16 & 18 (CTS/RTS) of J9 connector)"
		elif [ "$2" == "3" ]; then
			echo " (Pin 11 & 13 (Rx/Tx) and 15 & 17 (CTS/RTS) of J9 connector)"
		elif [ "$2" == "4" ]; then
			echo " (Pin 14 & 16 (Rx/Tx) and 18 & 20 (CTS/RTS) of J10 connector)"
		else
			echo " (No consign yet for this port)"
		fi
	elif [ "$1" == "APF6" ]; then
		if [ "$2" == "4" ]; then
			echo
			echo " (Pin 15 & 16 (CTS/RTS) and 17 & 18 (Rx/Tx) of J5 connector | o o o o o o o v v o |)"
			echo "                                                            | o o o o o o o ^ ^ o |"
		else
			echo " (No consign yet for this port)"
		fi
	else
		echo "Platform not supported by this test"
	fi
}

test_serial_port()
{
	show_test_banner "Serial port $1"
	SERIAL_DEVICE="$SERIAL_DEVICES$1"
	TEMP_FILE="/tmp/serial_test$1"
	echo "$SERIAL_DEVICE"
	
	stty -F $SERIAL_DEVICE -echo
	if [ "$?" == 0 ]; then
		echo -n "Connect loopback jumper"
		execute_for_target echo_jumper_help $1
		ask_user "and then press <ENTER>"
		# Configure port as raw
		stty -F $SERIAL_DEVICE raw
		stty -F $SERIAL_DEVICE -echo -echoe -echok
		# Get incoming data
		cat $SERIAL_DEVICE > $TEMP_FILE & 
		pid=$!
		echo -n "."; sleep 1
		# Send data
		echo "Serial Port N°$1 ($SERIAL_DEVICE) is working." > $SERIAL_DEVICE
		echo -n "."; sleep 1; echo
		cat /proc/interrupts | grep uart
		kill $pid
		# Check if data were transmitted
		grep "working" $TEMP_FILE
		if [ "$?" == 0 ]; then
			echo_test_ok
		else
			exit_failed
		fi
	else
		echo "  serial port not found !"
	fi
}

configure_ports_devices()
{
	LINUX_VERSION=`uname -r | cut -d . -f 1-3`
	if [ "$1" == "APF9328" ]; then
		REGEXPR="*"
		if [ "$LINUX_VERSION" == "2.6.29" ]; then
			SERIAL_DEVICES="/dev/ttySMX"
		else
			SERIAL_DEVICES="/dev/ttymxc"
		fi
	elif [ "$1" == "APF27" ]; then
		REGEXPR="[!0]"
		if [ "$LINUX_VERSION" == "2.6.29" ]; then
			SERIAL_DEVICES="/dev/ttySMX"
		else
			SERIAL_DEVICES="/dev/ttymxc"
		fi
	elif [ "$1" == "APF51" ]; then
		REGEXPR="[!2]"
		SERIAL_DEVICES="/dev/ttymxc"
	elif [ "$1" == "APF28" ]; then
		REGEXPR="*"
		if [ "$LINUX_VERSION" == "2.6.35" ]; then
			SERIAL_DEVICES="/dev/ttySP"
		else
			SERIAL_DEVICES="/dev/ttyAPP"
		fi
	elif [ "$1" == "APF6" ]; then
		REGEXPR="[4]"
		SERIAL_DEVICES="/dev/ttymxc"
	else
		echo "$1 platform not supported by this test"
		exit 0
	fi
}


show_test_banner "Serial ports"
PORTS="$1"
execute_for_target configure_ports_devices
HELP="NO"
# No parameter given -> script in manual/help mode
if [ "$1" == "" ]; then
	HELP="YES"
	echo "Please give the port number to test !"
	PORTS=`ls $SERIAL_DEVICES$REGEXPR`
	echo -n "    Available one: [ "
	for port in $PORTS; do
		port_num=`echo $port | awk '{print substr($0,length($0))}'`
		echo -n "$port_num "
	done
	echo "]"
	read -p "> " PORTS
	[ "$PORTS" == "" ] && exit_failed
fi

for PORT in $PORTS; do
	test_serial_port $PORT
done
