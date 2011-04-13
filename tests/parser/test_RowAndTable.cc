#include <event_model/detail/OidContainer.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <sstream>
#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>
#include <boost/cstdint.hpp>
//#include <event_model/detail/EventModelGrammar.h>
#include <gtest/gtest.h>
#include <string>
#include <parser/NamespaceParsers.h>
#include <event_model/Descriptors.h>
#include <gtest/gtest.h>
#include <boost/cstdint.hpp>
#include <event_model/detail/OidContainer.h>

#include <typeinfo>
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

#include <event_processing/EventSet.h>

#include <boost/variant/apply_visitor.hpp>

using namespace event_model;

TEST(event_table_test, test_one)
{
    ContainerBuilder<EventTypeContainer> container_builder;
    typedef ContainerBuilder<EventTypeContainer>::entry_type et;

    Oid oid0("monkey",0);
    Oid oid2("zebra",2);
    Oid oid3("donkey",3);
    Oid oid5("baboon",5);
   
    EventTypeDescriptor type_d0; 
    EventTypeDescriptor type_d2;
    EventTypeDescriptor type_d3;
    EventTypeDescriptor type_d5;

    type_d0.set_is_primitive(true);
    type_d0.set_qualifier(ENTRY_REQUIRED);
    type_d0.set_type(VALUE_INT4);
    
    type_d2.set_is_primitive(true);
    type_d2.set_qualifier(ENTRY_REQUIRED);
    type_d2.set_type(VALUE_STRING);
    
    type_d3.set_is_primitive(true);
    type_d3.set_qualifier(ENTRY_REQUIRED);
    type_d3.set_type(VALUE_INT8);

    type_d5.set_is_primitive(true);
    type_d5.set_qualifier(ENTRY_REQUIRED);
    type_d5.set_type(VALUE_STRING);

    container_builder.SetEntry(et(oid0,type_d0));
    container_builder.SetEntry(et(oid2,type_d2));
    container_builder.SetEntry(et(oid3,type_d3));
    container_builder.SetEntry(et(oid5,type_d5)); 
    
    ASSERT_EQ(container_builder.size(), 6);
    ASSERT_EQ(container_builder.occupied_size(),4);

    EventTypeContainer cont = container_builder;
    ASSERT_EQ(cont.size(), 6);
    ASSERT_EQ(cont.occupied_size(),4);

    EventDescriptor td( Oid("test",0), cont);
    ASSERT_EQ(td.entry().size(), 6);
    ASSERT_EQ(td.entry().occupied_size(),4);

    Event event(td);
    ASSERT_EQ(td.RowTypeAt(0), VALUE_INT4);
    ASSERT_EQ(td.RowTypeAt(1), VALUE_UNINITIALIZED);
    ASSERT_EQ(td.RowTypeAt(2), VALUE_STRING);
    ASSERT_EQ(td.RowTypeAt(3), VALUE_INT8);
    ASSERT_EQ(td.RowTypeAt(4), VALUE_UNINITIALIZED);
    ASSERT_EQ(td.RowTypeAt(5), VALUE_STRING);
 
    std::string test_string("0(0=5, 2=\"jungle fever\", 3=12423, 5=\"boring\")");
    EXPECT_TRUE(event << test_string);
    const value_variant_vector & v = event.get_event_vector(); 
    const event_parser_error_descriptor * e_d = event.get_event_parser_error_descriptor();
    ASSERT_TRUE(e_d!=NULL);    
 
    ASSERT_EQ( boost::get<int32_t>( v[0]),5);
    ASSERT_EQ( boost::get<int64_t>( v[3]),12423);
    ASSERT_EQ( boost::get<std::string>( v[2]), "jungle fever");
    ASSERT_EQ( boost::get<std::string>( v[5]), "boring");

    ASSERT_EQ(  boost::apply_visitor( get_type_variant_visitor(),v[0]), 
                VALUE_INT4);
    ASSERT_EQ(  boost::apply_visitor( get_type_variant_visitor(),v[1]), 
                VALUE_UNINITIALIZED);
    ASSERT_EQ(  boost::apply_visitor( get_type_variant_visitor(),v[2]), 
                VALUE_STRING);
    ASSERT_EQ(  boost::apply_visitor( get_type_variant_visitor(),v[3]), 
                VALUE_INT8);
    ASSERT_EQ(  boost::apply_visitor( get_type_variant_visitor(),v[4]), 
                VALUE_UNINITIALIZED);
    ASSERT_EQ(  boost::apply_visitor( get_type_variant_visitor(),v[5]), 
                VALUE_STRING);




    test_string ="0(0=2147483647 , 3=12423)";
    EXPECT_TRUE(event << test_string);

    test_string ="0(0=5, 3=9223372036854775807)";
    EXPECT_TRUE(event << test_string);

    test_string ="0(0=2147483648, 3=12423)";
    EXPECT_FALSE(event << test_string);
    EXPECT_TRUE(e_d->has_error);
    EXPECT_TRUE(e_d->was_parsing_value);
    EXPECT_EQ(e_d->current_ordinal,0);
    EXPECT_EQ(e_d->current_value_type,VALUE_INT4);
    
    test_string ="0(0=5, 3=9223372036854775808)";
    EXPECT_FALSE(event << test_string);
    EXPECT_TRUE(e_d->has_error);
    EXPECT_TRUE(e_d->was_parsing_value);
    EXPECT_EQ(e_d->current_ordinal,3);
    EXPECT_EQ(e_d->current_value_type,VALUE_INT8);
    
    test_string = "0(0=5, 2=\"jungle \"\" fever\", 3=12423, 5=\"bo\"\"ring\")";
    EXPECT_TRUE(event << test_string);
    
    ASSERT_EQ( boost::get<int32_t>( v[0]),5);
    ASSERT_EQ( boost::get<int64_t>( v[3]),12423);
    
    ASSERT_EQ( boost::get<std::string>( v[2]), "jungle \" fever");
    ASSERT_EQ( boost::get<std::string>( v[5]), "bo\"ring");

}

int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
