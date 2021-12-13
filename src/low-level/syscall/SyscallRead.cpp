#include "syscall.hpp"
#include "keyboard/Keyboard.hpp"
#include "process/Process.hpp"
#include "filesystems/VFS.hpp"
#include "debug.hpp"
#include "errno.h"
#include "ctype.h"

using namespace FileSystem;

int64_t SyscallRead(InterruptStack *stack)
{
    int fd = stack->rsi;
    void *buffer = (void *)stack->rdx;
    size_t count = (size_t)stack->rcx;

    if(fd == 0) //stdin
    {
        while(!GotKeyboardInput())
        {
            ProcessYield();
        }

        char keyboardInput = (char)KeyboardInput();

        uint8_t *ptr = (uint8_t *)buffer;

        *ptr = keyboardInput;

        printf("%c", keyboardInput);

        return 1;
    }
    else if(fd >= 3) //actual files
    {
        FILE_HANDLE handle = fd - 3;

        if(vfs->FileType(handle) == FILE_HANDLE_UNKNOWN)
        {
            return -ENOENT;
        }

        return vfs->ReadFile(handle, buffer, count);
    }

    return 0;
}