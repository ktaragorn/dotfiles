#!/bin/bash
cd /media/ubuntu/Chest
ls

if [ $? -eq 0 ]; then
    echo OK
elif [ $1 = 'try_only' ]; then
	/home/ubuntu/pushbullet "Chest HDD Alert" "The Chest hard disk is down, and unable to fix by remount!"
else
    /home/ubuntu/remount_drive.sh Chest
    /home/ubuntu/monitor_chest_drive.sh try_only
fi