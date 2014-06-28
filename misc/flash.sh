#!/bin/bash
# export for task_source
export logFile="flash.log"
> "$logFile"

HEX="$1"

[[ -z $HEX ]] && {
    echo "HEX shoud be defined"
    exit 1
}

[[ ! -e $HEX ]] && {
    echo "HEX isnt set to a valid .hex file"
    exit 1
}


which dfu-programmer &>/dev/null || {
    echo "Error: Need 'dfu-programmer'"
    exit 1
}

function __die()
{
    echo "Error: $1" 1>2
    exit 1
}

function __die_log()
{
    echo "Error: See log"
    echo "---------- LOG ----------"
    cat "$logFile"
    exit 1
}

echo "Flashing..."
sudo -v || exit 1

source $(dirname $(readlink -m ${BASH_SOURCE[0]}))/task_source || { 
    __die "need 'task_source'"
}

if [[ $(lsusb | grep -c Atmel) -gt 1 ]]; then
    __die "There are multiple devices present, please removal all but one and try again"
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

function resetit()
{
    sleep 1
    sudo dfu-programmer atmega16u2 reset
} &>> "$logFile"

poll & waitProgress "Waiting for DFU mode (connect device and jump the pads!)"  || exit 1
flashit & waitProgress "Erasing..."                                             || __die_log
uploadit & waitProgress "Uploading..."                                          || __die_log
resetit & waitProgress "Resetting..."                                           || __die_log
