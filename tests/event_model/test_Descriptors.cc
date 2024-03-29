#include "event_model/descriptors_common.h"
#include "event_model/marshall_descriptors.h"
#include "event_model/home/descriptors/marshall_descriptor_builders.h"
#include "event_model/home/descriptors/relay_descriptors-serialization.h"
#include "event_model/home/descriptors/generator_descriptors-serialization.h"


#include <gtest/gtest.h>

#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>

using namespace rubble::event_model;

TEST(descriptor_building_and_serialization, event_entry_serialization_test)
{
    EventTypeDescriptor etd_default_constructed;
    EventTypeDescriptor etd_source(ENTRY_REQUIRED, rbl_int4(), false);
    
    ASSERT_TRUE(etd_default_constructed.qualifier() == ENTRY_UNINITIALIZED);
    ASSERT_EQ(etd_default_constructed.type(),
      get_type_ordinal_f<rbl_undefined>::pos::value);
    ASSERT_TRUE(etd_default_constructed.is_primitive() == false);
  
    ASSERT_TRUE(etd_source.qualifier() == ENTRY_REQUIRED);
    ASSERT_EQ(etd_source.type(),
      get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_TRUE(etd_source.is_primitive() == false);

    std::stringstream ss;
    SF::OBinaryStream os(ss);   
    os << etd_source;
    
    SF::IBinaryStream is(ss);
    is >> etd_default_constructed;
    
    ASSERT_TRUE(etd_default_constructed.qualifier() == ENTRY_REQUIRED);
    ASSERT_EQ(etd_default_constructed.type(),
      get_type_ordinal_f<rbl_int4>::pos::value);
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
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb.AddEventType(  hassan_zero,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    
    medb.AddEventType(  hassan_zero_ord_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    medb.AddEventType(  hassan_zero_name_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
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
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb.AddEventType(  hassan_zero,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    
    medb.AddEventType(  hassan_zero_ord_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_FALSE(res);
    medb.AddEventType(  hassan_zero_name_collision,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
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
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.Init(Oid("monkey",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
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
   
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(3)->type(),get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mndb.EventAt(0)->TypeAt(8)->type(),get_type_ordinal_f<rbl_int8>::pos::value);
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
    
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(0)->type(),  get_type_ordinal_f< rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mndb.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.Init(Oid("monkey",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
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
   
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mnd.EventAt(0)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
    
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(mnd.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
   
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
   
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd.EventAt(0)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
    
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(gnd.EventAt(5)->TypeAt(8)->is_primitive(), true);

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
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                      res);
  ASSERT_TRUE(res);
  medb1.AddEventType(  two,
                      EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                      res);
  ASSERT_TRUE(res);
  
  medb1.AddEventType(  three,
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
                      res);
  ASSERT_TRUE(res);
  
  medb2.Init(Oid("monkey",5),mndb,res);
  ASSERT_TRUE(res);

  medb2.AddEventType(  one,
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                      res);
  ASSERT_TRUE(res);
  medb2.AddEventType(  two,
                      EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                      res);
  ASSERT_TRUE(res);
  
  medb2.AddEventType(  three,
                      EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
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
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb1.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb1.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
                        res);
    ASSERT_TRUE(res);
    
    medb2.Init(Oid("monkey",5),mndb,res);
    ASSERT_TRUE(res);

    medb2.AddEventType(  one,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int4(), true),
                        res);
    ASSERT_TRUE(res);
    medb2.AddEventType(  two,
                        EventTypeDescriptor(ENTRY_OPTIONAL, rbl_int4(), false),
                        res);
    ASSERT_TRUE(res);
    
    medb2.AddEventType(  three,
                        EventTypeDescriptor(ENTRY_REQUIRED, rbl_int8(), true),
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
   
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(0)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
   
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd_out.EventAt(0)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
    
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(rnd_out.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
   
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd_out.EventAt(0)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
    
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(3)->is_primitive(), false);
    
    ASSERT_EQ(gnd_out.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
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
