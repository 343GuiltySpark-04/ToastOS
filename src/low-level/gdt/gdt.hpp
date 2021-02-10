#pragma once
#include <stdint.h>

#define GDTAccessDPL(n) (n << 5)

namespace GDTAccessFlag
{
    enum GDTAccessFlag
    {
        ReadWrite = (1 << 1),
        DC = (1 << 2),
        Execute = (1 << 3),
        Segments = (1 << 4),
        Present = (1 << 7)
    };
}

#define GDTAccessKernelCode (GDTAccessFlag::ReadWrite | GDTAccessFlag::Execute | GDTAccessFlag::Segments | GDTAccessFlag::Present)
#define GDTAccessKernelData (GDTAccessFlag::ReadWrite | GDTAccessFlag::Segments | GDTAccessFlag::Present)
#define GDTAccessUserCode (GDTAccessFlag::ReadWrite | GDTAccessFlag::Execute | GDTAccessFlag::Segments | GDTAccessDPL(3) | GDTAccessFlag::Present)
#define GDTAccessUserData (GDTAccessFlag::ReadWrite | GDTAccessFlag::Segments | GDTAccessDPL(3) | GDTAccessFlag::Present)

struct GDTDescriptor {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct GDTEntry {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMiddle;
    uint8_t accessFlag;
    uint8_t limitFlags;
    uint8_t baseHigh;
}__attribute__((packed));

struct GDT {
    GDTEntry null; //0x00
    GDTEntry kernelCode; //0x08
    GDTEntry kernelData; //0x10
    GDTEntry userNull;
    GDTEntry userCode;
    GDTEntry userData;
} __attribute__((packed)) 
__attribute((aligned(0x1000)));

extern GDT DefaultGDT;

extern "C" void LoadGDT(GDTDescriptor* gdtDescriptor);