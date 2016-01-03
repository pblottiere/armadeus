#!/bin/sh 
export PIDFILE=/var/run/udhcpc/${1}.pid 
dhcp_lease()( 
[ -e $PIDFILE ] && ( kill `cat $PIDFILE` ; rm $PIDFILE ; ) 
mkdir -p `dirname $PIDFILE`
udhcpc -i $1 -h `hostname` -p $PIDFILE -b
) 
case $2 in 
CONNECTED) 
dhcp_lease $1
;; 
DISCONNECTED) 
[ -e $PIDFILE ] && ( kill `cat $PIDFILE` ; rm $PIDFILE ; ) 
wpa_cli -i $1  scan 
;; 
stop) 
wpa_cli -i $1 disconnect 
wpa_cli -i $1 terminate 
;; 
*) 
echo "Use this script as a wpa_cli action script or $0 iface1 stop to leave iface1 ready for manual configuration" 
;; 
esac 

