#pragma once
#include "Paging.hpp"

class PageTableManager
{
public:
    PageTableManager(PageTable* PML4Address);
    PageTable* PML4;
    void mapMemory(void* virtualMemory, void* physicalMemory);
    void unmapMemory(void* virtualMemory);
    void identityMap(void *physicalMemory);
};

extern PageTableManager *globalPageTableManager;
