#!/usr/bin/bash

/usr/bin/ssh -o StrictHostKeyChecking=no  -oHostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa  root@router wifi status | /usr/bin/jq '.radio0.up'
