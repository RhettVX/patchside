@echo off
setlocal EnableDelayedExpansion EnableExtensions

cls
color
::call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86

set BUILD_DIR=build_proxy_dinput
if exist !BUILD_DIR! del /q !BUILD_DIR!\*
if not exist !BUILD_DIR! mkdir !BUILD_DIR!

pushd !BUILD_DIR!

set LIBS=user32.lib
cl /Fe:dinput8.dll ..\code\proxy_dinput.c /nologo /W3 /WX /O2 /MD /options:strict /LD /link /incremental:no /subsystem:windows,5.1 /def:..\code\proxy_dinput.def !LIBS!
if !errorlevel! neq 0 goto error
copy dinput8.dll "D:\Projects\openside\_game_\"

goto noerror
:error
color f4
:noerror

popd