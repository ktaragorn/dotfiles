#!/bin/bash
cd /media/ubuntu/Chest
ls

if [ $? -eq 0 ]; then
    echo OK
elif [ $1 = 'try_only' ]; then
	/home/ubuntu/pushbullet "Chest HDD Alert" "The Chest hard disk is down, and unable to fix by remount!"
else
    /home/ubuntu/pushbullet "Chest HDD Alert" "The Chest hard disk is down, attempting to remount!"
    sudo systemctl stop smbd && sudo systemctl stop deluged && sudo umount /media/ubuntu/Chest/ && sleep 5 && sudo mount /media/ubuntu/Chest/
    sudo systemctl start smbd && sudo systemctl start deluged
    /home/ubuntu/monitor_chest_drive.sh try_only
fi