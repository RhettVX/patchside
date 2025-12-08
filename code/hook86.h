void
nop_mem(u8* target, u32 size) {
        DWORD old_protect;
        VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &old_protect);
        memset(target, 0x90, size);
        VirtualProtect(target, size, old_protect, &old_protect);
        }

void
hook_jump_32(u8* target, u32 target_size, uptr dest) {
        u8 jmp_stub[5] = { 0xe9, 0, 0, 0, 0 };
        ASSERT(target_size >= sizeof jmp_stub);
        DWORD old_protect;
        VirtualProtect(target, target_size, PAGE_EXECUTE_READWRITE, &old_protect);
        memset(target, 0x90, target_size);
        target[0] = jmp_stub[0];
        *(u32*)((uptr)target + 1) = dest - ((u32)target + sizeof jmp_stub);
        VirtualProtect(target, target_size, old_protect, &old_protect);
        }

void*
hook_trampoline_32(u8* target, u32 size, uptr our_func) {
        u8 jmp_stub[] = { 0xe9, 0, 0, 0, 0 };
        ASSERT(size >= sizeof jmp_stub);

        u32 gateway_size = size + sizeof jmp_stub;
        u8* gateway = VirtualAlloc(0, gateway_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        DWORD old_protect;
        VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &old_protect);

        memcpy(gateway, target, size);
        gateway[size] = jmp_stub[0];
        // if (gateway > target)
        *(u32*)((uptr)gateway + size + 1) = ((u32)target + size) - ((u32)gateway + size + sizeof jmp_stub);
        // else
        //         *(u32*)((uptr)gateway + size + 1) = ((u32)target + size) - ((u32)gateway + size + sizeof jmp_stub);

        memset(target, 0x90, size);
        target[0] = jmp_stub[0];
        *(u32*)((uptr)target + 1) = our_func - ((u32)target + sizeof jmp_stub);
        VirtualProtect(target, size, old_protect, &old_protect);

        return gateway;
        }

#if 0 /* shelving this for now */

enum {
      COUNTER_MAX_COUNT = 32,
      COUNTER_RESERVED_CODE_SIZE = 32,
      }; 

/* count above this will require more bytes */
CT_ASSERT(COUNTER_MAX_COUNT <= 32);
CT_ASSERT(COUNTER_RESERVED_CODE_SIZE >= 32);

typedef struct Counters Counters;
struct Counters {
        u8 active_count;
        uptr entries[COUNTER_MAX_COUNT];
        u32 counters[COUNTER_MAX_COUNT];
        u8 (*code)[COUNTER_MAX_COUNT][COUNTER_RESERVED_CODE_SIZE];
        };

static Counters global_counters; 

void
counters_init(void) {
        global_counters.code = VirtualAlloc(0, sizeof *global_counters.code, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        memset(global_counters.code, 0xcc, sizeof *global_counters.code);
        }
void
hook_counter_32(u8* target, u32 target_size) {
        u8 jmp_stub[5] = {
                0xe9, 0, 0, 0, 0 /* jmp PTR */
                };
        ASSERT(target_size >= sizeof jmp_stub);
        u8 inc_stub[10] = {
                0x50,             /* push   eax */
                0xb8, 0, 0, 0, 0, /* mov    eax,PTR */
                0xff, 0x40, 0,    /* inc    DWORD PTR [eax+IDX] */
                0x58              /* pop    eax */
                };
        u8 index = global_counters.active_count;
        ASSERT(index < 128);
        /* prepare cave */
        u8* cave = *global_counters.code[index];
        memcpy(cave, inc_stub, sizeof inc_stub);
        *(uptr*)(&inc_stub[2]) = (uptr)&global_counters.counters[index];
        cave[8] = index;
        memcpy(&cave[sizeof inc_stub], target, target_size);
        hook_jump_32(&cave[sizeof inc_stub + target_size], 5, (uptr)target + sizeof jmp_stub);

        /* hook target */
        hook_jump_32(target, target_size, (uptr)cave);

        global_counters.entries[index] = (uptr)target;
        global_counters.active_count += 1;
        }

void
counters_print(void) {
        u32 i = 0;
        while (i < global_counters.active_count) {
                printf("[0x%08x]  %16u\n", global_counters.entries[i], global_counters.counters[i]);
                i += 1;
                }
        }
#endif