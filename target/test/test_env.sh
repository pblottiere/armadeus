#!/bin/sh

#
# Envt variables for scripts to test Armadeus Software release
#
#  Copyright (C) 2008-2012 The Armadeus Project
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

MMC_MOUNT_DIR="/media/mmc"
MMC_DEVICE="/dev/mmcblk0p1"
TEMP_DIR="/tmp/test_release/"
TEMP_FILE="rnd_data.bin"
TEMP_FILE_SIZE=6144 # kbytes
WRITE_BENCH="/tmp/test_mmc.sh"
SERVER_IP="192.168.0.2"
SERVER_NFS_DIR=/local/export

mkdir -p $TEMP_DIR

