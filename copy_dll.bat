@echo off

set curdir=%~dp0

if not exist %curdir%\_game_ mkdir %curdir%\_game_
if exist %curdir%\Debug\openside.dll copy %curdir%\Debug\openside.dll %curdir%\_game_\
