void trampoline_hook_32(u8* target, u32 size, uptr our_func) {
        u8 jmp_stub[] = { 0xe9, 0, 0, 0, 0 };
        ASSERT(size >= sizeof jmp_stub);

        u32 gateway_size = size + sizeof jmp_stub;
        u8* gateway = VirtualAlloc(0, gateway_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        DWORD old_protect;
        VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &old_protect);

        memcpy(gateway, target, size);
        gateway[size] = jmp_stub[0];
        *((u32*)gateway + size + 1) = (gateway + size) - (sizeof jmp_stub + target + size);

        memset(target, 0x90, size);
        target[0] = jmp_stub[0];
        *((u32*)target + 1) = target - (sizeof jmp_stub + our_func);
        VirtualProtect(target, size, old_protect, &old_protect);

        return gateway;
        }