#!/bin/sh

mkdir android/dependencies/jni/gdxpp/build
cd android/dependencies/jni/gdxpp/build
cmake ..
make
cd ../../../../..
mkdir build
cd build
cmake ..
ln -s ../assets assets
make
./Core
