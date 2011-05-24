#include "marshall/parser/NamespaceParsers.h"

#include "marshall/DescriptorBuilders.h"
#include "event_model/detail/DescriptorsCommon-Serialization.h"
#include "relay/Descriptors-Serialization.h"
#include "generator/Descriptors-Serialization.h"
#include <gtest/gtest.h>


#ifdef ISOLATED_GTEST_COMPILE
std::string test_data_dir = "./test_data";
#else
std::string test_data_dir;
#endif

using namespace rubble::event_model::descriptors;

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
    parser::NamespaceFileParser::t_mnd_shp mnd_s = 
        file_parser.get_descriptor();

    ASSERT_EQ(mnd_s->name(), "baboons");

    ASSERT_TRUE((*mnd_s).EventAt(1) != NULL);
    ASSERT_TRUE((*mnd_s).EventAt(6) != NULL);
    EXPECT_EQ((*mnd_s).event_container_occupied_size(),2);
    EXPECT_EQ((*mnd_s).event_container_size(), 7);

    ASSERT_TRUE((*mnd_s).EventAt(1)->TypeAt(0) == NULL);
    ASSERT_TRUE((*mnd_s).EventAt(1)->TypeAt(1) != NULL);
    ASSERT_TRUE((*mnd_s).EventAt(1)->TypeAt(2) != NULL);

    EXPECT_EQ( (*mnd_s).EventAt(1)->type_container_occupied_size(), 2);
    EXPECT_EQ( (*mnd_s).EventAt(1)->type_container_size(),3);

    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeOidAt(1)->name(), "hassan");
    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeAt(1)->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeAt(1)->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeAt(1)->type(), RBL_TYPE_ORDINAL(rbl_types::rbl_int4));

    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeOidAt(2)->name(), "monkeys");
    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeAt(2)->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeAt(2)->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( (*mnd_s).EventAt(1)->TypeAt(2)->type(), RBL_TYPE_ORDINAL(rbl_types::rbl_int8));

    EXPECT_EQ( (*mnd_s).EventAt(6)->type_container_occupied_size(), 2);
    EXPECT_EQ( (*mnd_s).EventAt(6)->type_container_size(),3);

    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeOidAt(1)->name(), "hassan");
    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeAt(1)->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeAt(1)->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeAt(1)->type(), RBL_TYPE_ORDINAL(rbl_types::rbl_int4));

    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeOidAt(2)->name(), "monkeys");
    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeAt(2)->is_primitive(), true);
    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeAt(2)->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( (*mnd_s).EventAt(6)->TypeAt(2)->type(), RBL_TYPE_ORDINAL(rbl_types::rbl_int8));
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
    }
}

#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    std::cout << sizeof(rbl_types::type_variant) << std::endl;
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
