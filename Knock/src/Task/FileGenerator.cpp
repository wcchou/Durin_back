#include <Config/Config.hpp>

#include <Task/FileGenerator.hpp>

#include <fstream>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include <Durin/Hash/Hash.hpp>

using namespace std;

namespace fs = boost::filesystem;

namespace Knock {

bool generateFile(
    const boost::filesystem::path& filePath,
    size_t fileSize,
    std::string& o_sha256 )
{
    using namespace std;

    char* data = nullptr;
    try {
        data = new char[ fileSize ];
    }
    catch ( std::bad_alloc& ex ) {
        return false;
    }

    size_t round = rand() % 20;
    //cout << "[Generate] " << round << endl;
    for ( size_t i = 0; i < round; ++i ) {
        int pos = rand() % fileSize;
        int value = (rand() + i) % 0xFF;
        data[ pos ] = value;
        //cout << "[Generate] ( " << pos << ", " << value << " )" << endl;
    }

    ofstream out( filePath.string(), ios::binary );
    if ( out ) {
        out.write( data, fileSize );
    }

    o_sha256 = Durin::Hash::bufferSha256( data, fileSize );
    //std::string checksum = "checksum";
    return true;
}
/*!
 * \brief Generate nFiles files in dir with fileSize (in bytes).
 */
bool FileGenerator::generateFiles(
    const boost::filesystem::path& dir, size_t nFiles, size_t fileSize )
{
    files_.reserve( nFiles );
    ::srand( ::time( nullptr ) );

    fs::create_directories( dir );
    std::string sha256;
    std::string fileName;
    for ( size_t i = 0; i < nFiles; ++i ) {
        fileName = std::to_string( i );
        while ( generateFile( dir / fileName, fileSize, sha256 ) ) {
            auto pos = sha256Set_.find( sha256 );

            if ( sha256Set_.end() != pos ) {
                continue;
            }

            cout << "[Generate] (" << i << "/" << nFiles << ") sha256: " << sha256 << endl;
            files_.push_back( { fileName, sha256 } );
            sha256Set_.insert( sha256 );
            break;
        }
    }

    return writeReport( dir / "gen_report.txt" );
}

bool FileGenerator::writeReport( const boost::filesystem::path& reportPath )
{
    YAML::Emitter   out;
    out << YAML::BeginMap
        << YAML::Key << "version"
        << YAML::Value << "1.0";

    out << YAML::Key << "list"
        << YAML::Value << YAML::BeginSeq;
    for ( const auto& file: files_ ) {
        out << YAML::BeginMap
            << YAML::Key << "file"
            << YAML::Value << file.fileName
            << YAML::Key << "sha256"
            << YAML::Value << file.sha256
            << YAML::EndMap;
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;
    //std::cout << out.c_str() << std::endl;
    std::ofstream report( reportPath.string() );
    report << out.c_str() ;

    return true;
}

} // namespace Knock