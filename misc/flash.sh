#!/bin/bash
HEX="$1"

[[ -z $HEX ]] && {
    echo "HEX shoud be defined"
    exit 1
}

[[ ! -e $HEX ]] && {
    echo "HEX isnt set to a valid .hex file"
    exit 1
}

echo "Flashing..."
sudo -v || exit 1

export logFile="flash.log"
source $(dirname $(readlink -m ${BASH_SOURCE[0]}))/task_source || { 
    echo "need task_source"
    exit 1
}

if [[ $(lsusb | grep -c Atmel) -gt 1 ]]; then
    echo "There are multiple devices present, please removal all but one"
    echo " and try again"
    exit 1
fi

function poll()
{
    while [[ -z $(lsusb | grep Atmel) ]] ; do
        sleep 1; 
    done
}

function flashit()
{
    sudo dfu-programmer atmega16u2 erase
} &>> "$logFile"


function uploadit()
{
    sudo dfu-programmer atmega16u2 flash ${HEX} --debug-level 5
} &>> "$logFile"


poll & waitProgress "Waiting for DFU mode (connect device and jump the pads!)"  || exit 1
flashit & waitProgress "Erasing..."                                             || exit 1
uploadit & waitProgress "Uploading..."                                          || exit 1
