#include "syscall.hpp"
#include "process/Process.hpp"
#include "debug.hpp"

int64_t SyscallSpawnThread(InterruptStack *stack)
{
    uint64_t rip = stack->rsi;
    uint64_t rsp = stack->rdx;

#if KERNEL_DEBUG_SYSCALLS
    DEBUG_OUT("Syscall: spawn thread rip: %p rsp: %p", rip, rsp);
#endif

    auto process = globalProcessManager->CurrentProcess();
    
    if(process == NULL || process->isValid == false)
    {
        return 0;
    }

    auto pcb = globalProcessManager->AddThread(rip, rsp);

    if(pcb == NULL)
    {
        return -EAGAIN;
    }

    return pcb->tid;
}