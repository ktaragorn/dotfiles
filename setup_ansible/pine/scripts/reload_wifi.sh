#!/usr/bin/bash

/usr/bin/ssh -o StrictHostKeyChecking=no  -oHostKeyAlgorithms=+ssh-rsa -o PubkeyAcceptedKeyTypes=+ssh-rsa root@router wifi reload
