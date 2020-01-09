#ifndef DURIN_HASH_HASH_HPP
#define DURIN_HASH_HASH_HPP

#include <Durin/Config/Config.hpp>

#include <string>

#include <boost/filesystem.hpp>

namespace Durin {
namespace Hash {

constexpr size_t Sha256DigestLength = 32;

DURIN_EXPORT std::string fileSha256( const boost::filesystem::path& filePath );
DURIN_EXPORT std::string bufferSha256( const std::vector<unsigned char>& buf );
DURIN_EXPORT std::string bufferSha256( const void* pBuf, size_t bufSize );
DURIN_EXPORT std::string stringSha256( const std::string& str );

} // namespace Durin::Hash
} // namespace Durin

#endif // End of include guard
