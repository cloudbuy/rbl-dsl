#include "event_processing/event_gen_site.h"
#include <string>
#include <gtest/gtest.h>

using namespace rubble::event_processing;

void function()
{
  static EventGenSite species("species");
  static EventToken & person        = species.eventToken("person");
  static FieldToken person_height   = person.fieldToken("height", "int");
  static FieldToken person_weight   = person.fieldToken("weight", "int");
  static FieldToken person_name     = person.fieldToken("name", "string");

  static EventToken & child = species.eventToken("child");
  static FieldToken child_id      = child.fieldToken("id", "int");
  static FieldToken child_person  = child.fieldToken("person", "event(person)");
  
  static EventToken & adult         = species.eventToken("adult");
  static FieldToken adult_id        = adult.fieldToken("id", "int");
  static FieldToken adult_person    = adult.fieldToken("person", "int");
  static FieldToken adult_children  = adult.fieldToken ("children", "event(child)[]");
  
}

TEST(callsite_schema_generation_tests, basic)
{
  static EventGenSite species("species");
  static EventToken & person        = species.eventToken("person");
  static FieldToken person_height   = person.fieldToken("height", "int");

  EXPECT_EQ(std::string(person.name()), std::string("person"));
  EXPECT_EQ(std::string(species.name()), std::string("species"));
  EXPECT_EQ(std::string(person_height.name()), std::string("height"));
  EXPECT_EQ(std::string(person_height.signature()), std::string("int"));
}

#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
