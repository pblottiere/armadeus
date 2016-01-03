#!/bin/sh

#
# Script to test a Buildroot package for Armadeus Software release
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

#DEBUG=True
EXEC_NAME="imxregs"

set_ids()
{
	echo -n "Running on an $1"
	if [ "$1" == "APF27" ]; then
		echo "(Requires a run unlock_regs in U-Boot)"
		ID_REG="CID"
		ID_VAL="0x2882101d"
	elif [ "$1" == "APF9328" ]; then
		echo "(Requires a run unlock_regs in U-Boot)"
		ID_REG="SIDR"
		ID_VAL="0x00d4c01d"
	elif [ "$1" == "APF51" ]; then
		ID_REG="PREV"
		ID_VAL="0x000000f2"
	elif [ "$1" == "APF28" ]; then
		ID_REG="HW_DIGCTL_CHIP_ID"
		ID_VAL="0x28000001"
	elif [ "$1" == "APF6" ]; then
		ID_REG="PMU_MISC2"
		ID_VAL="0x00676767"
	fi
	echo "$APF"

}

test_imxregs()
{
	show_test_banner "imxregs"
	is_package_installed $EXEC_NAME

	execute_for_target set_ids
	$EXEC_NAME $ID_REG
	if [ "$?" == 0 ]; then
		CHIP_ID=`$EXEC_NAME $ID_REG | grep $ID_REG | sed 's/^.*0x/0x/' | cut -d " " -f 1`
		echo "Chip ID: $CHIP_ID"
		if [ "$CHIP_ID" == $ID_VAL ]; then
			echo_test_ok
			exit 0
		fi
	fi
	exit_failed
}

test_imxregs

