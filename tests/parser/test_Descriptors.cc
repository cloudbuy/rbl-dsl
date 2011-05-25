#include "marshall/DescriptorBuilders.h"
#include "event_model/detail/DescriptorsCommon-Serialization.h"
#include "relay/Descriptors-Serialization.h"
#include "generator/Descriptors-Serialization.h"
#include <gtest/gtest.h>

#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>

using namespace rubble::event_model::descriptors;

TEST(descriptor_building_and_serialization, event_entry_serialization_test)
{
    EventTypeDescriptor etd_default_constructed;
    EventTypeDescriptor etd_source(ENTRY_REQUIRED, rbl_types::rbl_int4(), false);
    
    ASSERT_TRUE(etd_default_constructed.qualifier() == ENTRY_UNINITIALIZED);
    ASSERT_EQ(etd_default_constructed.type(),
      rbl_types::get_type_ordinal_f<rbl_types::rbl_undefined>::pos::value);
    ASSERT_TRUE(etd_default_constructed.is_primitive() == false);
  
    ASSERT_TRUE(etd_source.qualifier() == ENTRY_REQUIRED);
    ASSERT_EQ(etd_source.type(),
      rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_TRUE(etd_source.is_primitive() == false);

    std::stringstream ss;
    SF::OBinaryStream os(ss);   
    os << etd_source;
    
    SF::IBinaryStream is(ss);
    is >> etd_default_constructed;
    
    ASSERT_TRUE(etd_default_constructed.qualifier() == ENTRY_REQUIRED);
    ASSERT_EQ(etd_default_constructed.type(),
      rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
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
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb.AddEventType(  hassan_zero,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    
    medb.AddEventType(  hassan_zero_ord_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    medb.AddEventType(  hassan_zero_name_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
}


TEST(descriptor_building_and_serialization, event_descriptor_building_test)
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
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb.AddEventType(  hassan_zero,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    
    medb.AddEventType(  hassan_zero_ord_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    medb.AddEventType(  hassan_zero_name_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
}

TEST(descriptor_building_and_serialization, exhaustive_mndb_building_test)
{
    MarshallNamespaceDescriptorBuilder mndb("testing",0);
    ASSERT_TRUE(mndb.name() == "testing");
    ASSERT_TRUE(mndb.event_container_size() == 0); 
    ASSERT_TRUE(mndb.event_container_occupied_size() == 0);

    MarshallEventDescriptorBuilder medb1;
    MarshallEventDescriptorBuilder medb2;
    bool res;
    Oid one("monkey",0);
    Oid two("zebra",3);
    Oid three("giraffe",8);
    
    medb1.Init(Oid("evenT",0),mndb,res);
    ASSERT_TRUE(res);

    medb1.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.Init(Oid("monkey",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);

    mndb.AddEventDescriptor(medb1,res);
    ASSERT_TRUE(res);
    
    mndb.AddEventDescriptor(medb2,res);
    ASSERT_TRUE(res);

    ASSERT_EQ(mndb.event_container_size(),6) ;    
    ASSERT_EQ(mndb.event_container_occupied_size(), 2);
    
    // first test if medb1 and medb2 are structuraly correct
    ASSERT_TRUE(mndb.EventAt(0) != NULL);
    ASSERT_TRUE(mndb.EventAt(1) == NULL);
    ASSERT_TRUE(mndb.EventAt(2) == NULL);
    ASSERT_TRUE(mndb.EventAt(3) == NULL);
    ASSERT_TRUE(mndb.EventAt(4) == NULL);
    ASSERT_TRUE(mndb.EventAt(5) != NULL);
  
    ASSERT_TRUE( mndb.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( mndb.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( mndb.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(mndb.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(mndb.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ(mndb.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(mndb.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(mndb.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(3)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(8)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(mndb.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(mndb.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(mndb.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(0)->type(),  rbl_types::get_type_ordinal_f< rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(8)->is_primitive(), true);
}

TEST(descriptor_building_and_serialization, descriptor_downcast_slicing_tests)
{
    MarshallNamespaceDescriptorBuilder mndb("testing",0);
    ASSERT_TRUE(mndb.name() == "testing");
    ASSERT_TRUE(mndb.event_container_size() == 0); 
    ASSERT_TRUE(mndb.event_container_occupied_size() == 0);

    MarshallEventDescriptorBuilder medb1;
    MarshallEventDescriptorBuilder medb2;
    bool res;
    Oid one("monkey",0);
    Oid two("zebra",3);
    Oid three("giraffe",8);
    
    medb1.Init(Oid("evenT",0),mndb,res);
    ASSERT_TRUE(res);

    medb1.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.Init(Oid("monkey",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);

    mndb.AddEventDescriptor(medb1,res);
    ASSERT_TRUE(res);
    
    mndb.AddEventDescriptor(medb2,res);
    ASSERT_TRUE(res);

    MarshallNamespaceDescriptor mnd = mndb;
    
    ASSERT_TRUE(mnd.EventAt(0) != NULL);
    ASSERT_TRUE(mnd.EventAt(1) == NULL);
    ASSERT_TRUE(mnd.EventAt(2) == NULL);
    ASSERT_TRUE(mnd.EventAt(3) == NULL);
    ASSERT_TRUE(mnd.EventAt(4) == NULL);
    ASSERT_TRUE(mnd.EventAt(5) != NULL);
 
    ASSERT_TRUE( mnd.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( mnd.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( mnd.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(mnd.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(mnd.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( typeid((*mnd.EventWithName("Monkey"))) 
        ,typeid(*new MarshallEventDescriptor()) );

    ASSERT_EQ(mnd.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(mnd.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(mnd.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(mnd.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(mnd.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(mnd.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(8)->is_primitive(), true);

    RelayNamespaceDescriptor rnd = mnd;

    ASSERT_TRUE(rnd.EventAt(0) != NULL);
    ASSERT_TRUE(rnd.EventAt(1) == NULL);
    ASSERT_TRUE(rnd.EventAt(2) == NULL);
    ASSERT_TRUE(rnd.EventAt(3) == NULL);
    ASSERT_TRUE(rnd.EventAt(4) == NULL);
    ASSERT_TRUE(rnd.EventAt(5) != NULL);
 
    ASSERT_TRUE( rnd.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( rnd.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( rnd.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(rnd.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(rnd.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( (typeid(*rnd.EventWithName("Monkey"))) 
        ,typeid(*new RelayEventDescriptor()) );

    ASSERT_EQ(rnd.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(rnd.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(rnd.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(rnd.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->is_primitive(), true);

    GeneratorNamespaceDescriptor gnd = rnd;

    ASSERT_TRUE(gnd.EventAt(0) != NULL);
    ASSERT_TRUE(gnd.EventAt(1) == NULL);
    ASSERT_TRUE(gnd.EventAt(2) == NULL);
    ASSERT_TRUE(gnd.EventAt(3) == NULL);
    ASSERT_TRUE(gnd.EventAt(4) == NULL);
    ASSERT_TRUE(gnd.EventAt(5) != NULL);
 
    ASSERT_TRUE( gnd.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( gnd.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( gnd.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(gnd.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(gnd.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( typeid((*gnd.EventWithName("Monkey"))) 
        ,typeid(*new GeneratorEventDescriptor()) );

    ASSERT_EQ(gnd.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(gnd.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(gnd.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(gnd.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(gnd.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(gnd.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(8)->is_primitive(), true);
}

// there is a serialization issue for the new class heirarchy, rather than 
// understand the code above I decided to write a new test from scratch.
TEST( descriptor_building_and_serialization, base_descriptor_serialization_test)
{
  Oid event_oid("hassan",0);
  ordinal_type ns_o(0);

  EventTypeContainer etc;  

  Oid one("monkey",0);
  Oid two("zebra",1);
  Oid three("giraffe",2);
  
  EventTypeDescriptor etd1(ENTRY_REQUIRED,rbl_types::rbl_int4(),true);
  EventTypeDescriptor etd2(ENTRY_OPTIONAL,rbl_types::rbl_int8(),false);
  EventTypeDescriptor etd3(ENTRY_REQUIRED,rbl_types::rbl_int4(),true);
  
  EventTypeContainer::entry_type oet1(one, etd1);
  EventTypeContainer::entry_type oet2(two, etd2);
  EventTypeContainer::entry_type oet3(three,etd3);
 
  etc.SetEntry(oet1);
  etc.SetEntry(oet2);
  etc.SetEntry(oet3);

  EventDescriptorBase etb(event_oid, ns_o, etc);
  
  ASSERT_EQ(etb.type_container_size(),3);
  ASSERT_EQ(etb.type_container_occupied_size(),3);

  std::stringstream ss;
  SF::OBinaryStream os(ss);   
  os << etb;

  EventDescriptorBase etb_out;
  
  SF::IBinaryStream is(ss);
  is >> etb_out;
 
  ASSERT_EQ(etb_out.type_container_size(),3);
  ASSERT_EQ(etb_out.type_container_occupied_size(),3);
  
  EXPECT_EQ(etb_out.TypeAt(0)->qualifier(),ENTRY_REQUIRED); 
  EXPECT_EQ(etb_out.TypeAt(1)->qualifier(),ENTRY_OPTIONAL);
  EXPECT_EQ(etb_out.TypeAt(2)->qualifier(),ENTRY_REQUIRED);

  EXPECT_EQ(etb_out.TypeAt(0)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
  EXPECT_EQ(etb_out.TypeAt(1)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
  EXPECT_EQ(etb_out.TypeAt(2)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);

  EXPECT_EQ(etb_out.TypeAt(0)->is_primitive(),true);
  EXPECT_EQ(etb_out.TypeAt(1)->is_primitive(),false);
  EXPECT_EQ(etb_out.TypeAt(2)->is_primitive(),true);
  
  typedef NamespaceDescriptorBase<EventDescriptorBase> ndb_type;

  ndb_type::t_edc edc;
  edc.SetEntry(one,   etb);
  edc.SetEntry(two,   etb);
  edc.SetEntry(three, etb);
  
  ndb_type ndb("hassan",0,edc);

  ASSERT_EQ(ndb.event_container_size(),3);
  ASSERT_EQ(ndb.event_container_occupied_size(),3);
  
  os << ndb;

  ndb_type ndb_out;  

  is >> ndb_out;

  ASSERT_EQ(ndb_out.event_container_size(),3);
  ASSERT_EQ(ndb_out.event_container_occupied_size(),3);

  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(0)->qualifier(),ENTRY_REQUIRED); 
  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(1)->qualifier(),ENTRY_OPTIONAL);
  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(2)->qualifier(),ENTRY_REQUIRED);

  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(0)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(1)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(2)->type(),rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);

  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(0)->is_primitive(),true);
  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(1)->is_primitive(),false);
  EXPECT_EQ(ndb_out.EventAt(0)->TypeAt(2)->is_primitive(),true);
}

// ok the base classes work fine, why can't I static_cast during
// serialization ? 
TEST(descriptor_building_and_serializationm, static_cast_test)
{
  MarshallNamespaceDescriptorBuilder mndb("testing",0);
  ASSERT_TRUE(mndb.name() == "testing");
  ASSERT_TRUE(mndb.event_container_size() == 0); 
  ASSERT_TRUE(mndb.event_container_occupied_size() == 0);

  MarshallEventDescriptorBuilder medb1;
  MarshallEventDescriptorBuilder medb2;
  bool res;
  Oid one("monkey",0);
  Oid two("zebra",3);
  Oid three("giraffe",8);
  
  medb1.Init(Oid("evenT",0),mndb,res);
  ASSERT_TRUE(res);

  medb1.AddEventType(  one,
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                      res);
  ASSERT_TRUE(res);
  medb1.AddEventType(  two,
                      EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                      res);
  ASSERT_TRUE(res);
  
  medb1.AddEventType(  three,
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                      res);
  ASSERT_TRUE(res);
  
  medb2.Init(Oid("monkey",5),mndb,res);
  ASSERT_TRUE(res);

  medb2.AddEventType(  one,
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                      res);
  ASSERT_TRUE(res);
  medb2.AddEventType(  two,
                      EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                      res);
  ASSERT_TRUE(res);
  
  medb2.AddEventType(  three,
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                      res);
  ASSERT_TRUE(res);

  mndb.AddEventDescriptor(medb1,res);
  ASSERT_TRUE(res);
  
  mndb.AddEventDescriptor(medb2,res);
  ASSERT_TRUE(res);

  RelayNamespaceDescriptor rnd = (MarshallNamespaceDescriptor) mndb;
  
  ASSERT_TRUE( static_cast<NamespaceDescriptorBase<RelayEventDescriptor> >(rnd).EventAt(0) != NULL);
  ASSERT_TRUE( static_cast<NamespaceDescriptorBase<RelayEventDescriptor> >(rnd).EventAt(1) == NULL);
  ASSERT_TRUE( static_cast<NamespaceDescriptorBase<RelayEventDescriptor> >(rnd).EventAt(2) == NULL);
  ASSERT_TRUE( static_cast<NamespaceDescriptorBase<RelayEventDescriptor> >(rnd).EventAt(3) == NULL);
  ASSERT_TRUE( static_cast<NamespaceDescriptorBase<RelayEventDescriptor> >(rnd).EventAt(4) == NULL);
  ASSERT_TRUE( static_cast<NamespaceDescriptorBase<RelayEventDescriptor> >(rnd).EventAt(5) != NULL);
  
  
}

// descriptor_building_and_serialization
TEST(descriptor_building_and_serialization , exhaustive_serialization_tests)
{
    MarshallNamespaceDescriptorBuilder mndb("testing",0);
    ASSERT_TRUE(mndb.name() == "testing");
    ASSERT_TRUE(mndb.event_container_size() == 0); 
    ASSERT_TRUE(mndb.event_container_occupied_size() == 0);

    MarshallEventDescriptorBuilder medb1;
    MarshallEventDescriptorBuilder medb2;
    bool res;
    Oid one("monkey",0);
    Oid two("zebra",3);
    Oid three("giraffe",8);
    
    medb1.Init(Oid("evenT",0),mndb,res);
    ASSERT_TRUE(res);

    medb1.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.Init(Oid("monkey",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_types::rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_types::rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);

    mndb.AddEventDescriptor(medb1,res);
    ASSERT_TRUE(res);
    
    mndb.AddEventDescriptor(medb2,res);
    ASSERT_TRUE(res);
    
    RelayNamespaceDescriptor rnd = (MarshallNamespaceDescriptor) mndb;

    ASSERT_TRUE(rnd.EventAt(0) != NULL);
    ASSERT_TRUE(rnd.EventAt(1) == NULL);
    ASSERT_TRUE(rnd.EventAt(2) == NULL);
    ASSERT_TRUE(rnd.EventAt(3) == NULL);
    ASSERT_TRUE(rnd.EventAt(4) == NULL);
    ASSERT_TRUE(rnd.EventAt(5) != NULL);
 
    ASSERT_TRUE( rnd.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( rnd.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( rnd.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(rnd.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(rnd.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ(rnd.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(rnd.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(rnd.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(rnd.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(rnd.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(rnd.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->is_primitive(), true);

    std::stringstream ss;
    SF::OBinaryStream os(ss);   
    os << rnd;

    RelayNamespaceDescriptor rnd_out;

    SF::IBinaryStream is(ss);
    is >> rnd_out;
    
    ASSERT_TRUE(rnd_out.EventAt(0) != NULL);
    ASSERT_TRUE(rnd_out.EventAt(1) == NULL);
    ASSERT_TRUE(rnd_out.EventAt(2) == NULL);
    ASSERT_TRUE(rnd_out.EventAt(3) == NULL);
    ASSERT_TRUE(rnd_out.EventAt(4) == NULL);
    ASSERT_TRUE(rnd_out.EventAt(5) != NULL);
 
    ASSERT_TRUE( rnd_out.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( rnd_out.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( rnd_out.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(rnd_out.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(rnd_out.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ( typeid(*rnd_out.EventWithName("Monkey")) 
        ,typeid(*new RelayEventDescriptor()) );

    ASSERT_EQ(rnd_out.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(rnd_out.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(rnd_out.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(rnd_out.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(rnd_out.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(8)->is_primitive(), true);

    GeneratorNamespaceDescriptor gnd = rnd_out;
    GeneratorNamespaceDescriptor gnd_out;
    os << gnd;
    is >> gnd_out;
    
    ASSERT_TRUE(gnd_out.EventAt(0) != NULL);
    ASSERT_TRUE(gnd_out.EventAt(1) == NULL);
    ASSERT_TRUE(gnd_out.EventAt(2) == NULL);
    ASSERT_TRUE(gnd_out.EventAt(3) == NULL);
    ASSERT_TRUE(gnd_out.EventAt(4) == NULL);
    ASSERT_TRUE(gnd_out.EventAt(5) != NULL);
 
    ASSERT_TRUE( gnd_out.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( gnd_out.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( gnd_out.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(gnd_out.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(gnd_out.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ(gnd_out.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(gnd_out.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(gnd_out.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(gnd_out.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(gnd_out.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(0)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(3)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(8)->type(), rbl_types::get_type_ordinal_f<rbl_types::rbl_int8>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(8)->is_primitive(), true);

}

#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
