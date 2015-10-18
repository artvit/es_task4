#!/bin/bash

echo "12" > /dev/calc-1st
echo "8" > /dev/calc-2nd
echo "+" > /dev/calc-oper
sudo cat /dev/calc-res

echo "120" > /dev/calc-1st
echo "40" > /dev/calc-2nd
echo "-" > /dev/calc-oper
sudo cat /dev/calc-res

echo "5" > /dev/calc-1st
echo "11" > /dev/calc-2nd
echo "*" > /dev/calc-oper
sudo cat /dev/calc-res

echo "100" > /dev/calc-1st
echo "5" > /dev/calc-2nd
echo "/" > /dev/calc-oper
sudo cat /dev/calc-res

echo "100" > /dev/calc-1st
echo "11" > /dev/calc-2nd
echo "/" > /dev/calc-oper
sudo cat /dev/calc-res