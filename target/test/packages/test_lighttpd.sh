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
EXEC_NAME="lighttpd"
HTML_DIR="/var/www/servers/www.example.org/pages/"
HTML_FILE="$HTML_DIR/index.html"
CONFIG="/root/lighttpd.conf"
HTML_PORT=3000
BOARD=`cat /etc/machine`

generate_html_test_file()
{
	cat << EOF > $HTML_FILE
<html>
    <head>
        <title> Test of $EXEC_NAME Web Server </title>
    </head>
    <body>
        <h1> If you can read that, then the test on your $BOARD is OK ! ;-) </h1>
    </body>
</html>
EOF
}

generate_lighttpd_config_file()
{
	cat << EOF > $CONFIG
server.document-root = "/var/www/servers/www.example.org/pages/" 

server.port = $HTML_PORT

mimetype.assign = (
  ".html" => "text/html", 
  ".txt" => "text/plain",
  ".jpg" => "image/jpeg",
  ".png" => "image/png" 
)
EOF
}

test_cleanup()
{
	rm -f "$HTML_FILE"
	killall $EXEC_NAME
}

test_lighttpd()
{
	show_test_banner "Lighttpd"

	is_package_installed $EXEC_NAME

	if [ "$?" == 0 ]; then
		mkdir -p $HTML_DIR
		grep armadeus /etc/hosts | grep 127.0.0.1
		if [ "$?" != 0 ]; then
			echo "127.0.0.1 `cat /etc/hostname`" >> /etc/hosts
		fi
		generate_html_test_file
		generate_lighttpd_config_file
		IP=`ifconfig eth0 | grep "inet addr" | cut -d ":" -f 2 | cut -d " " -f 1`
		# Launch it
		$EXEC_NAME -f $CONFIG
		#
		echo "Now you can access to http://"$IP":"$HTML_PORT"/index.html from your Host PC Web browser"
		sleep 8
		if [ "$?" == 0 ]; then
			ask_user "Did you manage to show the web page ? (y/N)"
			if [ "$response" == "y" ] || [ "$response" == "yes" ]; then
				echo_test_ok
				test_cleanup
				exit 0
			fi
		fi
	fi
	test_cleanup
	exit_failed
}

test_lighttpd
