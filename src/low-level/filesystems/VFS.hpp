#pragma once

#include <stdint.h>

#include "FileSystem.hpp"
#include <frg/manual_box.hpp>
#include <frg/vector.hpp>
#include <frg_allocator.hpp>

struct ProcessInfo;

typedef uint64_t FILE_HANDLE;

struct VirtualFile
{
    frg::string<frg_allocator> path;
    FileHandleType type;

    uint64_t (*length)();
    uint64_t (*read)(void *buffer, uint64_t cursor, uint64_t size);
    uint64_t (*write)(const void *buffer, uint64_t cursor, uint64_t size);

    uint64_t Length() const;
    uint64_t Read(void *buffer, uint64_t cursor, uint64_t size);
    uint64_t Write(const void *buffer, uint64_t cursor, uint64_t size);
};

class VFS
{
private:
    struct MountPoint
    {
        const char *path;
        FileSystem *fileSystem;
    };

    struct FileHandle
    {
        MountPoint *mountPoint;
        frg::string<frg_allocator> path;
        FileSystemHandle fsHandle;
        uint64_t length;
        uint64_t cursor;
        bool isValid;
        uint32_t flags;
        int fileType;
        uint64_t ID;
        struct stat stat;
        VirtualFile *virtualFile;
    };

    frg::vector<MountPoint *, frg_allocator> mountPoints;
    frg::vector<FileHandle, frg_allocator> fileHandles;
    frg::vector<VirtualFile *, frg_allocator> virtualFiles;
    uint64_t fileHandleCounter;

    FileHandle *GetFileHandle(FILE_HANDLE handle);
    FileHandle *NewFileHandle();
    FileHandle *ResolveSymlink(FileHandle *original, uint32_t flags);
public:
    VFS();
    void AddMountPoint(const char *path, FileSystem *fileSystem);
    void RemoveMountPoint(const char *path);

    void AddVirtualFile(const VirtualFile &file);
    void RemoveVirtualFile(const frg::string<frg_allocator> &path);

    FILE_HANDLE OpenFile(const char *path, uint32_t flags, ProcessInfo *currentProcess);
    void CloseFile(FILE_HANDLE handle);
    int FileType(FILE_HANDLE handle);
    uint32_t FileFlags(FILE_HANDLE handle);
    void SetFileFlags(FILE_HANDLE handle, uint32_t flags);

    uint64_t FileLength(FILE_HANDLE handle);
    uint64_t FileOffset(FILE_HANDLE handle);
    uint64_t ReadFile(FILE_HANDLE handle, void *buffer, uint64_t length);
    uint64_t WriteFile(FILE_HANDLE handle, const void *buffer, uint64_t length);
    uint64_t SeekFile(FILE_HANDLE handle, uint64_t cursor);
    uint64_t SeekFileBegin(FILE_HANDLE handle);
    uint64_t SeekFileEnd(FILE_HANDLE handle);

    frg::string<frg_allocator> GetFilePath(FILE_HANDLE handle);

    dirent *ReadEntries(FILE_HANDLE handle);
    void CloseDir(FILE_HANDLE handle);

    stat Stat(FILE_HANDLE handle);
};

extern frg::manual_box<VFS> vfs;

void InitializeVirtualFiles();
