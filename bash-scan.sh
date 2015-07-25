#!/bin/bash

USAGE="Usage: $0 HOST START_PORT END_PORT"

#Check the number of arguments.
if [ "$#" -ne 3 ]; then
	echo $USAGE
	exit
fi

#Check for non-numeric ports.
if [ "`echo $2 | egrep ^[[:digit:]]+$`" = "" -o "`echo $3 | egrep ^[[:digit:]]+$`" = "" ]; then
	echo $USAGE;
	echo "Ports must be numberic"
	exit
fi

#Check the port range.
if [ $2 -le 0 -o $2 -ge 65535 -o $3 -le 0 -o $3 -ge 65535 ]; then
	echo $USAGE;
	echo "Ports must be between 1 - 65534"
	exit
fi

function portScanner () {
	HOST=$1;
	START_PORT=$2;
	END_PORT=$3;

	for PORT in `seq $START_PORT $END_PORT`; do
		(echo >/dev/tcp/$HOST/$PORT) &>/dev/null
		
		if [ $? -eq 0 ]; then
			echo "$HOST:$PORT is open"
		else
			echo "$HOST:$PORT is closed"
		fi
	done
}

portScanner $1 $2 $3

#Copyright 2015 Shipping Soon
