#include <Config/Config.hpp>

#include <iostream>

#include <Durin/CmdLine/CmdLine.hpp>

#include <Commands.hpp>
#include <Task/HttpServer.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace Knock {

Durin::CmdLine::TaskResult
onHttpServer( Durin::CmdLine::CmdLineContext& cmdLine )
{
    using namespace Durin::CmdLine;

    if ( !cmdLine.has( HttpServerCmd ) ) {
        return TaskResult::Continue;
    }

    HttpServer  server;
    if ( !server.init(
            cmdLine.getOptionValue<std::string>( IpCmd, "0.0.0.0" ),
            cmdLine.getOptionValue<std::string>( PortCmd, "8080" ),
            cmdLine.getOptionValue<std::string>( InputCmd, "." ) ) ) {
        return TaskResult::Failed;
    }

    std::cout << "Start http server: " << server.address().to_string() << ":" << server.port()
              << std::endl;

    if ( !server.start() ) {
        return TaskResult::Failed;
    }

    server.wait();

    return TaskResult::Done;
}

} // Knock namespace

int main( int argc, char* argv[] )
{
    using namespace std;
    using namespace Knock;
    using namespace Durin::CmdLine;

    try {
        po::options_description desc( "Allowed options" );
        desc.add_options()
            ( HelpCmd,
              "Show help message" )

            ( InputCmd,
              po::value<std::string>(),
              "Used with other commands to specify the input" )
            ( OutputCmd,
              po::value<std::string>(),
              "Used with other command to specify the output file/directory/..." )

            ( Knock::HttpServerCmd,
              "Run a HTTP Server" )

            ( IpCmd,
              po::value<std::string>(),
              "Used with other commands to specify address" )
            ( PortCmd,
              po::value<std::string>(),
              "Used with other commands to specify port" );

        if ( 1 == argc ) {
            cout << desc << endl;
            return 0;
        }

        po::variables_map vm;
        po::store( po::parse_command_line( argc, argv, desc), vm );
        po::notify( vm );

        if ( vm.count( HelpCmd ) ) {
            cout << desc << endl;
            return 0;
        }

        struct CmdDispatchTable {
            const char* cmd;
            CmdHandler  handler;
        } table[] = {
            { Knock::HttpServerCmd, &onHttpServer }
        };

        Durin::CmdLine::CmdLineContext   cmdLine( vm );
        for ( auto& item : table ) {
            if ( !vm.count( item.cmd ) ) {
                continue;
            }

            if ( Durin::CmdLine::TaskResult::Continue == item.handler( cmdLine ) ) {
                break;
            }
        }
    }
    catch ( std::exception& ex ) {
        cout << "Exception: " << ex.what() << endl;;
    }

    return 0;
}
