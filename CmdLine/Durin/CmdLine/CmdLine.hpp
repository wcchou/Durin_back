#ifndef DURIN_CMD_LINE_CMD_LINE_HPP
#define DURIN_CMD_LINE_CMD_LINE_HPP

#include <Durin/Config/Config.hpp>

#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <Durin/CmdLine/Commands.hpp>

namespace Durin {
namespace CmdLine {

enum class TaskResult {
    Done,
    Failed,
    Continue
};

class DURIN_EXPORT CmdLineContext {
public:
    explicit
    CmdLineContext( boost::program_options::variables_map& vm )
        : vm_( vm )
    {
    }

    bool has( const char* optionName )
    {
        return vm_.count( optionName );
    }

    template <typename T>
    T getOptionValue(
        const char* optionName,
        T defaultValue )
    {
        auto pos = vm_.find( optionName );
        if ( vm_.end() == pos ) {
            return std::move( defaultValue );
        }

        return pos->second.as<T>();
    }

    std::vector<std::string> getOptionValues(
        const char* optionName,
        const char* delimiter );

    boost::filesystem::path getConfigFilePath();

    /*!
     * \brief Get output path from --output option
     */
    boost::filesystem::path getOutputPath()
    {
        return getOptionValue<std::string>( OutputCmd, "output" );
    }

    template <typename SizeT>
    bool getFileSize( SizeT& o_result )
    {
        if ( !has( SizeCmd ) ) {
            return false;
        }

        struct UnitKind {
            const char* unit;
            SizeT nBytes;
        } kinds[] = {
            { "kb", 1024 },
            { "mb", 1024 * 1024 },
            { "gb", 1024 * 1024 * 1024 }
        };

        std::string sizeStr = vm_[ SizeCmd ].as<std::string>();
        std::transform(
            sizeStr.begin(), sizeStr.end(),
            sizeStr.begin(),
            []( unsigned char ch ) {
                return std::tolower( ch );
            } );

        std::string::size_type unitPos;
        for ( const auto& kind : kinds ) {
            auto unitPos = sizeStr.find( kind.unit );
            if ( std::string::npos == unitPos ) {
                continue;
            }

            try {
                o_result = boost::lexical_cast<SizeT>( sizeStr.data(), unitPos ) * kind.nBytes;
                return true;
            }
            catch ( std::exception& ex ) {
                //cout << ex.what() << endl;
                return false;
            }
        }

        try {
            o_result = boost::lexical_cast<SizeT>( sizeStr );
            return true;
        }
        catch ( std::exception& ex ) {
            //cout << ex.what() << endl;
            return false;
        }

        return false;
    }

private:
    boost::program_options::variables_map&  vm_;
};

using CmdHandler = TaskResult (*)( CmdLineContext& );

} // namespace CmdLine
} // namespace Durin

#endif // End of include guard
