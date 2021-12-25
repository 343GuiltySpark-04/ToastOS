#include "syscall.hpp"
#include "process/Process.hpp"
#include "debug.hpp"

int64_t SyscallGetPPID(InterruptStack *stack)
{
#if KERNEL_DEBUG_SYSCALLS
    DEBUG_OUT("Syscall: GetPPID", 0);
#endif

    ProcessInfo *process = globalProcessManager->CurrentProcess();

    return process->ppid;
}