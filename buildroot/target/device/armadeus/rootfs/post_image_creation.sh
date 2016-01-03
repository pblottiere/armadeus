#!/bin/sh
#
# arg1: BR images dir
# arg2: board name
# arg3: i.MX processor type

if [ ! -d "$1" ]; then
	echo "Images directory ($1) not valid !"
	exit 1
fi

symlink_image()
{
	# $1 = dir, $2 = filename, $3 = symlink
	if [ -f "$1/$2" ]; then
		cd $1; ln -sf $2 $3
	fi
}

# Create images symbolic links according to board name,
# for compatibility with older armadeus versions:
if [ "$2" != "" ]; then
	# New BR

	# Cleanup if previous build
	rm -f $1/$2-rootfs.*
	rm -f $1/$2-linux.bin
	rm -f $1/$2.dtb

	# rootfs
	for rootfstype in ubi ubifs tar jffs2 ext4; do
		symlink_image $1 rootfs.$rootfstype $2-rootfs.$rootfstype
	done

	# linux
	symlink_image $1 uImage $2-linux.bin
	symlink_image $1 zImage $2-linux.bin
	symlink_image $1 $3-$2dev.dtb $2.dtb

	# APF6
	symlink_image $1 SPL $2-u-boot.spl
	symlink_image $1 u-boot.img $2-u-boot.img

else
	# Old BR compat
	BOARD_NAME=`grep 'BR2_BOARD_NAME=' $BUILDROOT_CONFIG | cut -d = -f 2 | sed s/\"//g`
	if [ "$BOARD_NAME" != "" ]; then
		mv -u $1/rootfs.ubi $1/$BOARD_NAME-rootfs.ubi
	fi
fi

# if target uses ext4 for rootfs,
if [ -f "$1/rootfs.ext4" ]; then
	# generate also boot.ext4:
	mke2img -d $1/../target/boot/ -G 4 -R 1 -l BOOT -b 49152 -o $1/boot.ext4
	symlink_image $1 boot.ext4 $2-boot.ext4
	# overwrite kernel symlink in boot.ext4 as U-Boot doesn't like it:
	e2cp $1/*-linux.bin -d $1/boot.ext4:/
	# make a copy of rootfs for MMC/SD usage:
	#cp $1/rootfs.ext4 $1/rootfs-for-sd.ext4
	# cleanup /boot in eMMC rootfs:
	#e2rm $1/rootfs.ext4:/boot/*
fi

# generate full HD/MMC image if config file found
GENIMAGE_CFG="${BASE_DIR}/../target/device/armadeus/$2/genimage.cfg"
GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"
rm -rf "${GENIMAGE_TMP}"
if [ -f "$GENIMAGE_CFG" ]; then
	genimage \
		--rootpath "${TARGET_DIR}" \
		--tmppath "${GENIMAGE_TMP}" \
		--inputpath "${BINARIES_DIR}" \
		--outputpath "${BINARIES_DIR}" \
		--config "${GENIMAGE_CFG}"
fi

RET=${?}
exit ${RET}
