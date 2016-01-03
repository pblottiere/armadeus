#!/bin/bash

MYDIR=`dirname $0`

make shell_env -C $MYDIR/..

source $MYDIR/../armadeus_env.sh
if [ "$ARMADEUS_U_BOOT_VERSION" == "custom" ]; then
	ARMADEUS_U_BOOT_VERSION="custom ($ARMADEUS_U_BOOT_CUSTOM_VERSION)"
fi

echo -en "\033[1m"
echo ""
echo "Your Armadeus BSP (version "$ARMADEUS_BSP_VERSION" for "$ARMADEUS_BOARD_NAME") was successfully built !"
echo "    Buildroot: "$ARMADEUS_BUILDROOT_VERSION""
echo "    gcc: "$ARMADEUS_GCC_VERSION""
echo "    libc: "$ARMADEUS_LIBC_NAME $ARMADEUS_LIBC_VERSION""
echo "    busybox: "$ARMADEUS_BUSYBOX_VERSION""
echo "    U-Boot: "$ARMADEUS_U_BOOT_VERSION""
echo "    Linux: "$ARMADEUS_LINUX_VERSION""
echo ""
echo -en "\033[0m"

