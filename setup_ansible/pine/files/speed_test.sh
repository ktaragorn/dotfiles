let speed=$(iperf3 -c 192.168.1.1 -J | jq .end.sum_sent.bits_per_second)/1000000

if [[ speed -lt 10 ]]; then
	~/pushbullet "Pine Network Slow" "$speed mbits/sec"
fi