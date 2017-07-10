#!/bin/sh
cd android
./gradlew installDebug
adb shell am start -n org.libsdl.app/org.libsdl.app.SDLActivity
cd ..
