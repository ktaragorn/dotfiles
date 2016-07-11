#!/bin/sh

#http://apple.stackexchange.com/questions/119177/mavericks-no-sound-after-sleep
sudo kextunload /System/Library/Extensions/AppleHDA.kext
sudo kextload /System/Library/Extensions/AppleHDA.kext
#sudo kill -9  `ps ax | grep [c]oreaudio | awk '{print $1}'`
