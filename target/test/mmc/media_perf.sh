#!/bin/sh

#
# Script to test Armadeus boards performances to handle removable storage media
#
#  Copyright (C) 2007-2013 The Armadeus Project
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

if [ -f ./test_helpers.sh ]; then
	source ./test_helpers.sh
else
	source /usr/bin/testsuite/test_helpers.sh
fi

# Script parameters
BOARD=`cat /etc/machine`
if [ "$BOARD" == "APF9328" ]; then
	TEMP_FILE_SIZE=4096
else
	TEMP_FILE_SIZE=16384 # kbytes (ex: 16384 24576)
fi
NB_ITER=10

MMC_MOUNT_DIR="/media/mmc"
MMC_DEVICE="/dev/mmcblk0p1"
USB_DEVICE="/dev/sda1"
USB_MOUNT_DIR="/media/usbdisk"
TEMP_DIR="/tmp/perf_mmc/"
TEMP_FILE="/tmp/data.bin"
WRITE_BENCH="/tmp/test_mmc.sh"
REPORT_FILE="/tmp/media_perf.report"

WTIME=0
RTIME=0
#DEBUG=True
UID=`id -u`

debug()
{
	if [ "$DEBUG" == "True" ]; then
		echo $1
	fi
}

get_time_in_ms_from_file()
{
	if [ ! -f "$1" ]; then
		echo "File doesn't exist"
		return 1
	fi

	TIME=`cat $1 | grep real | cut -d m -f 2 | sed 's/s//'`
	STIME=`echo $TIME | cut -d . -f 1`
	STIME=`expr $STIME \* 1000`
	MSTIME=`echo $TIME | cut -d . -f 2`
	MSTIME=`expr $MSTIME \* 10`
	NAME=`echo $1 | grep write`
	if [ "$NAME" != "" ]; then
		WTIME=`expr $STIME + $MSTIME`
		debug "WTIME: $WTIME"
	else
		RTIME=`expr $STIME + $MSTIME`
		debug "RTIME: $RTIME"
	fi
}

clean_report()
{
	echo "# Report" > $REPORT_FILE
}

log_report()
{
	echo "$1" >> $REPORT_FILE
}

remount()
{
	if [ "$UID" == "0" ]; then	# root
		umount $MOUNT_DIR && mount $REM_DEVICE $MOUNT_DIR
	else
		# /etc/fstab & /etc/sudoers should have been configured
		sudo /bin/umount $MOUNT_DIR && mount $REM_DEVICE
	fi
}

special_umount()
{
	if [ "$WAS_MOUNTED" != "1" ]; then
		if [ "$UID" == "0" ]; then	# root
			umount $MOUNT_DIR
		else
			# /etc/fstab & /etc/sudoers should have been configured
			sudo /bin/umount $MOUNT_DIR
		fi
	fi
}

test_speed()
{
	# Create temp bench file
	create_random_file $TEMP_FILE $TEMP_FILE_SIZE

	# Create write bench script
	echo "mv $TEMP_FILE $MOUNT_DIR" > $WRITE_BENCH
	echo "sync" >> $WRITE_BENCH
	echo "exit 0" >> $WRITE_BENCH

	for it in `seq $NB_ITER`; do
		echo "--- Test iteration No$it ---"
		echo "  Writing"
		time sh $WRITE_BENCH 2>/tmp/writetime
		remount
		echo "  Reading"
		time mv $MOUNT_DIR/data.bin /tmp/ 2>/tmp/readtime
		# Update writing mean time
		get_time_in_ms_from_file /tmp/writetime
		if [ $it == "1" ]; then
			WTOTIME=$WTIME
		else
			WTOTIME=`expr $WTIME + $WTOTIME`
		fi
		debug "Total write time: $WTOTIME ms"
		# Update reading mean time
		get_time_in_ms_from_file /tmp/readtime
		if [ $it == "1" ]; then
			RTOTIME=$RTIME
		else
			RTOTIME=`expr $RTIME + $RTOTIME`
		fi
		debug "Total read time: $RTOTIME ms"
	done

	WTOTIME=`expr $WTOTIME / $it`
	RTOTIME=`expr $RTOTIME / $it`
	WSPEED=`expr $TEMP_FILE_SIZE \* 1000 / $WTOTIME`
	RSPEED=`expr $TEMP_FILE_SIZE \* 1000 / $RTOTIME`
	echo "--- Test result (mean values): read -> $RSPEED kBytes/sec   write -> $WSPEED kBytes/sec"
	log_report "RESULT=OK"
	log_report "READ_SPEED=$RSPEED"
	log_report "WRITE_SPEED=$WSPEED"
	special_umount
}

