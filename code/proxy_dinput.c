/*#define WIN32_LEAN_AND_MEAN 1*/
#define _CRT_SECURE_NO_WARNINGS 1
#define UNICODE 1
#include <windows.h>
#include <stdio.h>

//static HANDLE global_stdout_handle;

typedef HRESULT DirectInput8Create_type(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

HRESULT
DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
        {
        HMODULE dinput_module = LoadLibrary(L"dinput8_real.dll");
        if (dinput_module == 0) {
                MessageBox(0, L"Unable to load original module", 0, MB_OK | MB_ICONEXCLAMATION);
                ExitProcess(1);
                }
        DirectInput8Create_type* DirectInput8Create_orig = (DirectInput8Create_type*)GetProcAddress(dinput_module, "DirectInput8Create");
        if (DirectInput8Create_orig == 0) {
                MessageBox(0, L"Unable to load original procedure", 0, MB_OK | MB_ICONEXCLAMATION);
                ExitProcess(1);
                }

        return DirectInput8Create_orig(hinst, dwVersion, riidltf, ppvOut, punkOuter);
        }

BOOL WINAPI
DllMain(HINSTANCE dll_instance, DWORD reason, LPVOID reserved)
        {
        if (reason == DLL_PROCESS_ATTACH) {
                /*AllocConsole();
                global_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
                freopen("CONOUT$", "w", stdout);
                puts("yo\n");
                */
                }
        
        return TRUE;
        }