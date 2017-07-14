#!/bin/sh

#Cleanup
rm -rf build
rm -rf android/dependencies/jni/gdxpp
rm -rf android/dependencies/jni/SDL2
rm -rf android/dependencies/jni/SDL2_image
rm -rf android/dependencies/libs
rm -rf android/dependencies/obj
rm -rf android/build
rm -rf android/app/build
rm -rf android/app/.externalNativeBuild

#Build
sh buildAndroid.sh
sh buildDesktop.sh

