#include "parser/detail/OidContainer.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <sstream>
#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>
#include <boost/cstdint.hpp>
#include "parser/detail/EventModelGrammar.h"
#include <gtest/gtest.h>
#include <string>
#include <parser/NamespaceParsers.h>
#include "parser/Descriptors.h"
#include <gtest/gtest.h>
#include <boost/cstdint.hpp>
#include "parser/detail/OidContainer.h"

#include <typeinfo>
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>


#include "test_FileParsing.cc"
#include "test_Grammar.cc"
#include "test_Descriptors.cc"
#include "test_OidContainer.cc"


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
