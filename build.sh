#!/bin/bash

echo ---------- CMAKE ----------
cmake .
if [ $? -ne 0 ]; then
    exit 0
fi
echo ---------------------------
echo

echo ---------- MAKE -----------
make
if [ $? -ne 0 ]; then
    exit 0
fi
echo ---------------------------
echo

echo --------- mlp-cli ---------
./bin/mlp-cli
echo ---------------------------