#include "Timer.hpp"
#include "ports/Ports.hpp"
#include "debug.hpp"
#include <stdlib.h>

#define TIMER_FREQUENCY 50 // in Hz
#define TIMER_QUOTIENT  1193180

#define PIT_0           0x40
#define PIT_1           0x41
#define PIT_2           0x42
#define PIT_CMD         0x43
#define PIT_SET         0x36

static uint32_t currentTick;

Timer timer;

void timerCallback(InterruptStack *stack)
{
    currentTick++;

    timer.RunHandlers(stack);
}

void Timer::Initialize()
{
    handlers = (DynamicArray<void *> *)malloc(sizeof(DynamicArray<void *>));

    interrupts.RegisterHandler(IRQ0, timerCallback);

    uint32_t divisor = TIMER_QUOTIENT / TIMER_FREQUENCY;

    outport8(PIT_CMD, PIT_SET);
    outport8(PIT_0, divisor & 0xFF);
    outport8(PIT_0, (divisor >> 8) & 0xFF);
}

void Timer::RunHandlers(InterruptStack *stack)
{
    if(handlers == NULL)
    {
        return;
    }
    
    for(uint32_t i = 0; i < handlers->length(); i++)
    {
        void (*handler)(InterruptStack *) = (void (*)(InterruptStack *))(*handlers)[i];

        handler(stack);
    }
}

void Timer::RegisterHandler(void (*callback)(InterruptStack *))
{
    if(handlers == NULL)
    {
        return;
    }

    handlers->add((void *)callback);
}

void Timer::UnregisterHandler(void (*callback)(InterruptStack *))
{
    if(handlers == NULL)
    {
        return;
    }
    
    handlers->remove((void *)callback);
}

uint32_t Timer::GetTicks()
{
    return currentTick;
}
