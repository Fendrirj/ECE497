#!/bin/bash
# Set up gpio 7 to read and gpio 3 to write
cd /sys/class/gpio
echo 30 > export
echo 60 > export
echo 31 > export
echo in > gpio30/direction
echo in > gpio60/direction
echo out > gpio31/direction
