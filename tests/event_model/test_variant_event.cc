// This test tests two construct added to the schema grammar:
// event references and variant entries.
#include "event_model/marshall_parser.h"
#include <gtest/gtest.h>

#ifdef ISOLATED_GTEST_COMPILE
std::string test_data_dir = "./test_data";
#else
std::string test_data_dir;
#endif

using namespace rubble::event_model;

TEST(VariantEventFileParsing, test_basic)
{
  std::string correct_namespace_file = test_data_dir;
  correct_namespace_file.append("/namespace_variant_event");
    
  NamespaceFileParser file_parser(correct_namespace_file);
  
  ASSERT_TRUE(file_parser.CanParse()) << file_parser.error();

  ASSERT_TRUE(file_parser.Parse());

}

#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
