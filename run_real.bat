@echo off
if not exist _game_ mkdir _game_
cd _game_
start planetside.exe /K:StagingTest /CC
