#!/bin/bash

help()
{
	echo
	echo "This script will publish Armadeus Linux and U-Boot current/stable patches."
	echo "This way we ease Armadeus board integration in build systems, like Buildroot for example."
	echo
}

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
	help
	exit 1
fi

echo "--- Getting envt variables:"
THIS_DIR=`dirname $0`
make -C $THIS_DIR/.. shell_env
source $THIS_DIR/../armadeus_env.sh

PATCHES_LIST=`ls $LINUX_PATCHES_DIR`
LINUX_VERSIONS=`ls $ARMADEUS_LINUX_PATCH_DIR/..`
echo
PS3="Choose the Linux kernel you want to publish: "
select LINUX in $LINUX_VERSIONS
do
	if [ "$LINUX" == "" ]; then
		exit 1
	else
		echo "You choosed $LINUX"
		break
	fi
done

echo "--- Merging patches:"
FINAL_PATCH=linux-$LINUX-armadeus.patch
rm -f $FINAL_PATCH
touch $FINAL_PATCH
LINUX_PATCHES=`ls $ARMADEUS_LINUX_PATCH_DIR/../$LINUX`
for patch in $LINUX_PATCHES; do
	echo "Merging $patch"
	cat $ARMADEUS_LINUX_PATCH_DIR/../$LINUX/$patch >> $FINAL_PATCH
#	sleep 1
done

echo
echo "--- Compressing:"
tar cvzf $FINAL_PATCH.tar.gz $FINAL_PATCH
if [ "$?" == 0 ]; then
	rm $FINAL_PATCH
fi

echo
echo "--- Uploading archive:"
HOST=ftp2.armadeus.com
echo "username for $HOST ?"
read -p "> " USER
echo "password ?"
read -p "> " PASSWD
if [ "$USER" == "" ] || [ "$PASSWD" == "" ]; then
	echo "aborting"
	exit 1
fi
ftp -n $HOST <<END_SCRIPT
quote USER $USER
quote PASS $PASSWD
cd download
put $FINAL_PATCH.tar.gz
quit
END_SCRIPT

if [ "$?" == 0 ]; then
	echo "done !"
fi

exit 0
