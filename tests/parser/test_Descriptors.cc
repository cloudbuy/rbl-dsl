#include "marshall/DescriptorBuilders.h"
#include <gtest/gtest.h>

#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>

using namespace event_model;

TEST(descriptor_building_and_serialization, event_entry_serialization_test)
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

TEST(descriptor_building_and_serialization, identifier_collision_tests)
{
    MarshallNamespaceDescriptorBuilder mndb("testing",0);
    ASSERT_TRUE(mndb.name() == "testing");
    ASSERT_TRUE(mndb.event_container_size() == 0); 
    ASSERT_TRUE(mndb.event_container_occupied_size() == 0);

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
#if 0
TEST(descriptor_building_and_serialization, event_descriptor_building_test)
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
}

TEST(descriptor_building_and_serialization, exhaustive_mndb_building_test)
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

    ASSERT_EQ(mndb.events[0].size(),9);
    ASSERT_EQ(mndb.events[0].occupied_size(),3);

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

TEST(descriptor_building_and_serialization, descriptor_downcast_slicing_tests)
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

    MarshallNamespaceDescriptor mnd = mndb;
    
    ASSERT_TRUE(mnd.events[0] != NULL);
    ASSERT_TRUE(mnd.events[1] == NULL);
    ASSERT_TRUE(mnd.events[2] == NULL);
    ASSERT_TRUE(mnd.events[3] == NULL);
    ASSERT_TRUE(mnd.events[4] == NULL);
    ASSERT_TRUE(mnd.events[5] != NULL);
 
    ASSERT_TRUE( mnd.events.EntryWithName("MonKey") != NULL);
    ASSERT_TRUE( mnd.events.EntryWithName("HasSan") ==NULL);
    ASSERT_TRUE( mnd.events.EntryWithName("eVeNt") !=NULL);

    ASSERT_TRUE(mnd.events.EntryWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(mnd.events.EntryWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( typeid(mnd.events.EntryWithName("Monkey")->entry()) 
        ,typeid(*new MarshallEventDescriptor()) );

    ASSERT_EQ(mnd.events[0]->types.size(),9);
    ASSERT_EQ(mnd.events[0]->types.occupied_size(),3);

    ASSERT_TRUE(mnd.events[0]->types[0]!=NULL);
    ASSERT_TRUE(mnd.events[0]->types[1]==NULL);
    ASSERT_TRUE(mnd.events[0]->types[2]==NULL);
    ASSERT_TRUE(mnd.events[0]->types[3]!=NULL);
    ASSERT_TRUE(mnd.events[0]->types[4]==NULL);
    ASSERT_TRUE(mnd.events[0]->types[5]==NULL);
    ASSERT_TRUE(mnd.events[0]->types[6]==NULL);
    ASSERT_TRUE(mnd.events[0]->types[7]==NULL);
    ASSERT_TRUE(mnd.events[0]->types[8]!=NULL);
   
    ASSERT_EQ(mnd.events[0]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(mnd.events[0]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mnd.events[0]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(mnd.events[0]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(mnd.events[0]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mnd.events[0]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(mnd.events[0]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(mnd.events[0]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mnd.events[0]->types[8]->is_primitive(), true);

    ASSERT_EQ(mnd.events[5]->types.size(),9);
    ASSERT_EQ(mnd.events[5]->types.occupied_size(),3);

    ASSERT_TRUE(mnd.events[5]->types[0]!=NULL);
    ASSERT_TRUE(mnd.events[5]->types[1]==NULL);
    ASSERT_TRUE(mnd.events[5]->types[2]==NULL);
    ASSERT_TRUE(mnd.events[5]->types[3]!=NULL);
    ASSERT_TRUE(mnd.events[5]->types[4]==NULL);
    ASSERT_TRUE(mnd.events[5]->types[5]==NULL);
    ASSERT_TRUE(mnd.events[5]->types[6]==NULL);
    ASSERT_TRUE(mnd.events[5]->types[7]==NULL);
    ASSERT_TRUE(mnd.events[5]->types[8]!=NULL);
    
    ASSERT_EQ(mnd.events[5]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(mnd.events[5]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mnd.events[5]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(mnd.events[5]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(mnd.events[5]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mnd.events[5]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(mnd.events[5]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(mnd.events[5]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mnd.events[5]->types[8]->is_primitive(), true);

    RelayNamespaceDescriptor rnd = mnd;

    ASSERT_TRUE(rnd.events[0] != NULL);
    ASSERT_TRUE(rnd.events[1] == NULL);
    ASSERT_TRUE(rnd.events[2] == NULL);
    ASSERT_TRUE(rnd.events[3] == NULL);
    ASSERT_TRUE(rnd.events[4] == NULL);
    ASSERT_TRUE(rnd.events[5] != NULL);
 
    ASSERT_TRUE( rnd.events.EntryWithName("MonKey") != NULL);
    ASSERT_TRUE( rnd.events.EntryWithName("HasSan") ==NULL);
    ASSERT_TRUE( rnd.events.EntryWithName("eVeNt") !=NULL);

    ASSERT_TRUE(rnd.events.EntryWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(rnd.events.EntryWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( typeid(rnd.events.EntryWithName("Monkey")->entry()) 
        ,typeid(*new RelayEventDescriptor()) );

    ASSERT_EQ(rnd.events[0]->types.size(),9);
    ASSERT_EQ(rnd.events[0]->types.occupied_size(),3);

    ASSERT_TRUE(rnd.events[0]->types[0]!=NULL);
    ASSERT_TRUE(rnd.events[0]->types[1]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[2]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[3]!=NULL);
    ASSERT_TRUE(rnd.events[0]->types[4]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[5]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[6]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[7]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[8]!=NULL);
   
    ASSERT_EQ(rnd.events[0]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[0]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.events[0]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(rnd.events[0]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[0]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.events[0]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(rnd.events[0]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(rnd.events[0]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.events[0]->types[8]->is_primitive(), true);

    ASSERT_EQ(rnd.events[5]->types.size(),9);
    ASSERT_EQ(rnd.events[5]->types.occupied_size(),3);

    ASSERT_TRUE(rnd.events[5]->types[0]!=NULL);
    ASSERT_TRUE(rnd.events[5]->types[1]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[2]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[3]!=NULL);
    ASSERT_TRUE(rnd.events[5]->types[4]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[5]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[6]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[7]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[8]!=NULL);
    
    ASSERT_EQ(rnd.events[5]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[5]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.events[5]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(rnd.events[5]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[5]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.events[5]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(rnd.events[5]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(rnd.events[5]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.events[5]->types[8]->is_primitive(), true);

    GeneratorNamespaceDescriptor gnd = rnd;

    ASSERT_TRUE(gnd.events[0] != NULL);
    ASSERT_TRUE(gnd.events[1] == NULL);
    ASSERT_TRUE(gnd.events[2] == NULL);
    ASSERT_TRUE(gnd.events[3] == NULL);
    ASSERT_TRUE(gnd.events[4] == NULL);
    ASSERT_TRUE(gnd.events[5] != NULL);
 
    ASSERT_TRUE( gnd.events.EntryWithName("MonKey") != NULL);
    ASSERT_TRUE( gnd.events.EntryWithName("HasSan") ==NULL);
    ASSERT_TRUE( gnd.events.EntryWithName("eVeNt") !=NULL);

    ASSERT_TRUE(gnd.events.EntryWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(gnd.events.EntryWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( typeid(gnd.events.EntryWithName("Monkey")->entry()) 
        ,typeid(*new GeneratorEventDescriptor()) );

    ASSERT_EQ(gnd.events[0]->types.size(),9);
    ASSERT_EQ(gnd.events[0]->types.occupied_size(),3);

    ASSERT_TRUE(gnd.events[0]->types[0]!=NULL);
    ASSERT_TRUE(gnd.events[0]->types[1]==NULL);
    ASSERT_TRUE(gnd.events[0]->types[2]==NULL);
    ASSERT_TRUE(gnd.events[0]->types[3]!=NULL);
    ASSERT_TRUE(gnd.events[0]->types[4]==NULL);
    ASSERT_TRUE(gnd.events[0]->types[5]==NULL);
    ASSERT_TRUE(gnd.events[0]->types[6]==NULL);
    ASSERT_TRUE(gnd.events[0]->types[7]==NULL);
    ASSERT_TRUE(gnd.events[0]->types[8]!=NULL);
   
    ASSERT_EQ(gnd.events[0]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(gnd.events[0]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd.events[0]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(gnd.events[0]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(gnd.events[0]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd.events[0]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(gnd.events[0]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(gnd.events[0]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd.events[0]->types[8]->is_primitive(), true);

    ASSERT_EQ(gnd.events[5]->types.size(),9);
    ASSERT_EQ(gnd.events[5]->types.occupied_size(),3);

    ASSERT_TRUE(gnd.events[5]->types[0]!=NULL);
    ASSERT_TRUE(gnd.events[5]->types[1]==NULL);
    ASSERT_TRUE(gnd.events[5]->types[2]==NULL);
    ASSERT_TRUE(gnd.events[5]->types[3]!=NULL);
    ASSERT_TRUE(gnd.events[5]->types[4]==NULL);
    ASSERT_TRUE(gnd.events[5]->types[5]==NULL);
    ASSERT_TRUE(gnd.events[5]->types[6]==NULL);
    ASSERT_TRUE(gnd.events[5]->types[7]==NULL);
    ASSERT_TRUE(gnd.events[5]->types[8]!=NULL);
    
    ASSERT_EQ(gnd.events[5]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(gnd.events[5]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd.events[5]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(gnd.events[5]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(gnd.events[5]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd.events[5]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(gnd.events[5]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(gnd.events[5]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd.events[5]->types[8]->is_primitive(), true);
}


// descriptor_building_and_serialization
TEST(descriptor_building_and_serialization , exhaustive_serialization_tests)
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
    
    RelayNamespaceDescriptor rnd = (MarshallNamespaceDescriptor) mndb;

    ASSERT_TRUE(rnd.events[0] != NULL);
    ASSERT_TRUE(rnd.events[1] == NULL);
    ASSERT_TRUE(rnd.events[2] == NULL);
    ASSERT_TRUE(rnd.events[3] == NULL);
    ASSERT_TRUE(rnd.events[4] == NULL);
    ASSERT_TRUE(rnd.events[5] != NULL);
 
    ASSERT_TRUE( rnd.events.EntryWithName("MonKey") != NULL);
    ASSERT_TRUE( rnd.events.EntryWithName("HasSan") ==NULL);
    ASSERT_TRUE( rnd.events.EntryWithName("eVeNt") !=NULL);

    ASSERT_TRUE(rnd.events.EntryWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(rnd.events.EntryWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ(rnd.events[0]->types.size(),9);
    ASSERT_EQ(rnd.events[0]->types.occupied_size(),3);

    ASSERT_TRUE(rnd.events[0]->types[0]!=NULL);
    ASSERT_TRUE(rnd.events[0]->types[1]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[2]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[3]!=NULL);
    ASSERT_TRUE(rnd.events[0]->types[4]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[5]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[6]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[7]==NULL);
    ASSERT_TRUE(rnd.events[0]->types[8]!=NULL);
   
    ASSERT_EQ(rnd.events[0]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[0]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.events[0]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(rnd.events[0]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[0]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.events[0]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(rnd.events[0]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(rnd.events[0]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.events[0]->types[8]->is_primitive(), true);

    ASSERT_EQ(rnd.events[5]->types.size(),9);
    ASSERT_EQ(rnd.events[5]->types.occupied_size(),3);

    ASSERT_TRUE(rnd.events[5]->types[0]!=NULL);
    ASSERT_TRUE(rnd.events[5]->types[1]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[2]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[3]!=NULL);
    ASSERT_TRUE(rnd.events[5]->types[4]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[5]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[6]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[7]==NULL);
    ASSERT_TRUE(rnd.events[5]->types[8]!=NULL);
    
    ASSERT_EQ(rnd.events[5]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[5]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.events[5]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(rnd.events[5]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(rnd.events[5]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.events[5]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(rnd.events[5]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(rnd.events[5]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.events[5]->types[8]->is_primitive(), true);

    std::stringstream ss;
    SF::OBinaryStream os(ss);   
    os << rnd;

    RelayNamespaceDescriptor rnd_out;

    SF::IBinaryStream is(ss);
    is >> rnd_out;

    ASSERT_TRUE(rnd_out.events[0] != NULL);
    ASSERT_TRUE(rnd_out.events[1] == NULL);
    ASSERT_TRUE(rnd_out.events[2] == NULL);
    ASSERT_TRUE(rnd_out.events[3] == NULL);
    ASSERT_TRUE(rnd_out.events[4] == NULL);
    ASSERT_TRUE(rnd_out.events[5] != NULL);
 
    ASSERT_TRUE( rnd_out.events.EntryWithName("MonKey") != NULL);
    ASSERT_TRUE( rnd_out.events.EntryWithName("HasSan") ==NULL);
    ASSERT_TRUE( rnd_out.events.EntryWithName("eVeNt") !=NULL);

    ASSERT_TRUE(rnd_out.events.EntryWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(rnd_out.events.EntryWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( typeid(rnd_out.events.EntryWithName("Monkey")->entry()) 
        ,typeid(*new RelayEventDescriptor()) );

    ASSERT_EQ(rnd_out.events[0]->types.size(),9);
    ASSERT_EQ(rnd_out.events[0]->types.occupied_size(),3);

    ASSERT_TRUE(rnd_out.events[0]->types[0]!=NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[1]==NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[2]==NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[3]!=NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[4]==NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[5]==NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[6]==NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[7]==NULL);
    ASSERT_TRUE(rnd_out.events[0]->types[8]!=NULL);
   
    ASSERT_EQ(rnd_out.events[0]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(rnd_out.events[0]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd_out.events[0]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(rnd_out.events[0]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(rnd_out.events[0]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd_out.events[0]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(rnd_out.events[0]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(rnd_out.events[0]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd_out.events[0]->types[8]->is_primitive(), true);

    ASSERT_EQ(rnd_out.events[5]->types.size(),9);
    ASSERT_EQ(rnd_out.events[5]->types.occupied_size(),3);

    ASSERT_TRUE(rnd_out.events[5]->types[0]!=NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[1]==NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[2]==NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[3]!=NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[4]==NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[5]==NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[6]==NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[7]==NULL);
    ASSERT_TRUE(rnd_out.events[5]->types[8]!=NULL);
    
    ASSERT_EQ(rnd_out.events[5]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(rnd_out.events[5]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd_out.events[5]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(rnd_out.events[5]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(rnd_out.events[5]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd_out.events[5]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(rnd_out.events[5]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(rnd_out.events[5]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd_out.events[5]->types[8]->is_primitive(), true);

    GeneratorNamespaceDescriptor gnd = rnd_out;
    GeneratorNamespaceDescriptor gnd_out;
    os << gnd;
    is >> gnd_out;
    
    ASSERT_TRUE(gnd_out.events[0] != NULL);
    ASSERT_TRUE(gnd_out.events[1] == NULL);
    ASSERT_TRUE(gnd_out.events[2] == NULL);
    ASSERT_TRUE(gnd_out.events[3] == NULL);
    ASSERT_TRUE(gnd_out.events[4] == NULL);
    ASSERT_TRUE(gnd_out.events[5] != NULL);
 
    ASSERT_TRUE( gnd_out.events.EntryWithName("MonKey") != NULL);
    ASSERT_TRUE( gnd_out.events.EntryWithName("HasSan") ==NULL);
    ASSERT_TRUE( gnd_out.events.EntryWithName("eVeNt") !=NULL);

    ASSERT_TRUE(gnd_out.events.EntryWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(gnd_out.events.EntryWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ(gnd_out.events[0]->types.size(),9);
    ASSERT_EQ(gnd_out.events[0]->types.occupied_size(),3);

    ASSERT_TRUE(gnd_out.events[0]->types[0]!=NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[1]==NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[2]==NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[3]!=NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[4]==NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[5]==NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[6]==NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[7]==NULL);
    ASSERT_TRUE(gnd_out.events[0]->types[8]!=NULL);
   
    ASSERT_EQ(gnd_out.events[0]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(gnd_out.events[0]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd_out.events[0]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(gnd_out.events[0]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(gnd_out.events[0]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd_out.events[0]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(gnd_out.events[0]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(gnd_out.events[0]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd_out.events[0]->types[8]->is_primitive(), true);

    ASSERT_EQ(gnd_out.events[5]->types.size(),9);
    ASSERT_EQ(gnd_out.events[5]->types.occupied_size(),3);

    ASSERT_TRUE(gnd_out.events[5]->types[0]!=NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[1]==NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[2]==NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[3]!=NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[4]==NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[5]==NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[6]==NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[7]==NULL);
    ASSERT_TRUE(gnd_out.events[5]->types[8]!=NULL);
    
    ASSERT_EQ(gnd_out.events[5]->types[0]->type(), VALUE_INT4);
    ASSERT_EQ(gnd_out.events[5]->types[0]->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd_out.events[5]->types[0]->is_primitive(), true);
    
    ASSERT_EQ(gnd_out.events[5]->types[3]->type(), VALUE_INT4);
    ASSERT_EQ(gnd_out.events[5]->types[3]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd_out.events[5]->types[3]->is_primitive(), false);
    
    ASSERT_EQ(gnd_out.events[5]->types[8]->type(), VALUE_INT8);
    ASSERT_EQ(gnd_out.events[5]->types[8]->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd_out.events[5]->types[8]->is_primitive(), true);

}
#endif 
#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
