speed_bps=$(iperf3 -c 192.168.1.1 -J | jq .end.sum_sent.bits_per_second)
speed_mbps=$(echo $speed_bps/1000000 | bc)

if [ $speed_mbps -lt 10 ]; then
	~/pushbullet "Pine Network Slow" "$speed_mbps mbits/sec"
fi
