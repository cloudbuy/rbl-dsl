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

TEST(FileParserTesting, parse_correct_file)
{
    std::string correct_namespace_file = test_data_dir;
    correct_namespace_file.append("/namespace_correct");
 
    parser::NamespaceFileParser file_parser(correct_namespace_file);
    ASSERT_TRUE(file_parser.CanParse());
    
     
    ASSERT_TRUE(file_parser.Parse());
    MarshallNamespaceDescriptor_shptr mnd_s = 
        file_parser.get_descriptor();

    ASSERT_EQ(mnd_s->name(), "baboons");

    ASSERT_TRUE((*mnd_s).events[1] != NULL);
    ASSERT_TRUE((*mnd_s).events[6] != NULL);
    EXPECT_EQ((*mnd_s).events.occupied_size(),2);
    EXPECT_EQ((*mnd_s).events.size(), 7);

    ASSERT_TRUE((*mnd_s).events[1]->types[0] == NULL);
    ASSERT_TRUE((*mnd_s).events[1]->types[1] != NULL);
    ASSERT_TRUE((*mnd_s).events[1]->types[2] != NULL);

    EXPECT_EQ( (*mnd_s).events[1]->types.occupied_size(), 2);
    EXPECT_EQ( (*mnd_s).events[1]->types.size(),3);

    EXPECT_EQ( (*mnd_s).events[1]->types.EntryAtordinal(1)->name(), "hassan");
    EXPECT_EQ( (*mnd_s).events[1]->types[1]->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).events[1]->types[1]->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( (*mnd_s).events[1]->types[1]->type(), VALUE_INT4);

    EXPECT_EQ( (*mnd_s).events[1]->types.EntryAtordinal(2)->name(), "monkeys");
    EXPECT_EQ( (*mnd_s).events[1]->types[2]->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).events[1]->types[2]->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( (*mnd_s).events[1]->types[2]->type(), VALUE_INT8);

    EXPECT_EQ( (*mnd_s).events[6]->types.occupied_size(), 2);
    EXPECT_EQ( (*mnd_s).events[6]->types.size(),3);

    EXPECT_EQ( (*mnd_s).events[6]->types.EntryAtordinal(1)->name(), "hassan");
    EXPECT_EQ( (*mnd_s).events[6]->types[1]->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).events[6]->types[1]->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( (*mnd_s).events[6]->types[1]->type(), VALUE_INT4);

    EXPECT_EQ( (*mnd_s).events[6]->types.EntryAtordinal(2)->name(), "monkeys");
    EXPECT_EQ( (*mnd_s).events[6]->types[2]->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).events[6]->types[2]->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( (*mnd_s).events[6]->types[2]->type(), VALUE_INT8);
}

TEST(FileParserTesting, parse_incorrect_files)
{
    std::string correct_namespace_file = test_data_dir;
    correct_namespace_file.append("/namespace_failbrace");
 
    parser::NamespaceFileParser file_parser(correct_namespace_file);
    ASSERT_TRUE(file_parser.CanParse());
    
    try { ASSERT_FALSE(file_parser.Parse());}
    catch(std::runtime_error & e)
    {
        std::cout << e.what();
    }
}

TEST(FileParserTesting, parse_spacein_id)
{
    std::string correct_namespace_file = test_data_dir;
    correct_namespace_file.append("/namespace_spacein_id");
 
    parser::NamespaceFileParser file_parser(correct_namespace_file);
    ASSERT_TRUE(file_parser.CanParse());
    
    try { ASSERT_FALSE(file_parser.Parse());}
    catch(std::runtime_error & e)
    {
        std::cout << e.what();
    }
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
