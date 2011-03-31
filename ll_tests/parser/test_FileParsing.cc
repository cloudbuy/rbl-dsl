#include <gtest/gtest.h>
#include <parser/NamespaceParsers.h>

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

std::string test_data_dir;

using namespace event_model;

TEST(FileParserTesting, test_file_existance)
{
    std::string correct_namespace_file = test_data_dir;
    correct_namespace_file.append("/namespace_correct");
    
    std::string non_existing_file = test_data_dir;
    non_existing_file.append("/namespace_monkeys");


    parser::NamespaceFileParser file_parser(correct_namespace_file);
    
    ASSERT_TRUE(file_parser.CanParse()) << file_parser.error();
   
    file_parser.SetFile(non_existing_file); 
    ASSERT_FALSE(file_parser.CanParse());
     
}

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
