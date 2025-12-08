#!/bin/bash
set -x
BUILD_DIR=build_test
clear
if [ ! -d $BUILD_DIR ]; then mkdir $BUILD_DIR; fi
pushd $BUILD_DIR
rm *.i *.s *.o *.dll
#i686-w64-mingw32-cpp -o proxy_dinput.i -CC -D_UCRT ../code/proxy_dinput.c
#i686-w64-mingw32-gcc -o proxy_dinput.s -O0 -S -x c -std=c99 -fms-extensions proxy_dinput.i
#i686-w64-mingw32-as  -o proxy_dinput.o proxy_dinput.s
#i686-w64-mingw32-ld  -o dinput8.dll    -s -shared --dll --major-os-version 5 --minor-os-version 1 --major-image-version 0 --subsystem windows:5.1 proxy_dinput.o -L'/usr/i686-w64-mingw32/sys-root/mingw/lib/' -lkernel32 -luser32 -lucrt

i686-w64-mingw32-gcc -o dinput8.o -O2 -c ../code/proxy_dinput.c
i686-w64-mingw32-gcc -o dinput8.dll dinput8.o ../code/proxy_dinput.def -shared

cp dinput8.dll ../_game_/
popd
