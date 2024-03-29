#include <Config/Config.hpp>

#include <Task/ProcessUtil.hpp>

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string_view>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <Durin/String/String.hpp>

namespace fs = boost::filesystem;

namespace {

std::vector<std::string> parseProcessStatus( const std::string_view& value )
{
    using namespace std;

    string_view::size_type pos = value.find( ":\t" );
    if ( string_view::npos == pos ) {
        return {};
    }

    std::vector<std::string>    result;
    result.emplace_back( value.substr( 0, pos ) );

    pos = value.find_first_not_of( " \t", pos + 2 );
    if ( string_view::npos != pos ) {
        result.emplace_back( value.substr( pos ) );
    }

    return result;
}

Knock::ProcessInfo getProcessInfo( const boost::filesystem::path& path )
{
    using namespace std;

    ifstream in( (path / "status").string(), ios::in );
    if ( !in ) {
        return {};
    }

    Knock::ProcessInfo proc;

    string value;
    while ( getline( in, value ) ) {
        std::vector<std::string> status{ parseProcessStatus( value ) };
        if ( status.empty() ) {
            continue;
        }

        if ( status.size() != 2 ) {
            continue;
        }

        if ( status[ 0 ] == "Name" ) {
            proc.name = status[ 1 ];
        }
        else if ( status[ 0 ] == "Seccomp" ) {
            proc.seccomp = boost::lexical_cast<int>( status[ 1 ] );
        }
    }
    in.close();

    in.open( (path / "cmdline").string(), ios::in );
    if ( in ) {
        std::string oneLineValues;
        getline( in, oneLineValues );

        // std::istringstream iss( oneLineValues );
        // while ( iss ) {
        //     std::string value;
        //     iss >> value;
        //     proc.cmdLine.push_back( value );
        // }
        boost::algorithm::split( proc.cmdLine, oneLineValues, boost::is_cntrl() );
    }
    proc.cmdLine.pop_back();

    return proc;
}

} // unnamed

namespace Knock {

std::vector<ProcessInfo> ProcessFinder::query()
{
    std::vector<ProcessInfo>    processes;

    for ( const fs::directory_entry& entry : fs::directory_iterator( "/proc" ) ) {
        if ( !fs::is_directory( entry ) ) {
            continue;
        }

        if ( !Durin::String::isDecimalNumber( entry.path().leaf().string() ) ) {
            continue;
        }

        //std::cout << entry.path() << std::endl;
        processes.emplace_back( getProcessInfo( entry.path() ) );
        processes.back().pid = entry.path().leaf().string();
    }

    return processes;
}

void show(
    const ProcessFinder::ProcessListT& processes,
    const std::vector<std::string>& filters,
    const std::vector<std::string>& fields )
{
    using DumpTableT = std::unordered_map<std::string, std::function<void (const ProcessInfo&)>>;
    DumpTableT dumpTable{
        { "seccomp", []( const ProcessInfo& info ) {
                         switch (info.seccomp) {
                             case 0: { std::cout << "disabled"; } break;
                             case 1: { std::cout << "strict"; } break;
                             case 2: { std::cout << "filter"; } break;
                             default: { std:: cout << "unknown"; }
                         }
                     } },
        { "cmdline", []( const ProcessInfo& info ) {
                         if ( info.cmdLine.empty() ) {
                             return;
                         }

                         std::cout << info.cmdLine[ 0 ];
                         for ( size_t i = 1; i < info.cmdLine.size(); ++i ) {
                             std::cout << " " << info.cmdLine[ i ];
                         }
                     } }
    };

    for ( const auto& process : processes ) {
        bool matched = false;
        for ( auto iter = filters.begin(); !matched && iter != filters.end(); ++iter ) {
            matched = boost::algorithm::starts_with( process.name, *iter );
        }

        if ( !matched ) {
            continue;
        }

        std::cout << process.pid << " " << process.name;

        for ( const auto& field : fields ) {
            DumpTableT::const_iterator dumpFunction = dumpTable.find( field );
            if ( dumpTable.end() != dumpFunction ) {
                std::cout << " ";
                dumpFunction->second( process );
            }
        }

        std::cout << std::endl;
    }
}

} // namespace Knock