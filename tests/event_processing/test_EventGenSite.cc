#include "event_processing/event_gen_site.h"
#include <string>
#include <gtest/gtest.h>

#define RBL_TYPE_HEADER_SPIRIT_PARSING
#include "event_model/types.h"
#undef RBL_TYPE_HEADER_SPIRIT_PARSING

using namespace rubble::event_processing;
using namespace rubble::event_model;

void function()
{
  static EventGenSite species("species");
  static EventToken & person        = species.eventToken("person");
  static FieldToken person_height   = person.fieldToken("height", "int");
  static FieldToken person_weight   = person.fieldToken("weight", "int");
  static FieldToken person_blood_pressure = person.fieldToken("blood_presssure", "int[]");
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
  static FieldToken person_blood_pressure = person.fieldToken("blood_presssure", "int[]");

  EXPECT_EQ(std::string(person.name()), std::string("person"));
  EXPECT_EQ(std::string(species.name()), std::string("species"));
  EXPECT_EQ(std::string(person_height.name()), std::string("height"));
  EXPECT_EQ(std::string(person_height.signature()), std::string("int"));
 
  EXPECT_EQ(person_height.type_ordinal(), get_type_ordinal_f<rbl_int4>::pos::value); 
  EXPECT_NE(person_height.type_ordinal(), get_type_ordinal_f<rbl_int8>::pos::value);

  EXPECT_FALSE(person_height.is_variant());

  EXPECT_EQ(person_blood_pressure.type_ordinal(), get_type_ordinal_f<rbl_int4>::pos::value); 
  EXPECT_TRUE(person_blood_pressure.is_variant());

  
  EXPECT_THROW(person.fieldToken("HeIght", "int"), GenSiteException);
  EXPECT_THROW(species.eventToken("PerSoN"), GenSiteException);
 
  ASSERT_NO_THROW( species.getEventToken("peRSon"));
  const EventToken & temp = species.getEventToken("person");
  
  EXPECT_EQ( std::string(temp.name()), std::string("person"));
 
  EXPECT_THROW(species.getEventToken("human_being"),GenSiteException); 
}

TEST(callsite_schema_generation_tests, event_parse_test)
{
  static EventGenSite species("species");

  // try to add a reference before person has been defined -- child is defined early for this reason
  static EventToken & child = species.eventToken("child");
  EXPECT_THROW( child.fieldToken("person", "event(person)"), GenSiteException);

  static EventToken & person        = species.eventToken("person");
  static FieldToken person_height   = person.fieldToken("height", "int");
  static FieldToken person_weight   = person.fieldToken("weight", "int");
  static FieldToken person_blood_pressure = person.fieldToken("blood_presssure", "int[]");
  static FieldToken person_name     = person.fieldToken("name", "string");

    static FieldToken child_id      = child.fieldToken("id", "int");
  static FieldToken child_person  = child.fieldToken("person", "event(person)");
  
  EXPECT_FALSE( child_id.is_event() );
  EXPECT_TRUE( child_person.is_event() );
  EXPECT_EQ( std::string(child_person.get_ref_event_token().name() ), std::string("person"));
 
  static EventToken & adult         = species.eventToken("adult");
  static FieldToken adult_id        = adult.fieldToken("id", "int");
  static FieldToken adult_person    = adult.fieldToken("person", "int");
  static FieldToken adult_children  = adult.fieldToken ("children", "event(child)[]");
  
  EXPECT_TRUE(adult_children.is_event());
  EXPECT_TRUE(adult_children.is_variant());
  EXPECT_EQ( std::string(adult_children.get_ref_event_token().name() ), std::string("child"));
}

#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
