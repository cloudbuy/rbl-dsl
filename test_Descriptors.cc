#include "Descriptors.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <sstream>
#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>
#include <boost/cstdint.hpp>

using namespace event_model;


// test the serialization of the event type descriptor
TEST(testing_const_serialization, test_one)
{
    EventTypeDescriptor etd_default_constructed;
    EventTypeDescriptor etd_source(ENTRY_REQUIRED, VALUE_INT4, false);
    
    std::stringstream ss;
    SF::OBinaryStream os(ss);   
    os << etd_source;
    
    SF::IBinaryStream is(ss);
    is >> etd_default_constructed;
    
    ASSERT_TRUE(etd_default_constructed.qualifier() == ENTRY_REQUIRED);
    ASSERT_TRUE(etd_default_constructed.type() == VALUE_INT4);
    ASSERT_TRUE(etd_default_constructed.is_primitive() == false);
}

// 
TEST(testing_marshall_building, test_one)
{
    MarshallNamespaceDescriptorBuilder mndb("testing");
    ASSERT_TRUE(mndb.name() == "testing");
    ASSERT_TRUE(mndb.events.size() == 0); 
    ASSERT_TRUE(mndb.events.occupied_size() == 0);

    MarshallEventDescriptorBuilder medb;

    bool res;
    Oid hassan_zero("hassan",0);
    Oid hassan_zero_ord_collision("hassan1",0);
    Oid hassan_zero_name_collision("hassan",1);

    medb.Init(hassan_zero,mndb,res);
    ASSERT_TRUE(res);

    medb.AddEventType(  hassan_zero,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
    medb.AddEventType(  hassan_zero,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_FALSE(res);
    
    medb.AddEventType(  hassan_zero_ord_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_FALSE(res);
    medb.AddEventType(  hassan_zero_name_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_FALSE(res);
//    medb.
}

TEST(testing_marshall_building, test_two)
{
    MarshallNamespaceDescriptorBuilder mndb("testing");
    ASSERT_TRUE(mndb.name() == "testing");
    ASSERT_TRUE(mndb.events.size() == 0); 
    ASSERT_TRUE(mndb.events.occupied_size() == 0);

    MarshallEventDescriptorBuilder medb1;
    MarshallEventDescriptorBuilder medb2;
    bool res;
    Oid one("monkey",0);
    Oid two("zebra",3);
    Oid three("giraffe",8);
    
    medb1.Init(Oid("evenT",0),mndb,res);
    ASSERT_TRUE(res);

    medb1.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, VALUE_INT4, false),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT8, true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.Init(Oid("monkey",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, VALUE_INT4, false),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT8, true),
                        res);
    ASSERT_TRUE(res);

    mndb.AddEventDescriptor(medb1,res);
    ASSERT_TRUE(res);
    
    mndb.AddEventDescriptor(medb2,res);
    ASSERT_TRUE(res);

    ASSERT_EQ(mndb.events.size(),6) ;    
    ASSERT_EQ(mndb.events.occupied_size(), 2);

    // first test if medb1 and medb2 are structuraly correct
    ASSERT_TRUE(mndb.events[0] != NULL);
    ASSERT_TRUE(mndb.events[1] == NULL);
    ASSERT_TRUE(mndb.events[2] == NULL);
    ASSERT_TRUE(mndb.events[3] == NULL);
    ASSERT_TRUE(mndb.events[4] == NULL);
    ASSERT_TRUE(mndb.events[5] != NULL);
  
    ASSERT_TRUE( mndb.events.EntryWithName("MonKey") != NULL);
    ASSERT_TRUE( mndb.events.EntryWithName("HasSan") ==NULL);
    ASSERT_TRUE( mndb.events.EntryWithName("eVeNt") !=NULL);

    ASSERT_TRUE(mndb.events.EntryWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(mndb.events.EntryWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ(mndb.events[0]->types.size(),9);
    ASSERT_EQ(mndb.events[0]->types.occupied_size(),3);

    ASSERT_TRUE(mndb.events[0]->types[0]!=NULL);
    ASSERT_TRUE(mndb.events[0]->types[1]==NULL);
    ASSERT_TRUE(mndb.events[0]->types[2]==NULL);
    ASSERT_TRUE(mndb.events[0]->types[3]!=NULL);
    ASSERT_TRUE(mndb.events[0]->types[4]==NULL);
    ASSERT_TRUE(mndb.events[0]->types[5]==NULL);
    ASSERT_TRUE(mndb.events[0]->types[6]==NULL);
    ASSERT_TRUE(mndb.events[0]->types[7]==NULL);
    ASSERT_TRUE(mndb.events[0]->types[8]!=NULL);
   
    ASSERT_EQ(mndb.events[0]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(mndb.events[0]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb.events[0]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(mndb.events[0]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(mndb.events[0]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb.events[0]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(mndb.events[0]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(mndb.events[0]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mndb.events[0]->types[8]->is_primitive(), true);

    ASSERT_EQ(mndb.events[5]->types.size(),9);
    ASSERT_EQ(mndb.events[5]->types.occupied_size(),3);

    ASSERT_TRUE(mndb.events[5]->types[0]!=NULL);
    ASSERT_TRUE(mndb.events[5]->types[1]==NULL);
    ASSERT_TRUE(mndb.events[5]->types[2]==NULL);
    ASSERT_TRUE(mndb.events[5]->types[3]!=NULL);
    ASSERT_TRUE(mndb.events[5]->types[4]==NULL);
    ASSERT_TRUE(mndb.events[5]->types[5]==NULL);
    ASSERT_TRUE(mndb.events[5]->types[6]==NULL);
    ASSERT_TRUE(mndb.events[5]->types[7]==NULL);
    ASSERT_TRUE(mndb.events[5]->types[8]!=NULL);
    
    ASSERT_EQ(mndb.events[5]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(mndb.events[5]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb.events[5]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(mndb.events[5]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(mndb.events[5]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb.events[5]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(mndb.events[5]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(mndb.events[5]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mndb.events[5]->types[8]->is_primitive(), true);
}



int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
