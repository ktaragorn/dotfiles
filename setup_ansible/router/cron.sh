ssh root@router "crontab -l | { cat; echo '4 5 * * * wifi'; } | crontab -"
ssh root@router "crontab -l | { cat; echo '@reboot iperf3 -s -D'; } | crontab -"
