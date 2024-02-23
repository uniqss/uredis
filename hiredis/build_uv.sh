#!/bin/bash


rm -rd ./libuv-1.48.0
tar zxvf libuv-1.48.0.tar.gz

cd libuv-1.48.0
mkdir build
cd build

cmake ..
cmake --build . --parallel 8
cmake --install . --prefix=../../../


cd ../../../
ls

