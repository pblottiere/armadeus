#!/bin/bash
#
# Create custom directories and links of Armadeus rootfs
#

if [ ! -d "$1" ]; then
	echo "Target directory ($1) not valid !"
	exit 1
fi

# factorizable with post_image_creation.sh ?
symlink_image()
{
	# $1 = dir, $2 = filename, $3 = symlink
	if [ -f "$1/$2" ]; then
		cd $1; ln -sf $2 $3
	fi
}

# /bin
mkdir -p $1/bin
# /dev
mkdir -p $1/dev/pts
ln -sf /tmp/log $1/dev/log
# /home
mkdir -p $1/home
# /lib
mkdir -p $1/lib/firmware
# /media
mkdir -p $1/media/mmc
mkdir -p $1/media/usbdisk
# /mnt
mkdir -p $1/mnt/flash
mkdir -p $1/mnt/nfs
mkdir -p $1/mnt/smb
# /opt
mkdir -p $1/opt
# /proc
mkdir -p $1/proc
# /sbin
mkdir -p $1/sbin
# /sys
mkdir -p $1/sys
# /tmp
mkdir -p $1/tmp
# /usr
mkdir -p $1/usr/bin
mkdir -p $1/usr/lib
mkdir -p $1/usr/sbin
# /var
mkdir -p $1/var/lib
ln -sf /tmp $1/var/lib/pcmcia
ln -sf /tmp $1/var/lock
ln -sf /tmp $1/var/log
ln -sf /tmp $1/var/pcmcia
ln -sf /tmp $1/var/run
ln -sf /tmp $1/var/spool
ln -sf /tmp $1/var/tmp
# /boot
mkdir -p $1/boot
ln -sfn . $1/boot/dtbs	# to keep compat with pre-6.0 APF6 U-Boots
symlink_image $1/boot uImage $2-linux.bin
symlink_image $1/boot zImage $2-linux.bin

# Not using $2 because it can be changed for customers' BSP
echo "APF"${3//[!0-9]/} > $1/etc/machine

exit 0
