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
    ASSERT_TRUE(mndb.events().size() == 0); 
    ASSERT_TRUE(mndb.events().occupied_size() == 0);

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
    ASSERT_TRUE(mndb.events().size() == 0); 
    ASSERT_TRUE(mndb.events().occupied_size() == 0);

    MarshallEventDescriptorBuilder medb1;

    bool res;
    Oid one("monkey",0);
    Oid two("zebra",3);
    Oid three("giraffe",8);
    
    medb1.Init(Oid("event",0),mndb,res);
    ASSERT_TRUE(res);

    medb1.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
   
    MarshallEventDescriptorBuilder medb2;

    medb2.Init(Oid("event",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, VALUE_INT4, true),
                        res);
    ASSERT_TRUE(res);

    mndb.AddEventDescriptor(medb1);
    mndb.AddEventDescriptor(medb2);

    // first test if medb1 and medb2 are structuraly correct
    // then downcast to non-builder marshallnamespace and test
}



int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
