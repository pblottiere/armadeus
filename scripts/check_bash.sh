#!/bin/bash

if [ -L /bin/sh ]; then
	SH_IS_BASH=`ls -al /bin/sh | grep bash`
	if [ "$SH_IS_BASH" == "" ]; then
		echo "On your system /bin/sh is a symbolic link that doesn't point to /bin/bash --> please correct that !"
		ls -al /bin/sh
		exit 1
	fi
fi
exit 0
