#include "PartitionManager.hpp"
#include "filesystems/VFS.hpp"
#include "elf/elf.hpp"
#include "debug.hpp"
#include <stdio.h>

frg::manual_box<PartitionManager> globalPartitionManager;

void PartitionManager::Initialize()
{
    frg::vector<GenericDevice *, frg_allocator> diskDevices = globalDeviceManager.GetDevices(DEVICE_TYPE_DISK);

    printf("Found %i devices\n", diskDevices.size());

    for(uint32_t i = 0; i < diskDevices.size(); i++)
    {
        if(diskDevices[i] == NULL)
        {
            continue;
        }

        DiskInfo diskInfo;
        diskInfo.device = (GenericIODevice *)diskDevices[i];

        printf("Getting partition table for %u\n", i);

        diskInfo.table = new GPT::PartitionTable(diskInfo.device);

        if(!diskInfo.table->Parse())
        {
            printf("Failed to parse partitions for %u\n", i);

            delete diskInfo.table;

            continue;
        }

        printf("Added %u partitions for %u\n", diskInfo.table->PartitionCount(), i);

        disks.push_back(diskInfo);
    }

    for(uint32_t i = 0; i < disks.size(); i++)
    {
        DiskInfo &disk = disks[i];

        printf("Partitions for disk %s:\n", disk.device->name());

        for(uint32_t j = 0; j < disk.table->PartitionCount(); j++)
        {
            GPT::Partition &partition = disk.table->GetPartition(j);

            printf("\t%s with size: %s, type: %s)\n", partition.GetID().ToString(), partition.SizeString(), partition.GetType().ToString());

/*
            if(disk.fileSystem != NULL)
            {
                printf("Found volume %s\n", disk.fileSystem->VolumeName());

                //TODO: Dynamic mount points
                vfs->AddMountPoint("/", disk.fileSystem);

                //disk.fileSystem->DebugListDirectories();
            }
*/
        }
    }
}
