#pragma once

#define S_IFMT 0x0F000
#define S_IFBLK 0x06000
#define S_IFCHR 0x02000
#define S_IFIFO 0x01000
#define S_IFREG 0x08000
#define S_IFDIR 0x04000
#define S_IFLNK 0x0A000
#define S_IFSOCK 0x0C000

#define S_IRWXU 0700
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXG 070
#define S_IRGRP 040
#define S_IWGRP 020
#define S_IXGRP 010
#define S_IRWXO 07
#define S_IROTH 04
#define S_IWOTH 02
#define S_IXOTH 01
#define S_ISUID 04000
#define S_ISGID 02000
#define S_ISVTX 01000

#define S_IREAD  S_IRUSR
#define S_IWRITE S_IWUSR
#define S_IEXEC  S_IXUSR