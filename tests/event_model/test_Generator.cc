#include "event_model/descriptor_generators.h"
//#include "../descriptors/marshall_descriptor_builders.h"
#include <event_model/home/parser/event_model_grammar.h>

#include <boost/ref.hpp>
#include <string>
#include <iostream>
#include <iterator>
#include <gtest/gtest.h>

using namespace rubble::event_model;

# if 0
int main()
{ 
  Oid oid("temp",1);
  typedef std::back_insert_iterator<std::string> iterator_t;
  std::string str;
  iterator_t sink(str);

  descriptor_generator_primitives<iterator_t> primitives;
  
  bool ret = karma::generate(
    sink,
    primitives.namespace_header(phoenix::cref(oid))
  );
  std::cout << str << std::endl;

  return 0;  
};
# endif

TEST(descriptor_generation , create_desc_generate_and_parse)
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
  
    typedef std::back_insert_iterator<std::string> iterator_t;
    std::string str;
    iterator_t sink(str);
   
    namespace_generator_grammar<iterator_t, MarshallNamespaceDescriptor> nsg;
 
    bool ret = karma::generate(
      sink,
      nsg(phoenix::cref(mndb))
    );
    //std::cout << str << std::endl;
    
    // now  parse the generated string, and test it.
    typedef std::string::iterator c_s_it;

    rubble::event_model::skipper<c_s_it> skipper_;
    CompoundRules<c_s_it, skipper<c_s_it> > compound_rules;
 
    c_s_it beg = str.begin();
    c_s_it end = str.end();

    MarshallNamespaceDescriptorBuilder mndb2;
    
    res =  qi::phrase_parse(
                    beg,end,
                    compound_rules.namespace_descriptor(phoenix::ref(mndb2)),
                    skipper_ 
                );

    ASSERT_EQ(mndb2.event_container_size(),6) ;    
    ASSERT_EQ(mndb2.event_container_occupied_size(), 2);
    
    // first test if medb1 and medb2 are structuraly correct
    ASSERT_TRUE(mndb2.EventAt(0) != NULL);
    ASSERT_TRUE(mndb2.EventAt(1) == NULL);
    ASSERT_TRUE(mndb2.EventAt(2) == NULL);
    ASSERT_TRUE(mndb2.EventAt(3) == NULL);
    ASSERT_TRUE(mndb2.EventAt(4) == NULL);
    ASSERT_TRUE(mndb2.EventAt(5) != NULL);
  
    ASSERT_TRUE( mndb2.EventWithName("MonKey") != NULL);
    ASSERT_TRUE( mndb2.EventWithName("HasSan") ==NULL);
    ASSERT_TRUE( mndb2.EventWithName("eVeNt") !=NULL);

    ASSERT_TRUE(mndb2.EventWithName("MonKey")->ordinal() == 5);
    ASSERT_TRUE(mndb2.EventWithName("eVeNt")->ordinal() == 0);

    ASSERT_EQ(mndb2.EventAt(0)->type_container_size(),9);
    ASSERT_EQ(mndb2.EventAt(0)->type_container_occupied_size(),3);

    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(0)!=NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(1)==NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(2)==NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(3)!=NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(4)==NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(5)==NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(6)==NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(7)==NULL);
    ASSERT_TRUE(mndb2.EventAt(0)->TypeAt(8)!=NULL);
   
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(0)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(3)->type(),get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(3)->is_primitive(), true);
    
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(8)->type(),get_type_ordinal_f<rbl_int8>::pos::value);
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mndb2.EventAt(0)->TypeAt(8)->is_primitive(), true);

    ASSERT_EQ(mndb2.EventAt(5)->type_container_size(),9);
    ASSERT_EQ(mndb2.EventAt(5)->type_container_occupied_size(),3);

    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(0)!=NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(1)==NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(2)==NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(3)!=NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(4)==NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(5)==NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(6)==NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(7)==NULL);
    ASSERT_TRUE(mndb2.EventAt(5)->TypeAt(8)!=NULL);
    
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(0)->type(),  get_type_ordinal_f< rbl_int4>::pos::value);
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(0)->qualifier(), ENTRY_REQUIRED); 
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(0)->is_primitive(), true);
    
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(3)->type(), get_type_ordinal_f<rbl_int4>::pos::value);
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(3)->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(3)->is_primitive(), true);
    
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(8)->type(), get_type_ordinal_f<rbl_int8>::pos::value);
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(8)->qualifier(),ENTRY_REQUIRED);
    ASSERT_EQ(mndb2.EventAt(5)->TypeAt(8)->is_primitive(), true);

}


#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
