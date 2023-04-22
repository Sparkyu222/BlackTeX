#!/bin/bash

rm -rf pgnp &&
git clone --recursive https://github.com/manzerbredes/pgnp.git &&

mkdir -p pgnp/build &&
cd pgnp/build && cmake ../ && make &&

cd ../../ &&
mkdir build/ &&
cmake . &&
make