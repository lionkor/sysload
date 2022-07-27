#include <sysload/sysload.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <unistd.h>

template<typename T>
static inline T read_file(const std::string& filename) {
    static_assert(sizeof(typename T::value_type) == 1, "read_file must use container holding bytes");
    auto file = std::fopen(filename.c_str(), "rb");
    if (!file) {
        throw std::runtime_error(std::strerror(errno));
    }
    const auto page_size = sysload::detail::page_size();
    size_t real_size = 0;
    T data {};
    // i counts pages used
    for (size_t i = 1;; ++i) {
        // allocate another page
        data.resize(i * page_size);
        // write to the page, by pointing to it's beginnig (end - page_size)
        auto n = fread(data.data() + (i - 1) * page_size, 1, page_size, file);
        // fail to read, consider it done
        if (n < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
        real_size += n;
        // if we didnt read a whole page, we're done :)
        if (n < page_size) {
            break;
        }
    }
    // set data.size() to the right size for the caller
    data.resize(real_size);
    return data;
}

std::vector<uint8_t> sysload::cmdline(pid_t pid) {
    return read_file<std::vector<uint8_t>>("/proc/" + detail::pid_str(pid) + "/cmdline");
}

sysload::ProcStat sysload::stat(pid_t pid) {
    ProcStat s;
    const auto str = read_file<std::string>("/proc/" + detail::pid_str(pid) + "/stat");
    auto n = ::sscanf(str.data(), "%d " // pid
                                  "%*s " // comm
                                  "%c " // state
                                  "%d " // ppid
                                  "%d " // pgrp
                                  "%d " // session
                                  "%d " // tty_nr
                                  "%d " // tpgid
                                  "%u " // flags
                                  "%lu " // minflt
                                  "%lu " // cminflt
                                  "%lu " // majflt
                                  "%lu " // cmajflt
                                  "%lu " // utime
                                  "%lu " // stime
                                  "%ld " // cutime
                                  "%ld " // cstime
                                  "%ld " // priority
                                  "%ld " // nice
                                  "%ld " // num_threads
                                  "%ld " // itrealvalue
                                  "%llu " // starttime
                                  "%lu " // vsize
                                  "%*ld " // rss
                                  "%lu " // rsslim
                                  "%lu " // startcode
                                  "%lu " // endcode
                                  "%lu " // startstack
                                  "%lu " // kstkesp
                                  "%lu " // kstkeip
                                  "%*lu " // signal
                                  "%*lu " // blocked
                                  "%*lu " // sigignore
                                  "%*lu " // sigbatch
                                  "%lu " // wchan
                                  "%*lu " // nswap
                                  "%*lu " // cnswap
                                  "%d " // exit_signal
                                  "%d " // processor
                                  "%u " // rt_priority
                                  "%u " // policy
        ,
        &s.pid, &s.state, &s.ppid, &s.pgrp, &s.session, &s.tty_nr, &s.tpgid, &s.flags,
        &s.minflt, &s.cminflt, &s.majflt, &s.cmajflt, &s.utime, &s.stime, &s.cutime, &s.cstime,
        &s.priority, &s.nice, &s.num_threads, &s.itrealvalue, &s.starttime, &s.vsize, &s.rsslim,
        &s.startcode, &s.endcode, &s.startstack, &s.kstesp, &s.kstkeip, &s.wchan, &s.exit_signal,
        &s.processor, &s.rt_priority, &s.policy);
    if (n != 33) {
        throw std::runtime_error(std::strerror(errno));
    }
    return s;
}
