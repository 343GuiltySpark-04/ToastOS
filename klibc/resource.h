#pragma once

#include "time.h"

#define PRIO_PROCESS 1
#define PRIO_PGRP 2
#define PRIO_USER 3

#define RLIM_INFINITY ((rlim_t)-1)
#define RLIM_SAVED_MAX ((rlim_t)-1)
#define RLIM_SAVED_CUR ((rlim_t)-1)

#define RUSAGE_SELF 1
#define RUSAGE_CHILDREN 2

#define RLIMIT_CORE 1
#define RLIMIT_CPU 2
#define RLIMIT_DATA 3
#define RLIMIT_FSIZE 4
#define RLIMIT_NOFILE 5
#define RLIMIT_STACK 6
#define RLIMIT_AS 7
#define RLIMIT_MEMLOCK 8
#define RLIMIT_RSS 9
#define RLIMIT_NPROC 10
#define RLIMIT_LOCKS 11
#define RLIMIT_SIGPENDING 12
#define RLIMIT_MSGQUEUE 13
#define RLIMIT_NICE 14
#define RLIMIT_RTPRIO 15
#define RLIMIT_NLIMITS 16
#define RLIM_NLIMITS RLIMIT_NLIMITS

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long rlim_t;

struct rlimit {
	rlim_t rlim_cur;
	rlim_t rlim_max;
};

struct rusage {
	struct timeval ru_utime;
	struct timeval ru_stime;
	long int ru_maxrss;
	long int ru_ixrss;
	long int ru_idrss;
	long int ru_isrss;
	long int ru_minflt;
	long int ru_majflt;
	long int ru_nswap;
	long int ru_inblock;
	long int ru_oublock;
	long int ru_msgsnd;
	long int ru_msgrcv;
	long int ru_nsignals;
	long int ru_nvcsw;
	long int ru_nivcsw;
};

#ifdef __cplusplus
}
#endif
