#!/bin/bash
CURRENT=$(df $1 | grep / | awk '{ print $5}' | sed 's/%//g')
THRESHOLD="$2"

if [ "$CURRENT" -gt "$THRESHOLD" ] ; then
    /home/ubuntu/pushbullet "Disk Space Alert" "Your partition, $1, has critically low remaining free space. Used: $CURRENT percent"
fi
