// This test tests two construct added to the schema grammar:
// event references and variant entries.
#include "event_model/marshall_parser.h"
#include "event_model/descriptor_generators.h"
#include <gtest/gtest.h>
#include <iostream>

#ifdef ISOLATED_GTEST_COMPILE
std::string test_data_dir = "./test_data";
#else
std::string test_data_dir;
#endif

using namespace rubble::event_model;

TEST(VariantEventFileParsing, test_variant)
{
  std::string correct_namespace_file = test_data_dir;
  correct_namespace_file.append("/namespace_variant_event");
    
  NamespaceFileParser file_parser(correct_namespace_file);
  
  ASSERT_TRUE(file_parser.CanParse()) << file_parser.error();

  ASSERT_TRUE(file_parser.Parse());
  
  NamespaceFileParser::t_mnd_shp mnd_s = 
        file_parser.get_descriptor();

  ASSERT_TRUE( mnd_s->EventAt(1)->TypeAt(3)!=NULL);
  EXPECT_TRUE( mnd_s->EventAt(1)->TypeAt(3)->is_variant());

  EXPECT_FALSE( mnd_s->EventAt(1)->TypeAt(2)->is_variant());
}

TEST(VariantEventFileParsing, test_event)
{
  std::string correct_namespace_file = test_data_dir;
  correct_namespace_file.append("/namespace_variant_event");
    
  NamespaceFileParser file_parser(correct_namespace_file);
  
  ASSERT_TRUE(file_parser.CanParse()) << file_parser.error();

  ASSERT_TRUE(file_parser.Parse());
  
  NamespaceFileParser::t_mnd_shp mnd_s = 
        file_parser.get_descriptor();
  
  ASSERT_TRUE(mnd_s->EventAt(2)->TypeAt(2)->is_event());
  ASSERT_TRUE(mnd_s->EventAt(3)->TypeAt(3)->is_event());
  ASSERT_TRUE(mnd_s->EventAt(3)->TypeAt(3)->is_variant());
  
  EXPECT_EQ(mnd_s->EventAt(2)->TypeAt(2)->reference_event_ordinal(),1);
  EXPECT_EQ(mnd_s->EventAt(3)->TypeAt(3)->reference_event_ordinal(),1);
}

TEST(VariantEventFileParsing, generation_testing)
{
  std::string correct_namespace_file = test_data_dir;
  correct_namespace_file.append("/namespace_variant_event");
    
  NamespaceFileParser file_parser(correct_namespace_file);
  
  ASSERT_TRUE(file_parser.CanParse()) << file_parser.error();

  ASSERT_TRUE(file_parser.Parse());
  
  NamespaceFileParser::t_mnd_shp mnd_s = 
        file_parser.get_descriptor();

  typedef std::back_insert_iterator<std::string> iterator_t;
  std::string str;
  iterator_t sink(str);
   
  namespace_generator_grammar<iterator_t, MarshallNamespaceDescriptor> nsg;

  bool ret = karma::generate(
      sink,
      nsg(phoenix::cref(*mnd_s))
    );

//  std::cout << str << std::endl;

  typedef std::string::iterator c_s_it;

  rubble::event_model::skipper<c_s_it> skipper_;
  CompoundRules<c_s_it, skipper<c_s_it> > compound_rules;

  c_s_it beg = str.begin();
  c_s_it end = str.end();

  MarshallNamespaceDescriptorBuilder mndb;
 
  bool res =  qi::phrase_parse(
                    beg,end,
                    compound_rules.namespace_descriptor(phoenix::ref(mndb)),
                    skipper_ 
                );
  // variant tests
  ASSERT_TRUE( mndb.EventAt(1)->TypeAt(3)!=NULL);
  EXPECT_TRUE( mndb.EventAt(1)->TypeAt(3)->is_variant());

  EXPECT_FALSE( mndb.EventAt(1)->TypeAt(2)->is_variant());
 
  // event and variant tests
  ASSERT_TRUE(mndb.EventAt(2)->TypeAt(2)->is_event());
  ASSERT_TRUE(mndb.EventAt(3)->TypeAt(3)->is_event());
  ASSERT_TRUE(mndb.EventAt(3)->TypeAt(3)->is_variant());
  
  EXPECT_EQ(mndb.EventAt(2)->TypeAt(2)->reference_event_ordinal(),1);
  EXPECT_EQ(mndb.EventAt(3)->TypeAt(3)->reference_event_ordinal(),1);

}

#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