usage()
{
	echo -e "\n   Usage: $0 options\n"
	echo "-m|--mode=<speed|integrity> : mandatory, test mode to use"
	echo "-d|--mode=<device> : device to use for test (default to $MMC_DEVICE)"
	echo "-n|--nb=<iteration> : number of test iterations (default to $NB_ITER)"
	echo "-h|--help: this help"
#	echo "-q|--quiet: don't output anything on stdout"

	exit 1
}

#
# Main
#

# Process parameters
if [ "$#" == 0 ]; then
	usage
fi
GETOPT=`getopt -o hm:n:d:q --long help,mode:,nb:,device:,quiet -- "$@"`
if [ $? != 0 ] ; then
	usage
fi
eval set -- "$GETOPT"
while true ; do
	case $1 in
		-q|--quiet)
			QUIET=1
			shift ;;
		-h|--help)
			usage ;;
		-m|--mode)
			[ "$2" == "" ] && usage
			MODE="$2"
			shift 2 ;;
		-n|--nb)
			[ "$2" == "" ] && usage
			NB_ITER="$2"
			shift 2 ;;
		-d|--device)
			[ "$2" == "" ] && usage
			MMC_DEVICE="$2"
			shift 2 ;;
		--)
			shift ; break ;;
		*)
			usage ;;
	esac
done

if [ "$MODE" != "speed" ] && [ "$MODE" != "integrity" ]; then
	usage
fi

show_test_banner "Storage medium performances"
clean_report

if [ ! -b $MMC_DEVICE ] && [ ! -b $USB_DEVICE ]; then
	echo -n "Please insert your storage device: MMC/SD/microSD or USB key"
	it=0
	while [ ! -b $MMC_DEVICE ] && [ ! -b $USB_DEVICE ] && [ $it -le 20 ]; do
		it=$((it+1))
		sleep 2
		echo -n "."
	done
fi

if [ -b $MMC_DEVICE ]; then
	echo "Testing MMC/SD/microSD"
	REM_DEVICE=$MMC_DEVICE
	MOUNT_DIR=$MMC_MOUNT_DIR
elif [ -b $USB_DEVICE ]; then
	echo "Testing USB key"
	REM_DEVICE=$USB_DEVICE
	MOUNT_DIR=$USB_MOUNT_DIR
else
	echo "No removable device found: exiting..."
	exit 1
fi

# Check some pre-requisites
if [ ! -d $MOUNT_DIR ]; then
	echo "Create $MOUNT_DIR directory ? (y/N)"
	read answer
	if [ "$answer" == "y" ] || [ "$answer" == "yes" ]; then
		mkdir -p $MOUNT_DIR
	else
		"Exiting..."
		exit 1
	fi
fi

# If not mounted, mount it:
WAS_MOUNTED=`mount | grep -c $MOUNT_DIR`
if [ "$WAS_MOUNTED" != "1" ]; then
	if [ "$UID" == "0" ]; then	# root
		mount $REM_DEVICE $MOUNT_DIR
	else
		# /etc/fstab should have been configured
		mount $REM_DEVICE
	fi
	if [ "$?" != 0 ]; then
		echo "Failed to mount device $REM_DEVICE"
		exit 1
	fi
fi

# Here we go:
MEDIUM_SIZE=`df -h | grep mmc | awk '{ print $2 }'`
echo "Medium size: $MEDIUM_SIZE"
log_report "SIZE=$MEDIUM_SIZE"

if [ "$MODE" == "speed" ]; then
	test_speed
	exit 0
fi

# or integrity test:

if [ "$NB_ITER" == "" ]; then NB_ITER=40; fi
it=0
mkdir -p $TEMP_DIR

date
FILE=$TEMP_DIR/test.rnd
MD5=$TEMP_DIR/test.rnd.md5
dd if=/dev/urandom of=$FILE bs=1024 count=1024 2>/dev/null
md5sum $FILE > $MD5

while [ $it -le $NB_ITER ]; do
	FILEMMC=$MOUNT_DIR/test_$it.rnd
#	MD5=$TEMP_DIR/test_$it.rnd.md5
	echo -en "\r Copying: $FILEMMC"
	it=$((it+1))
	cp $FILE $FILEMMC
	sync
done

echo ""

remount

date
FILES=`ls $MOUNT_DIR/*.rnd`
for file in $FILES; do
	file=`basename $file`
	echo -en "\r Checking MD5 for: $file"
	MD5MMC=`md5sum $MOUNT_DIR/$file | cut -d " " -f 1`
	MD5ORG=`cat $MD5 | cut -d " " -f 1`
	if [ "$MD5MMC" != "$MD5ORG" ]; then
		echo -e "\nMD5 failed for $file: $MD5MMC $MD5ORG"
		exit 1
	fi
done
echo
date
echo "Integrity test successful !"

rm -rf $TEMP_DIR
rm $MOUNT_DIR/*.rnd
special_umount

exit 0
