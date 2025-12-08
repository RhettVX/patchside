@echo off
setlocal

if not exist build_shellcode mkdir build_shellcode
pushd build_shellcode
ml /c ..\code\mouse_patch.asm /nologo /W3 /WX
popd

endlocal