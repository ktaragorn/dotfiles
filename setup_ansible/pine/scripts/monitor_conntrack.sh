#!/bin/bash

conntrack=`cat /proc/sys/net/netfilter/nf_conntrack_count`

if (( conntrack > 10000 )); then
    /home/ubuntu/telegram "Conntrack is higher than 10000 - $conntrack"
fi