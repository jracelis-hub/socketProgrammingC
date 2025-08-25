#!/usr/bin/env bash

RESET="\033[0m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"

NET_NAME="$(nmcli c show | awk '/wlan0/ {print $1}')"

verbose_mode () {
	echo -e "${RED}Are you sure you want to manually change IPv4 address [y/n]?${RESET} "	
	read INPUT
	
	if [[ "${INPUT}" == "y" || "${INPUT}" == "y" ]]; then
		echo -e "Beginning manual configuration "

	else
		echo "Okay exiting..."
		exit 0
	fi

}
