#ifndef KNOCK_TASK_PROCESS_UTIL_HPP
#define KNOCK_TASK_PROCESS_UTIL_HPP

#include <Config/Config.hpp>

#include <string>
#include <vector>

namespace Knock {

struct ProcessInfo {
    std::string                 pid{ 0 };
    std::string                 name;
    std::vector<std::string>    cmdLine;
    int                         seccomp{ 0 };
};

class ProcessFinder {
public:
    using ProcessListT = std::vector<ProcessInfo>;

    static ProcessListT query();
};

void show(
    const ProcessFinder::ProcessListT& processes,
    const std::vector<std::string>& filters,
    const std::vector<std::string>& fields );

} // namespace Knock

#endif // End of include guard
