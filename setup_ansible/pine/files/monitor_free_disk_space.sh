#!/bin/bash
CURRENT=$(df -BM $1 | grep / | awk '{ print $4}' | sed 's/M//g')
THRESHOLD="$2"

if [ "$CURRENT" -lt "$THRESHOLD" ] ; then
    /home/ubuntu/pushbullet "Disk Space Alert" "Your partition, $1, has critically low remaining free space. Remaining: $CURRENT MB"
fi
