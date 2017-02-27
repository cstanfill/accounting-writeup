#!/bin/sh
objdump -d -C bleh | awk -v RS='' "/^[0-9a-f]+ <$1\(\)>/"
