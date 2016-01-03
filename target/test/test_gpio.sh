#!/bin/sh

#
# Script to test Armadeus Software release
#
#  Copyright (C) 2008-2012 The Armadeus Project - Armadeus systems
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

source ./test_helpers.sh
source /usr/bin/gpio_helpers.sh

load_module()
{
	if [ "$1" == "APF9328" ] || [ "$1" == "APF27" ]; then
		loadgpio.sh
		if [ "$?" != 0 ] || [ ! -c "$GPIO_DEV_DIR/portA" ]; then
			echo "Module failed to load"
			exit_failed
		fi
	elif [ "$1" == "APF51" ] || [ "$1" == "APF28" ] || [ "$1" == "APF6" ]; then
		echo
	else
		echo "Platform not supported by this test"
		exit 0
	fi
}

unload_module()
{
	if [ "$1" == "APF9328" ] || [ "$1" == "APF27" ]; then
		rmmod gpio
	elif [ "$1" == "APF51" ] || [ "$1" == "APF28" ] || [ "$1" == "APF6" ]; then
		echo
	else
		echo "Platform not supported by this test"
	fi
}

# $1 == LED number to blink
blink_led_gpiolib()
{
	if [ "$1" == "" ]; then
		echo "Please give a correct GPIO number"
		return
	fi
	LED="$1"
	echo $LED > /sys/class/gpio/export
	if [ "$?" != 0 ]; then
		echo "Failed to export GPIO $LED"
		return
	fi
	ls /sys/class/gpio/gpio$LED/

	echo out > /sys/class/gpio/gpio$LED/direction
	for i in `seq 0 5`; do
		echo 1 > /sys/class/gpio/gpio$LED/value
		usleep 500000
		echo 0 > /sys/class/gpio/gpio$LED/value
		usleep 500000
	done
	echo $LED > /sys/class/gpio/unexport
}

blink_led_gpioled()
{
	if [ "$1" == "" ]; then
		echo "Please give a correct LED name"
		return
	fi

	MAX_BRIGHT=`cat /sys/class/leds/"$1"/max_brightness 2>/dev/null`
	if [ "$MAX_BRIGHT" == "" ]; then
		MAX_BRIGHT=255
	fi
	if [ -f /sys/class/leds/"$1"/gpio ]; then
		echo gpio > /sys/class/leds/"$1"/trigger
		echo 1 > /sys/class/leds/"$1"/gpio
	fi
	for i in `seq 0 5`; do
		echo -n .
		echo 0 > /sys/class/leds/"$1"/brightness
		usleep 500000
		echo -n .
		echo $MAX_BRIGHT > /sys/class/leds/"$1"/brightness
		usleep 500000
	done
}

blink_led()
{
	if [ "$1" == "APF9328" ]; then
		LED_NAME="a"
		gpio_mode PD31 1
		for i in `seq 0 5`; do
			gpio_set_value PD31 1
			usleep 500000
			gpio_set_value PD31 0
			usleep 500000
		done
	elif [ "$1" == "APF27" ]; then
		LED_NAME="D14"
		echo -e "\n    _oO- Old way to do it (custom GPIO driver)... -Oo_"
		gpio_mode PF14 1
		for i in `seq 0 5`; do
			gpio_set_value_old PF14 0
			usleep 500000
			gpio_set_value_old PF14 1
			usleep 500000
		done

		echo -e "\n    _oO- New way to do it (gpiolib)... -Oo_"
		# PF14 -> GPIO 174 (5x32 + 14)
		blink_led_gpiolib 174

		echo -e "\n    _oO- New way to do it (led-gpios)... -Oo_"
		blink_led_gpioled apfdev\:green\:user
	elif [ "$1" == "APF51" ]; then
		LED_NAME="i.MX"
		blink_led_gpiolib 2
		blink_led_gpioled apfdev\:green\:user
	elif [ "$1" == "APF28" ]; then
		if [ "$LINUX_VERSION" == "2.6.35" ]; then
			blink_led_gpioled apfdev\:green\:user
		else
			blink_led_gpioled Heartbeat
		fi
	else
		echo "Platform not supported by this test"
	fi
}

check_button()
{
	if [ "$1" == "APF9328" ]; then
		echo "TBDL"
	elif [ "$1" == "APF27" ]; then
		gpio_mode PF13 0	# input
		setbit /proc/driver/gpio/portFirq 13 1
		echo "Press 2 times on S1 button"
		/usr/bin/testsuite/testbutton /dev/gpio/PF13 3
		if [ "$?" != 0 ]; then
			exit_failed
		fi
	elif [ "$1" == "APF51" ] || [ "$1" == "APF28" ]; then
		echo "TBDL"
	else
		echo "Platform not supported by this test"
	fi
}

test_led_gpio()
{
	show_test_banner "GPIO (LED)"

	execute_for_target load_module

	ask_user "I will now blink the User LED on your board at 1Hz. Press ENTER to continue"
	execute_for_target blink_led

	ask_user "Did you see "$LED_NAME" LED blinking ? (y/N)"
	if [ "$response" != "y" ] && [ "$response" != "yes" ]; then
		exit_failed
	fi

	execute_for_target check_button

	execute_for_target unload_module
	echo_test_ok
	exit 0
}

test_led_gpio

