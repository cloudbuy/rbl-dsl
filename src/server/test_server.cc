#include <interface/RelayToServerInterface.h>
#include <server/RelayToServer.h>
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>


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
}
