#!/bin/bash
/home/ubuntu/telegram "$1 HDD Alert" "The $1 hard disk is down, attempting to remount!"
sudo /bin/systemctl stop smbd && sudo /bin/systemctl stop deluged && sudo umount /media/ubuntu/$1; sleep 5 && sudo mount /media/ubuntu/$1  && sudo /bin/systemctl start deluged
sudo /bin/systemctl start smbd