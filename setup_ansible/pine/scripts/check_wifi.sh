#!/usr/bin/bash

/usr/bin/ssh -o StrictHostKeyChecking=no  root@router wifi status | /usr/bin/jq '.radio0.up'
