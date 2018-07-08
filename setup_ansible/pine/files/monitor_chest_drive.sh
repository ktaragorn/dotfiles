#!/bin/bash
cd /media/ubuntu/Chest
ls

if [ $? -eq 0 ]; then
    echo OK
else
	# to test on first alert if it can be remounted right here
    /home/ubuntu/pushbullet "Chest HDD Alert" "The Chest hard disk is down, remount asap!"
fi