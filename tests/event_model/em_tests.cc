#include <gtest/gtest.h>

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

extern std::string test_data_dir;

int main(int argc,char ** argv)
{
    namespace filesystem = boost::filesystem;
    if(filesystem::exists("test_data"))
        test_data_dir = "test_data";
    else if(filesystem::exists("parser/test_data"))
        test_data_dir = "parser/test_data";
    else 
        throw "couldn't find the test_data dir to load test files from";

    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}

