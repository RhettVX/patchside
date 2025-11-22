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