#include "interface/RelayToServerInterface.h"
#include "marshall/ServerToRelay.h"
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <RCF/Idl.hpp>
#include <RCF/RcfServer.hpp>
#include <RCF/TcpEndpoint.hpp>
#include <iostream>

std::string test_data_dir;

int main()
{
    namespace filesystem = boost::filesystem;
    if(filesystem::exists("test_data"))
        test_data_dir = "test_data";
    else if(filesystem::exists("parser/test_data"))
        test_data_dir = "parser/test_data";
    else 
        throw "couldn't find the test_data dir to load test files from";
    
    std::string test_file = test_data_dir;
    test_file.append("/namespace_correct");

    NamespaceRepository nfp;
    nfp.AddNamespaceFromFile(test_file);
    
    RCF::RcfServer server( RCF::TcpEndpoint(50001));
    server.bind<I_NamespaceRepository>(nfp);
    server.start();

    std::cout << "Test Server Running (enter = exit)" << std::endl;
    std::cin.get();
    
    return 0;
}
