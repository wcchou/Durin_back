#include <Config/Config.hpp>

#include <iostream>

#include <Durin/System/ErrorCode.hpp>

int main( int argc, char* arv[] )
{
    std::cout << "Hello World\n";
    std::cout << Durin::lastErrorCode() << std::endl;
    return 0;
}