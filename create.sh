#!/bin/bash

sudo mknod /dev/calc-1st c 138 0
sudo chmod a+r+w /dev/calc-1st

sudo mknod /dev/calc-2nd c 139 0
sudo chmod a+r+w /dev/calc-2nd

sudo mknod /dev/calc-oper c 140 0
sudo chmod a+r+w /dev/calc-oper

sudo mknod /dev/calc-res c 141 0
sudo chmod a+r+w /dev/calc-res

sudo insmod task4.ko