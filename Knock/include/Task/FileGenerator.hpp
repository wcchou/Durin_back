#ifndef KNOCK_TASK_FILE_GENERATOR_HPP
#define KNOCK_TASK_FILE_GENERATOR_HPP

#include <Config/Config.hpp>

#include <vector>
#include <set>
#include <string>

#include <boost/filesystem.hpp>

namespace Knock {

class FileGenerator {
public:
    struct FileItem {
        std::string fileName;
        std::string sha256;
    };

    bool generateFiles( const boost::filesystem::path& dir, size_t nFiles, size_t fileSize );

private:
    bool writeReport( const boost::filesystem::path& reportPath );

    std::vector<FileItem>   files_;
    std::set<std::string>   sha256Set_;
    bool unique_ = true;
};

} // namespace Knock

#endif // End of includ eguard
