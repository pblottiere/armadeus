#!/bin/sh

#
# Script to test Armadeus Software release
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


WTIME=0
RTIME=0
#DEBUG=True

test_portmap()
{
	show_test_banner "Portmap"

	# Check if interface is up
	ifconfig eth0
	if [ "$?" != 0 ]; then
		echo "No Ethernet interface found"
		exit_failed
	fi

	echo "  NFS connect"
	mount -t nfs $SERVER_IP:$SERVER_NFS_DIR /mnt/nfs
	if [ "$?" != 0 ]; then
		echo "Failed to connect to server"
		exit_failed
	fi
	echo "  Check content"
	ls /mnt/nfs
	if [ "$?" != 0 ]; then
		echo "Failed to access directory content"
		umount test
		exit_failed
	fi
	umount /mnt/nfs
	if [ "$?" != 0 ]; then
		echo "Failed to disconnect from server"
		exit_failed
	fi

	echo_test_ok
}

test_portmap
