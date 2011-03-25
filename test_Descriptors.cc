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
    
    medb1.Init(Oid("evenT",0),mndb,res);
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
    
    std::cout << medb1.types().EntryAtordinal(0)->name().c_str() << std::endl;    
    //std::cout << medb1.types().EntryAtordinal(1)->name().c_str() << std::endl;

    ASSERT_TRUE( medb1.type[0] != NULL);
    ASSERT_TRUE( medb1.type[1] == NULL); 
    //ASSERT_TRUE( medb1.types().EntryAtordinal(0)->is_initialized() == true);
   
    EventTypeContainer test_copy_constructor(medb1.types());
    ASSERT_TRUE( test_copy_constructor.EntryAtordinal(0) != NULL);
    ASSERT_TRUE( test_copy_constructor.EntryAtordinal(1) == NULL); 

    std::cout << test_copy_constructor.EntryAtordinal(0)->name().c_str() << std::endl;
 
    MarshallEventDescriptorBuilder medb2;
    MarshallEventDescriptorBuilder init_return_false_damn_you;
    
    // test the downcast
    MarshallEventDescriptor med = medb1;
    ASSERT_TRUE( med.type[0] != NULL);
    ASSERT_TRUE( med.type[1] == NULL); 


 
    medb2.Init(Oid("monkey",5),mndb,res);
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

    mndb.AddEventDescriptor(medb1,res);
    ASSERT_TRUE(res);

    // following should be returning false
 //   init_return_false_damn_you.Init(Oid("eVent",5),mndb,res);
   // ASSERT_FALSE(res);

    
    mndb.AddEventDescriptor(medb2,res);
    ASSERT_TRUE(res);

    ASSERT_EQ(mndb.events().size(),6) ;    
    ASSERT_EQ(mndb.events().occupied_size(), 2);

    // first test if medb1 and medb2 are structuraly correct
    ASSERT_TRUE(mndb.event[0] != NULL);
    ASSERT_TRUE(mndb.event[1] == NULL);
    ASSERT_TRUE(mndb.event[2] == NULL);
    ASSERT_TRUE(mndb.event[3] == NULL);
    ASSERT_TRUE(mndb.event[4] == NULL);
    ASSERT_TRUE(mndb.event[5] != NULL);
//    ASSERT_TRUE(mndb.event[6] == NULL);
    // then downcast to non-builder marshallnamespace and test
 
    ASSERT_EQ(mndb.events().EntryAtordinal(0)->entry().types().occupied_size(),3);
    ASSERT_EQ(mndb.events().EntryAtordinal(0)->entry().types().size(),9);

    ASSERT_EQ(mndb.events().EntryAtordinal(5)->entry().types().occupied_size(),3);
    ASSERT_EQ(mndb.events().EntryAtordinal(5)->entry().types().size(),9);

    ASSERT_TRUE(mndb.event[0]->type[0]!=NULL);
    ASSERT_TRUE(mndb.event[5]->type[0]!=NULL);
    
    ASSERT_TRUE(mndb.events().EntryAtordinal(0)->entry().types().EntryAtordinal(0) 
        != NULL);

    ASSERT_TRUE(mndb.events().EntryAtordinal(0)->entry().types().EntryAtordinal(1) 
        == NULL);
    ASSERT_TRUE(mndb.events().EntryAtordinal(0)->entry().types().EntryAtordinal(2) 
        == NULL);
    
    std::cout << mndb.events().EntryAtordinal(0)->entry().types().EntryAtordinal(0)->name().c_str() << std::endl;
    std::cout << mndb.events().EntryAtordinal(0)->entry().types().EntryAtordinal(3)->name().c_str() << std::endl;
    std::cout << mndb.events().EntryAtordinal(0)->entry().types().EntryAtordinal(8)->name().c_str() << std::endl;


    ASSERT_TRUE(mndb.events().EntryAtordinal(0)->entry().types().EntryAtordinal(1) 
        == NULL);
    
    MarshallEventDescriptor grr= mndb.events().EntryAtordinal(0)->entry();
//    ASSERT_TRUE(grr.type[1] == NULL);
    const MarshallEventDescriptor * grr2 = mndb.event[0];
    ASSERT_TRUE(grr2->types().EntryAtordinal(1) == NULL);
    
    ASSERT_TRUE(mndb.event[0]->types().EntryAtordinal(0)->entry().is_primitive()==true );
    ASSERT_TRUE(mndb.event[0]->types().EntryAtordinal(1) == NULL);
    ASSERT_TRUE(mndb.event[0]->types().EntryAtordinal(2) == NULL);
    ASSERT_TRUE(mndb.event[0]->types().EntryAtordinal(3)->entry().is_primitive()==true );
    ASSERT_TRUE(mndb.event[0]->types().EntryAtordinal(8)->entry().is_primitive()==true );
    //ASSERT_TRUE(mndb.events().EntryAtordinal(0)->entry().types())  

    ASSERT_TRUE(mndb.event[5]->type[1] == NULL);

    //ASSERT_TRUE(mndb.event[0]->type[0]->type() == VALUE_INT4 ); 
    //ASSERT_TRUE(mndb.event[0]->type[0]
    /*
    ASSERT_TRUE(mndb.event[0].type[0]
    ASSERT_TRUE(mndb.event[0].type[0]
    ASSERT_TRUE(mndb.event[0].type[0]
   
    ASSERT_TRUE(mndb.event[0].type[3] 
    ASSERT_TRUE(mndb.event[0].type[3]    
    ASSERT_TRUE(mndb.event[0].type[3]
    ASSERT_TRUE(mndb.event[0].type[3]
    ASSERT_TRUE(mndb.event[0].type[3]
 
    ASSERT_TRUE(mndb.event[0].type[8]  
    ASSERT_TRUE(mndb.event[0].type[8]
    ASSERT_TRUE(mndb.event[0].type[8]
    ASSERT_TRUE(mndb.event[0].type[8]
    ASSERT_TRUE(mndb.event[0].type[8]
    */

        

}



int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
