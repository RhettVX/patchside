/*#define WIN32_LEAN_AND_MEAN 1*/
#define _CRT_SECURE_NO_WARNINGS 1
#define UNICODE 1
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "base.h"
#include "hook86.h"

static HANDLE global_stdout_handle;

typedef HRESULT DirectInput8Create_type(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

void print_stack(void) {
        u32 local_var = 0x1337;
        u32* ebp_ptr = ((uptr)&local_var)  ;
        printf("ebp-4 %p: %x\n", &local_var, local_var);
        printf('ebp   (prev sf) %p: %x\n", ')
        }

HRESULT
DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
        {
        static HMODULE dinput_module;
        print_stack();
        if (dinput_module == 0)
                dinput_module = LoadLibrary(L"C:\\Windows\\SysWOW64\\dinput8.dll");
        if (dinput_module == 0) {
                MessageBox(0, L"Unable to load original module", 0, MB_OK | MB_ICONEXCLAMATION);
                ExitProcess(1);
                }
        static DirectInput8Create_type* DirectInput8Create_orig;
        if (DirectInput8Create_orig == 0)
                DirectInput8Create_orig = (DirectInput8Create_type*)GetProcAddress(dinput_module, "DirectInput8Create");
        if (DirectInput8Create_orig == 0) {
                MessageBox(0, L"Unable to load original procedure", 0, MB_OK | MB_ICONEXCLAMATION);
                ExitProcess(1);
                }

        return DirectInput8Create_orig(hinst, dwVersion, riidltf, ppvOut, punkOuter);
        }

#define UPDATE_INPUT_STATE(name) int __stdcall name(void)
typedef UPDATE_INPUT_STATE(update_input_state_type);
static update_input_state_type* update_input_state_orig;
UPDATE_INPUT_STATE(update_input_state_hook) {
        static u32 counter;
        printf("%u UpdateInputState called\n", counter++);
        return update_input_state_orig();
        }

#define FUN_00415c80(name) void __fastcall name(u32 this, u32 xx, u32* param_1)
typedef FUN_00415c80(fun_00415c80_type);
static fun_00415c80_type* fun_00415c80_orig;
FUN_00415c80(test_hook) {
        i32 counter = 0;
        i32* x = &param_1[1];
        i32* y = &param_1[2];

        if (*x > -4 && *x < 0)
                *x = -5;
        else if (*x > 0 && *x < 4)
                *x = 5;

        if (*y > -4 && *y < 0)
                *y = -5;
        else if (*y > 0 && *y < 4)
                *y = 5;

        printf("[%d] %+d\n", counter, param_1[counter]);
        counter += 1;
        printf("[%d] X %+d\n", counter, param_1[counter]);
        counter += 1;
        printf("[%d] Y %+d\n", counter, param_1[counter]);
        counter += 1;
        printf("[%d] %+d\n", counter, param_1[counter]);
        counter += 1;
        fun_00415c80_orig(this, xx, param_1);
        }

#define ATTEMPT_3(name) void __fastcall name(void* this, void* xx, u32* param_1)
typedef ATTEMPT_3(attempt_3_type);
static attempt_3_type* attempt_3_orig;
ATTEMPT_3(attempt_3_hook) {
        i32 counter = 0;
        i32* x = &param_1[1];
        i32* y = &param_1[2];

        /*
        if (*x > -4 && *x < 0)
                *x = -5;
        else if (*x > 0 && *x < 4)
                *x = 5;

        if (*y > -4 && *y < 0)
                *y = -5;
        else if (*y > 0 && *y < 4)
                *y = 5;
        */

        //printf("[%d] %+d\n", counter, param_1[counter]);
        counter += 1;
        printf("[%d] X %+d\n", counter, param_1[counter]);
        counter += 1;
        printf("[%d] Y %+d\n", counter, param_1[counter]);
        counter += 1;
        //printf("[%d] %+d\n", counter, param_1[counter]);
        counter += 1;

        *x = 0;
        *y = 0;
        attempt_3_orig(this, xx, param_1);
        }



#define ATTEMPT_4(name) void __fastcall name(void* this, void* xx, u32* param_1)
typedef ATTEMPT_4(attempt_4_type);
static attempt_4_type* attempt_4_orig;
ATTEMPT_4(attempt_4_hook) {
        i32 counter = 0;
        i32* x = &param_1[1];
        i32* y = &param_1[2];

        /*
        if (*x > -4 && *x < 0)
                *x = -5;
        else if (*x > 0 && *x < 4)
                *x = 5;

        if (*y > -4 && *y < 0)
                *y = -5;
        else if (*y > 0 && *y < 4)
                *y = 5;
        */

        //printf("[%d] %+d\n", counter, param_1[counter]);
        counter += 1;
        printf("[%d] X %+d\n", counter, param_1[counter]);
        counter += 1;
        printf("[%d] Y %+d\n", counter, param_1[counter]);
        counter += 1;
        //printf("[%d] %+d\n", counter, param_1[counter]);
        counter += 1;

        // is it safe to apply the patch now that we made it here?
        u32 code_cave_size = 4096;
        u8* code_cave = VirtualAlloc(0, code_cave_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        unsigned char hexData[46] = {
            0x8B, 0x86, 0x44, 0x01, 0x00, 0x00, 0xD9, 0x80, 0xAC, 0x00, 0x00, 0x00, 0xDA, 0x4C, 0x24, 0x24,
            0xDF, 0x7C, 0x24, 0x18, 0x8B, 0x4C, 0x24, 0x18, 0x89, 0x4C, 0x24, 0x24, 0xD9, 0x80, 0xAC, 0x00,
            0x00, 0x00, 0xDA, 0x4C, 0x24, 0x28, 0xDF, 0x7C, 0x24, 0x18, 0x8B, 0x44, 0x24, 0x18 
        };
        memcpy(code_cave, hexData, sizeof hexData);

        u8* target = (u8*)0x00422267;
        u32 target_size = 46;
        //nop_mem(target, target_size);
        hook_jump_32(target, target_size, (uptr)code_cave);
        hook_jump_32(code_cave + sizeof hexData, 5, (uptr)target + target_size);

        //*x = 1;
        //*y = 0;
        attempt_4_orig(this, xx, param_1);
        }

#define LOG(name) void __cdecl name(char const* format, ...)
typedef LOG(log_type);
static log_type* log_orig;
LOG(log_hook) {
        }

BOOL WINAPI
DllMain(HINSTANCE dll_instance, DWORD reason, LPVOID reserved)
        {
        if (reason == DLL_PROCESS_ATTACH) {
                AllocConsole();
                //global_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
                freopen("CONOUT$", "w", stdout);

                //update_input_state_orig = hook_trampoline_32((u8*)0x00415a80, 5, (uptr)update_input_state_hook);
                //fun_00415c80_orig = hook_trampoline_32((u8*)0x00415c80, 8, (uptr)test_hook);
                //attempt_3_orig = hook_trampoline_32((u8*)0x00422030, 5, (uptr)attempt_3_hook);
                //log_orig = hook_trampoline_32((u8*)0x00868c90, 5, (uptr)log_hook);
                attempt_4_orig = hook_trampoline_32((u8*)0x00572680, 5, (uptr)attempt_4_hook);
                }
        
        return TRUE;
        }