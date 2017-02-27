#!/bin/bash
./bleh < input | grep Expected | grep -Eo '[0-9]+' | tac | xargs printf '%016x' | xxd -r -p - | rev | xargs printf '%s\n'
