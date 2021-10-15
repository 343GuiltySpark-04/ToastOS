#pragma once

#include <stdint.h>
#include <signal.h>
#include "elf/elf.hpp"
#include "lock.hpp"
#include "interrupts/Interrupts.hpp"

#define PROCESS_STACK_SIZE 1024

enum ProcessPermissionLevel
{
    PROCESS_PERMISSION_KERNEL,
    PROCESS_PERMISSION_USER
};

struct ProcessInfo
{
    uint64_t ID;
    uint64_t permissionLevel;
    char *name;
    char **argv;
    char **environment;
    uint64_t stack[PROCESS_STACK_SIZE];
    uint64_t rip;
    uint64_t rsp;
    uint64_t cr3;
    uint64_t rflags;
    uint64_t kernelStack;
    uint64_t savedKernelStack;
    uint64_t initialUserStack;
    uint64_t sleepTicks;
    uint64_t fsBase;
    //Currently the max signal is SIGCANCEL, might need to adjust if it changes in the mlibc ABI
    sigaction sigHandlers[SIGCANCEL + 1];

    Elf::ElfHeader *elf;
};

enum ProcessState
{
    PROCESS_STATE_NEEDS_INIT = 0,
    PROCESS_STATE_IDLE,
    PROCESS_STATE_RUNNING,
};

struct ProcessControlBlock
{
    uint64_t ss;
    uint64_t rsp;
    uint64_t rflags;
    uint64_t cs;
    uint64_t rip;

    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t rdi;

    uint64_t cr3;

    uint64_t stack[PROCESS_STACK_SIZE];

    uint64_t fsBase;

    char __attribute__((aligned(16))) FXSAVE[512];

    ProcessInfo *process;

    uint64_t state;

    ProcessControlBlock *next;
};

class IScheduler
{
public:
    virtual ProcessControlBlock *CurrentProcess() = 0;
    virtual void AddProcess(ProcessInfo *process) = 0;
    virtual ProcessControlBlock *NextProcess() = 0;
    virtual void ExitProcess(ProcessInfo *process) = 0;
};

class ProcessManager
{
private:
    IScheduler *scheduler;
public:
    Threading::AtomicLock lock;

    ProcessManager(IScheduler *scheduler);

    ProcessInfo *LoadImage(const void *image, const char *name, const char **argv, const char **envp, uint64_t permissionLevel);
    ProcessInfo *CreateFromEntryPoint(uint64_t entryPoint, const char *name, uint64_t permissionLevel);

    ProcessInfo *CurrentProcess();

    void SwitchProcess(InterruptStack *stack, bool fromTimer);

    bool IsLocked();

    void LoadFSBase(uint64_t base);

    void Sigaction(int signum, sigaction *act, sigaction *oldact);
};

extern ProcessManager *globalProcessManager;

extern "C" void ProcessYield();
