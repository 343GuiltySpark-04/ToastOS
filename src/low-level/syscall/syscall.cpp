#include "../registers/Registers.hpp"
#include "../gdt/gdt.hpp"
#include "../debug.hpp"
#include "syscall.hpp"
#include "../lock.hpp"

extern "C" void syscallHandler();

typedef void (*SyscallPointer)(void);

Threading::AtomicLock syscallLock;

bool PerformingSyscall()
{
    return syscallLock.IsLocked();
}

void SyscallLock()
{
    syscallLock.Lock();
}

void SyscallUnlock()
{
    syscallLock.Unlock();
}

int64_t KNotImplemented();
size_t SyscallWrite(int fd, const void *buffer, size_t count);

SyscallPointer syscallHandlers[] =
{
    (SyscallPointer)KNotImplemented,
    (SyscallPointer)SyscallWrite,
};

int64_t KNotImplemented()
{
    DEBUG_OUT("Syscall: KNotImplemented", 0);

    return -1;
}

void InitializeSyscalls()
{
    Registers::WriteMSR(Registers::IA32_EFER, Registers::ReadMSR(Registers::IA32_EFER) | 1);

    uint64_t star = Registers::ReadMSR(Registers::IA32_STAR);
    star |= (uint64_t)GDTKernelBaseSelector << 32;
    star |= (uint64_t)GDTUserBaseSelector << 48;

    Registers::WriteMSR(Registers::IA32_STAR, star);
    Registers::WriteMSR(Registers::IA32_LSTAR, (uint64_t)&syscallHandler);
    Registers::WriteMSR(Registers::IA32_SFMASK, 0);

    DEBUG_OUT("Syscalls: IA32_EFER=0x%016lx IA32_STAR=0x%016lx IA32_LSTAR=0x%016lx",
        Registers::ReadMSR(Registers::IA32_EFER),
        Registers::ReadMSR(Registers::IA32_STAR),
        Registers::ReadMSR(Registers::IA32_LSTAR));
}
