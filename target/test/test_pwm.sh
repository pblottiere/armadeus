#!/bin/sh

#
# Script to test Armadeus Software release
#
#  Copyright (C) 2010-2015 The Armadeus Project - Armadeus Systems
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

#
# Script to test your PWM: plug either an oscilloscope or a buzzer on the signal
#   and launch this script
#

source ./test_helpers.sh
source ./test_env.sh

SYS_DIR="/sys/class/pwm/pwm0/"
PERIOD="1000 2000 3000 4000 5000 6000 7000 8000 9000 10000"
FREQUENCY="100 200 300 400 500 600 700 800"
DUTY="001 250 333 500 666 750 999"	# may be overwritten in test
SLEEP_TIME=4
SYS_DUTY=$SYS_DIR/duty

load_driver()
{
	if [ "$1" == "APF9328" ] || [ "$1" == "APF27" ] || [ "$1" == "APF51" ]; then
		modprobe imx-pwm
		if [ ! -d "$SYS_DIR" ]; then
			echo "can't find /sys/ interface"
			if [ "$1" == "APF51" ]; then
				echo -e "On APF51, PWM is multiplexed with i.MX \
LED so you can't have both\nwith default\
 kernel"
			fi
			exit_failed
		fi
	elif [ "$1" == "APF28" ]; then
		if [ "$LINUX_VERSION" == "2.6.35" ]; then
			SYS_DIR="/sys/class/pwm/pwm4/"
		else
			SYS_DIR="/sys/class/pwm/pwmchip0/pwm0"
			echo 0 > /sys/class/pwm/pwmchip0/export
		fi
		if [ ! -d "$SYS_DIR" ]; then
			echo "can't find /sys/ interface"
			exit_failed
		fi
	elif [ "$1" == "APF6" ]; then
		SYS_DIR="/sys/class/pwm/pwmchip2/pwm0"
		echo 0 > /sys/class/pwm/pwmchip2/export
		if [ ! -d "$SYS_DIR" ]; then
			echo "can't find /sys/ interface"
			exit_failed
		fi
	else
		echo "Platform not supported by this test"
		exit 0
	fi
}

prepare_test()
{
	OSCILLO="Please connect your oscilloscope probe"
	WHEN_READY="then press ENTER when ready"
	if [ "$1" == "APF9328" ]; then
		PLAT_MSG="to pin 4 of X21/Timer connector (GND is on pin 8)"
	elif [ "$1" == "APF27" ]; then
		PLAT_MSG="to pin 4 of J22 connector (GND is on pin 6 or 40)"
	elif [ "$1" == "APF51" ]; then
		PLAT_MSG="to pin 6 of J35 (Extension) connector (GND is on pin 9 or 10)"
		EXTRA_PLAT_MSG="\n (you can also watch i.MX LED; requires a custom built kernel)"
	elif [ "$1" == "APF28" ]; then
		PLAT_MSG="to pin 26 of J10 connector (GND is on pin 32 or 34)"
	elif [ "$1" == "APF6" ]; then
		PLAT_MSG="to pin 26 of J2 (LVDS) connector (GND is on pin 32 or 34)"
		EXTRA_PLAT_MSG="\n (PWM3 is inverted and may be used as backlight and require a modified dtb)"
	fi

	ask_user "$OSCILLO $PLAT_MSG, $WHEN_READY $EXTRA_PLAT_MSG"
}

test_pwm_frequency()
{
	if [ "$1" == "APF6" ] || [[ "$1" == "APF28" && "$LINUX_VERSION" != "2.6.35" ]]; then
		return
	fi
	ask_user "Press ENTER when ready to check PWM output (frequencies starting at 100Hz)"
	echo 1 > $SYS_DIR/active
	# Test frequency setting
	for freq in $FREQUENCY; do
		echo $freq > $SYS_DIR/frequency
		echo "Setting PWM frequency to $freq Hz"
		sleep $SLEEP_TIME
	done
}

test_pwm_period()
{
	UNIT=us
	ONE_MHZ=1000000	# us
	if [ "$1" == "APF6" ] || [[ "$1" == "APF28" && "$LINUX_VERSION" != "2.6.35" ]]; then
		UNIT=ns
		ONE_MHZ=1000000000	# ns
		echo 1 > $SYS_DIR/enable
		SYS_DUTY=$SYS_DIR/duty_cycle
	fi
	ask_user "Press ENTER when ready to check PWM output (periods)"
	# Test period setting
	for period in $PERIOD; do
		if [ "$1" == "APF6" ]; then
			period=$((period*1000))	# us -> ns
			duty=$((period/2))
			echo $duty > $SYS_DUTY
		fi
		echo $period > $SYS_DIR/period
		freq=$((ONE_MHZ / period))
		echo "Setting PWM period to $period $UNIT (-> $freq Hz)"
		sleep $SLEEP_TIME
	done
}

test_pwm_duty()
{
	DUTY_MAX=1000
	PERIOD=10000
	if [ "$1" == "APF6" ]; then
		PERIOD=10000000
		DUTY="100000 2500000 3333333 5000000 6666666 7500000 9900000"
		DUTY_MAX=10000000
		SYS_DUTY=$SYS_DIR/duty_cycle
	fi
	ask_user "Press ENTER when ready to check PWM output (duty cycle @ 100Hz)"
	# Test duty cycle setting @ 100Hz -> duty value = positive width signal value
	echo $PERIOD > $SYS_DIR/period
	for duty in $DUTY; do
		echo $duty > $SYS_DUTY
		echo "Setting duty cycle to $duty/$DUTY_MAX"
		sleep $SLEEP_TIME
	done
}

test_pwm_end()
{
	if [ "$1" == "APF6" ]; then
		echo 500 > $SYS_DIR/duty_cycle
		echo 0 > $SYS_DIR/enable
	else
		echo 500 > $SYS_DIR/duty
		echo 0 > $SYS_DIR/active
	fi
	echo "End of PWM test"
}

test_pwm()
{
	show_test_banner "PWM"

	execute_for_target load_driver

	if [ "$1" != "" ]; then
		prepare_test $1
	else
		execute_for_target prepare_test
	fi

	execute_for_target test_pwm_frequency

	execute_for_target test_pwm_period

	execute_for_target test_pwm_duty

	execute_for_target test_pwm_end

	ask_user "Did you get correct signals ? (y/N)"
	if [ "$response" != "y" ] && [ "$response" != "yes" ]; then
		exit_failed
	fi

	echo_test_ok
	exit 0
}

test_pwm $1

