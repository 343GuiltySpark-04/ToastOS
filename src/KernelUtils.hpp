#pragma once

#include <stdint.h>
#include "framebuffer/BasicRenderer.hpp"
#include "cstring/cstring.hpp"
#include "efimemory/EFIMemory.hpp"
#include "Bitmap.hpp"
#include "paging/PageFrameAllocator.hpp"
#include "paging/PageMapIndexer.hpp"
#include "paging/Paging.hpp"
#include "paging/PageTableManager.hpp"
#include "text/psf2.hpp"
#include "serial/Serial.hpp"
#include "debug.hpp"

struct BootInfo
{
	Framebuffer* framebuffer;
	psf2_font_t* font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

struct KernelInfo
{
    PageTableManager* pageTableManager;
};

KernelInfo InitializeKernel(BootInfo* BootInfo);