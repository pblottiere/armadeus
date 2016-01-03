#!/bin/bash

setup() {
	modprobe leds-wm831x-status
	LED1=$(echo /sys/class/leds/LED1*);
	LED2=$(echo /sys/class/leds/LED2*);

	echo soft > $LED1/src;
	echo soft > $LED2/src;
}

handle_commands() {
	echo "1$(cat $LED1/brightness)" > $1
	echo "2$(cat $LED2/brightness)" > $1
	
	while read -r line < $1; do
		case $line in
			"10") echo 0 > $LED1/brightness;;
			"11") echo 1 > $LED1/brightness;;
			"20") echo 0 > $LED2/brightness;;
			"21") echo 1 > $LED2/brightness;;
		esac
	done
}

### main ###
case $# in
	1) setup; handle_commands $1;;
	*) echo "Usage: serial-server.sh <dev>. This script must be passed to rfcomm.";;
esac
