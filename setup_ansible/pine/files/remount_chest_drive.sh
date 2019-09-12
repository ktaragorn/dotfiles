#!/bin/bash
/home/ubuntu/pushbullet "Chest HDD Alert" "The Chest hard disk is down, attempting to remount!"
sudo /bin/systemctl stop smbd && sudo /bin/systemctl stop deluged && sudo umount /media/ubuntu/Chest; sleep 5 && sudo mount /media/ubuntu/Chest  && sudo /bin/systemctl start deluged
sudo /bin/systemctl start smbd