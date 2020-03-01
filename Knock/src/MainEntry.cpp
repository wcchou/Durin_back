#include <Config/Config.hpp>

#include <iostream>

#include <boost/date_time.hpp>

#include <Durin/CmdLine/CmdLine.hpp>
#include <Durin/Hash/Hash.hpp>

#include <Commands.hpp>
#include <Task/HttpServer.hpp>
#include <Task/FileGenerator.hpp>
#include <Task/ProcessUtil.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace Knock {

/*!
 * \par Usage
 *      --generate.file --size=10mb --count=10 [--output=output]
 */
Durin::CmdLine::TaskResult onGenerateFile( Durin::CmdLine::CmdLineContext& cmdLine )
{
    static const char* funcName( "onGenerateFile" );

    using namespace std;
    using namespace Durin;

    if ( !cmdLine.has( Knock::GenerateFileCmd ) ) {
        return CmdLine::TaskResult::Continue;
    }

    size_t fileSize = 0;
    if ( !cmdLine.getFileSize( fileSize ) ) {
        cout << "[" << Knock::GenerateFileCmd
             << "] usage: --generate.file --size=10mb --count10 [--output=output]"
             << endl;
        return CmdLine::TaskResult::Failed;
    }

    fs::path outDir = cmdLine.getOutputPath();
    Knock::FileGenerator generator;
    generator.generateFiles(
        outDir,
        cmdLine.getOptionValue( CmdLine::CountCmd, 1 ),
        fileSize );

    return CmdLine::TaskResult::Done;
}

Durin::CmdLine::TaskResult onHashFile( Durin::CmdLine::CmdLineContext& cmdLine )
{
    static const char* funcName( "onHashFile" );

    using namespace Durin;

    if ( !cmdLine.has( Knock::HashFileCmd ) ) {
        return CmdLine::TaskResult::Continue;
    }

    if ( !cmdLine.has( CmdLine::InputCmd ) ) {
        std::cout << "[Knock] please specify input via --input=xxx\n";
        return CmdLine::TaskResult::Failed;
    }

    std::string digest = Durin::Hash::fileSha256(
        cmdLine.getOptionValue<std::string>( CmdLine::InputCmd, "" ) );
    std::cout << "SHA256: " << digest << std::endl;

    return CmdLine::TaskResult::Done;
}

Durin::CmdLine::TaskResult onHttpServer( Durin::CmdLine::CmdLineContext& cmdLine )
{
    using namespace Durin;

    if ( !cmdLine.has( HttpServerCmd ) ) {
        return CmdLine::TaskResult::Continue;
    }

    HttpServer  server;
    if ( !server.init(
            cmdLine.getOptionValue<std::string>( CmdLine::IpCmd, "0.0.0.0" ),
            cmdLine.getOptionValue<std::string>( CmdLine::PortCmd, "8080" ),
            cmdLine.getOptionValue<std::string>( CmdLine::InputCmd, "." ) ) ) {
        return CmdLine::TaskResult::Failed;
    }

    std::cout << "Start http server: " << server.address().to_string() << ":" << server.port()
              << std::endl;

    if ( !server.start() ) {
        return CmdLine::TaskResult::Failed;
    }

    server.wait();

    return CmdLine::TaskResult::Done;
}

/*!
 * \par Usage
 *      --process.list [--output.field=name,cmdline,seccomp] [--filter=process_name] [--mode=snapshot|monitor]
 */
Durin::CmdLine::TaskResult onProcessList( Durin::CmdLine::CmdLineContext& cmdLine )
{
    using namespace Durin;

    if ( !cmdLine.has( ProcessListCmd ) ) {
        return CmdLine::TaskResult::Continue;
    }

    std::vector<std::string>    fields;
    if ( cmdLine.has( Knock::OutputFieldCmd ) ) {
        fields = cmdLine.getOptionValues( Knock::OutputFieldCmd, ";," );
    }

    std::vector<std::string>    filters = 
    cmdLine.getOptionValues( CmdLine::FilterCmd, ";," );

    std::string repeat = cmdLine.getOptionValue<std::string>( CmdLine::ModeCmd, "snapshot" );

    if ( "monitor" == repeat ) {
        while ( true ) {
            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

            std::cout << "=========================== " << now << " ===========================\n";
            ProcessFinder finder;
            ProcessFinder::ProcessListT processList = finder.query();
            show( processList, filters, fields );
            ::sleep( 3 );
        }
    }
    else {
        ProcessFinder finder;
        ProcessFinder::ProcessListT processList = finder.query();
        show( processList, filters, fields );
    }

    return CmdLine::TaskResult::Done;
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
              "Used with other commands to specify the output file/directory/..." )
            ( ModeCmd,
              po::value<std::string>(),
              "Used with other commands to specify mode" )
            ( FilterCmd,
              po::value<std::string>(),
              "Used with other commands to specify filter" )

            ( Knock::GenerateFileCmd,
              "Generate file(s) with random content.\n"
              "--generate.file --count=10 --size=10MB|KB|GB [--output=output]" )
            ( Knock::HashFileCmd, 
              "Calculate the file hash.\n"
              "--hash.file --input=file" )
            ( Knock::HttpServerCmd,
              "Run a HTTP Server.\n"
              "--http.server --ip=0.0.0.0 --port=8080 --input=doc-root" )
            ( Knock::ProcessListCmd,
              "List running process.\n"
              "--process.list [--output.field=name,cmdline,seccomp] [--filter=process-name] [--mode=snapshot|monitor]\n"
              "--output.field=name[,cmdline[,seccmp]]: process info to be dumped\n"
              "--filter=process-name: fitler process by starts-with-name\n"
              "--mode=snapshot|monitor: just one shot or list process periodically" )

            ( IpCmd,
              po::value<std::string>(),
              "Used with other commands to specify address" )
            ( PortCmd,
              po::value<std::string>(),
              "Used with other commands to specify port" )

            ( OutputFieldCmd,
              po::value<std::string>(),
              "Used with other command to specify output field" );

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
            { Knock::HashFileCmd,    &onHashFile },
            { Knock::HttpServerCmd,  &onHttpServer },
            { Knock::ProcessListCmd, &onProcessList }
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
