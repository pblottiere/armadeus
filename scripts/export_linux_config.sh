#!/bin/bash

pwd
make shell_env
source ./armadeus_env.sh

if [ "$ARMADEUS_LINUX_MAIN_VERSION" == "2.6.29" ] || [ "$ARMADEUS_LINUX_MAIN_VERSION" == "2.6.35" ]; then
	echo "Not supported for this kernel version ($ARMADEUS_LINUX_MAIN_VERSION)"
	exit 1
fi

cd $ARMADEUS_LINUX_DIR
make savedefconfig
if [ $? != 0 ]; then
	echo "Failed to save defconfig"
	exit 1
fi
cd -

if [ "$1" != "" ]; then
	cp $ARMADEUS_LINUX_DIR/defconfig $1
	echo "Config copied in $1"
	exit 0
fi

echo "Please enter your favorite merge tool (ex: kompare, vimdiff, etc...)"
read -p "> " DIFFTOOL

if [ "$DIFFTOOL" == "" ]; then
	DIFFTOOL=kompare
fi
echo "Using: $DIFFTOOL"
echo $DIFFTOOL $ARMADEUS_LINUX_DIR/defconfig $ARMADEUS_LINUX_CONFIG
$DIFFTOOL $ARMADEUS_LINUX_DIR/defconfig $ARMADEUS_LINUX_CONFIG

exit 0
