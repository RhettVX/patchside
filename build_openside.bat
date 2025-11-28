@echo off
setlocal EnableDelayedExpansion EnableExtensions
cls
color

call "C:\Program Files (x86)\Microsoft Visual Studio\VC98\Bin\vcvars32.bat" > nul

if not exist Debug mkdir Debug
pushd Debug

cl -Feopenside.dll -Tc..\code\main.c -nologo -G6 -LD -Od -W3 -Zi -link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"openside.pdb" /map:"openside.map" /debug /machine:I386 /def:"..\code\exports.def" /out:"openside.dll" /implib:"openside.lib" /pdbtype:sept 

call ..\copy_dll.bat

popd
