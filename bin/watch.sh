#!/bin/bash
# usage: watch.sh <your_command>
# https://stackoverflow.com/a/9574123/1520364
while :; 
  do 
  clear
  date
  $@ # to allow for complex commands like adb devices without needing quotes
  sleep 2
done
