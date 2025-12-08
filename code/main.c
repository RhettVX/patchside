#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <stdio.h>

#include "base.h"

static void*
memory_alloc(uptr size)
{
    static HANDLE heap_handle;
    if (!heap_handle)
    {
        heap_handle = GetProcessHeap();
    }
    return HeapAlloc(heap_handle, HEAP_ZERO_MEMORY, size);
}

/* Game Structs */
typedef struct AutoClass1 
	{
	u32 f1;
	u32 f2;
	u32 f3;
	} AutoClass1;

/* Game Procs */
typedef u32 __fastcall FUN_0084AA40_type(void* this, u32 _unused, u32 a1);
static FUN_0084AA40_type* FUN_0084AA40 = (void*)0x0084aa40;

typedef void FUN_00893870_type(u32* arg1, char* arg2);
static FUN_00893870_type* FUN_00893870 = (void*)0x00893870;

typedef void FUN_0084ad80_type(void);
static FUN_0084ad80_type* FUN_0084ad80 = (void*)0x0084ad80;

typedef void FUN_00a6b2c9_type(u32 arg1, u32 arg2);
static FUN_00a6b2c9_type* FUN_00a6b2c9 = (void*)0x00a6b2c9;

typedef void __fastcall FUN_00404fb0_type(AutoClass1* this, u32 _unused, char* arg1, u8* arg2);
static FUN_00404fb0_type* FUN_00404fb0 = (void*)0x00404fb0;

/* Static Data */
static u32* DAT_00d20364 = (void*)0x00d20364;
static char* DAT_00cce178 = (void*)0x00cce178;
static HINSTANCE* _DAT_00cce16c = (void*)0x00cce16c;

int WINAPI
openside_win_main(HINSTANCE instance,
                  HINSTANCE prev_instance,
                  LPSTR cmd_line,
                  int cmd_show)
{
char buffer[128] = {0};
    void* pvVar4 = memory_alloc(0x420);
	AutoClass1 auStack_390 = {0};
	u8* auStack_3a0 = 0;

    *DAT_00d20364 = FUN_0084AA40(pvVar4, 0, 0);
	DAT_00d20364[2] |= 1;
	DAT_00d20364[2] |= 4;

	FUN_00893870(DAT_00d20364, "Output Log File Opened in directory: %s\n");
	FUN_0084ad80();
	FUN_00a6b2c9(0x300, 0x300);
	*_DAT_00cce16c = instance;
	GetCurrentDirectoryA(0x400, DAT_00cce178);
	FUN_00404fb0(&auStack_390, 0, "planetside_crash.log", auStack_3a0 + 4);

    
    MessageBoxA(0, DAT_00cce178, "OpenSide", MB_OK);
    return 0;
}