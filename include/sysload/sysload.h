#pragma once

#include <string>
#include <unistd.h>
#include <vector>

namespace sysload {

// proc(5)
struct ProcStat {
    int pid;
    // comm is left out
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    unsigned flags;
    unsigned long minflt;
    unsigned long cminflt;
    unsigned long majflt;
    unsigned long cmajflt;
    unsigned long utime;
    unsigned long stime;
    long cutime;
    long cstime;
    long priority;
    long nice;
    long num_threads;
    long itrealvalue;
    unsigned long long starttime;
    unsigned long vsize;
    // rss itself is inaccurate and left out
    unsigned long rsslim;
    unsigned long startcode;
    unsigned long endcode;
    unsigned long startstack;
    unsigned long kstesp;
    unsigned long kstkeip;
    // signal related fiels are inaccurate and left out
    unsigned long wchan;
    // nswap is not maintained, and left out
    int exit_signal;
    int processor;
    unsigned rt_priority;
    unsigned policy;
    // linux >2.6 features are left out for now
};

std::vector<uint8_t> cmdline(pid_t);
ProcStat stat(pid_t);

namespace thread_self {
    inline pid_t tid() {
        static thread_local pid_t tid = gettid();
        return tid;
    }
}

namespace self {
    inline pid_t pid() {
        static pid_t pid = getpid();
        return pid;
    }
    inline auto cmdline() { return sysload::cmdline(pid()); }
    inline auto stat() { return sysload::stat(pid()); }
}

namespace detail {
    inline size_t page_size() {
        static size_t size = sysconf(_SC_PAGESIZE);
        return size;
    }
    inline size_t clk_tck() {
        static size_t tck = sysconf(_SC_CLK_TCK);
        return tck;
    }
    inline std::string pid_str(pid_t pid) {
        return std::to_string(pid);
    }
}

}
