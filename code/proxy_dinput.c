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

#if 0 /* shelving this */
void print_stack(void) {
        u32 volatile local_var = 0x1337;
        u32 volatile* volatile ebp_ptr = &local_var  ;
        printf("ebp-4 %p: %x\n", &local_var, local_var);
        }
#endif

HRESULT
DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
        {
        static HMODULE dinput_module;
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

typedef char __fastcall FUN_005905d0_type(void* this, void* xx, u32 param);
static FUN_005905d0_type* FUN_005905d0 = (void*)0x005905d0;

#define ATTEMPT_4(name) void __fastcall name(void* this, void* xx, u32* param_1)
typedef ATTEMPT_4(attempt_4_type);
static attempt_4_type* attempt_4_orig;
ATTEMPT_4(attempt_4_hook) {
        static u32 counter;
        i32* x = &param_1[1];
        i32* y = &param_1[2];
        
        void* local_player_behavior = (void*)*(uptr*)((uptr)this + 0x6c);
        u32* unk_194 = (u32*)((uptr)this + 0x194);
        f32* lpb_dx = (f32*)((uptr)local_player_behavior + 0x84);
        f32* lpb_dy = (f32*)((uptr)local_player_behavior + 0x88);

        *lpb_dx *= -1; 

        if (2) {
                printf("<%u>\n", counter);
                //if (*x || *y || *lpb_dx || *lpb_dy) {
                printf("LPB DX: %f\n", *lpb_dx);
                printf("LPB DY: %f\n", *lpb_dy);
                printf("  M DX: %d\n", *x);
                printf("  M DY: %d\n", *y);
        }

        u32 use_our_code = 1;
        if (use_our_code) {
                void* puVar1 = (void*)((uptr)this + 0x194);
                char cVar5 = FUN_005905d0(puVar1, xx, 9);
                if (cVar5 == 0) {
                        cVar5 = FUN_005905d0(puVar1, xx, 0x29);
                        if (cVar5 == 0) {
                                cVar5 = FUN_005905d0(puVar1, xx, 0x33);
                                if (cVar5 == 0) {
                                        /* todo: other float checks for valid value */
                                        if (*lpb_dx != 0.0f || *lpb_dy != 0.0f) {
                                                *(u32*)puVar1 = *(u32*)puVar1 | 4;
                                                }
                                        else {
                                                *(u32 *)puVar1 = *(u32 *)puVar1 & 0xfffffffb;
                                                }
                                        }
                                }
                        }
                }

        // is it safe to apply the patch now that we made it here?
        static u32 hook_once;
        if (hook_once == 0) {
                u32 code_cave_size = 4096;
                u8* code_cave = VirtualAlloc(0, code_cave_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
                Stream cave_stream = ZERO_STRUCT;
                cave_stream.data = code_cave;
                cave_stream.size = code_cave_size;

                hook_once = 1;
                //------------------------------------------------------------
                //-----------       Created with 010 Editor        -----------
                //------         www.sweetscape.com/010editor/          ------
                //
                // File    : D:\Projects\openside\build_shellcode\mouse_patch.obj
                // Address : 260 (0x104)
                // Size    : 85 (0x55)
                //------------------------------------------------------------
                unsigned char hexData[85] = {
                    0x8B, 0x8E, 0x1C, 0x01, 0x00, 0x00, 0x8B, 0x89, 0x88, 0x00, 0x00, 0x00, 0x8B, 0x49, 0x6C, 0x8B,
                    0x86, 0x44, 0x01, 0x00, 0x00, 0xD9, 0x80, 0xAC, 0x00, 0x00, 0x00, 0xDA, 0x4C, 0x24, 0x24, 0xD9,
                    0x91, 0x84, 0x00, 0x00, 0x00, 0xDF, 0x7C, 0x24, 0x18, 0x8B, 0x44, 0x24, 0x18, 0x89, 0x44, 0x24,
                    0x24, 0x8B, 0x86, 0x44, 0x01, 0x00, 0x00, 0xD9, 0x80, 0xAC, 0x00, 0x00, 0x00, 0xDA, 0x4C, 0x24,
                    0x28, 0xD9, 0x91, 0x88, 0x00, 0x00, 0x00, 0xDF, 0x7C, 0x24, 0x18, 0x8B, 0x44, 0x24, 0x18, 0x89,
                    0x44, 0x24, 0x28, 0x8B, 0xC8 
                };

                memcpy(STREAM_AT(cave_stream), hexData, sizeof hexData);
                cave_stream.cursor += sizeof hexData;

                u8* target = (u8*)0x00422267;
                u32 target_size = 46;
                hook_jump_32(target, target_size, (uptr)code_cave);
                hook_jump_32(STREAM_AT(cave_stream), 5, (uptr)target + target_size);


                //nop_mem((u8*)0x00748c80, 1307);
        }
        counter += 1;
        if (use_our_code == 0)
                attempt_4_orig(this, xx, param_1);
        }

typedef void* LocalPlayerBehavior;
#define DOES_MOUSE_MATH(name) u32 __fastcall name(LocalPlayerBehavior* this, void* xx, f32 dt)
typedef DOES_MOUSE_MATH(does_mouse_math_type);
static does_mouse_math_type* does_mouse_math_orig;
DOES_MOUSE_MATH(does_mouse_math_hook) {
        printf("dt: %f\n", dt);
        return does_mouse_math_orig(this, xx, dt);
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
                does_mouse_math_orig = hook_trampoline_32((u8*)0x00748920, 6, (uptr)does_mouse_math_hook);
                }
        
        return TRUE;
        }