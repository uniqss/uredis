#!/bin/bash


rm -rd ./hiredis-1.2.0
tar zxvf hiredis-1.2.0.tar.gz

cd hiredis-1.2.0
mkdir build
cd build

cmake ..
cmake --build . --parallel 8
cmake --install . --prefix=../../../


cd ../../../
ls

