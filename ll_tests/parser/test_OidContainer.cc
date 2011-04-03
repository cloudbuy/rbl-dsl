#include "parser/detail/OidContainer.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <sstream>
#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>
#include <boost/cstdint.hpp>

using namespace event_model::primitives;

typedef OidConstrainedString<char,8> oid_str_type;


TEST( oid_container_tests, constrained_str_comparison_op_tests)
{
    ASSERT_THROW(oid_str_type("lalalala"),std::length_error);
    ASSERT_NO_THROW(oid_str_type("lalalal"));

    oid_str_type str1 ("hassan"); 
    oid_str_type str2 ("gassan");
    oid_str_type str3 ("Hassan");
    
    ASSERT_TRUE(str2 < str1);
    ASSERT_TRUE(str1 > str2);
    ASSERT_TRUE(str1 == str3);
   
    ASSERT_TRUE(str1.is_initialized());    
 
    oid_str_type str4;
    ASSERT_THROW(str4.c_str(), std::invalid_argument);
   
    ASSERT_TRUE(str1=="hassan");
 
    str1 = str2;
    
    ASSERT_TRUE(str1==std::string("gassan"));
}

TEST( oid_container_tests , str_serialization_tests)
{
    std::stringstream ss;
    SF::OBinaryStream os(ss);
    
    oid_str_type s("hassan");
    os << s;
   
    SF::IBinaryStream is(ss); 
    oid_str_type s2;
    is >> s2;
    
    ASSERT_TRUE(s==s2);
}

typedef OidType<oid_str_type, boost::uint8_t> oid;

TEST( oid_container_tests , oid_serialization_tests)
{
    std::stringstream ss;
    SF::OBinaryStream os(ss);
    oid oid1(std::string("hassan"),0);
    os << oid1;
    
    SF::IBinaryStream is(ss);

    oid oid2;
    is >> oid2;
    ASSERT_TRUE(oid2 == oid1);
}

struct x
{
    int y;

    x(): y(0) {}
    x(int _y) : y(_y) {}

    void serialize(SF::Archive & ar)
    {
        ar & y;
    }
};
typedef ContainerBuilder< OidContainer<oid,x> > test_container;


TEST(oid_container_tests, container_entry_test)
{
    test_container::entry_type et1(oid("hassan",0),1);
        
 
    std::stringstream ss;
    SF::OBinaryStream os(ss);

    os << et1;

    SF::IBinaryStream is(ss);
    test_container::entry_type et2;

    is >> et2;
    ASSERT_TRUE(et1==et2);
}

TEST(oid_container_tests , exchaustive_container_tests)
{
    test_container container;
   
    // test getting an entry from an empty container
    // it should be uninitialized.
    ASSERT_TRUE( container.EntryAtordinal(0) == NULL);

    // An index entry should now be added in order to 
    // simulate a empty slot, also the valid entry 
    // needs to be tested.
    x ex(1);
    test_container::entry_type entry(oid("hassan",3),ex);
    ASSERT_TRUE ( container.SetEntry(entry) == OP_NO_ERROR);
    
    ASSERT_TRUE( container.EntryAtordinal(3)->is_initialized());
    ASSERT_TRUE ( container.EntryAtordinal(3)->name() == "hassan");
    ASSERT_TRUE ( container.EntryAtordinal(3)->ordinal() == 3);
   
    ASSERT_TRUE( container.size() == 4);
    ASSERT_TRUE( container.occupied_size() == 1);    
 
    ASSERT_TRUE( container.EntryWithName("hassan")->is_initialized());
    ASSERT_TRUE ( container.EntryWithName("hassan")->name() == "hassan");
    ASSERT_TRUE ( container.EntryWithName("hassan")->ordinal() == 3);

    ASSERT_TRUE( container.EntryAtordinal(0) == NULL);
    ASSERT_TRUE( container.EntryAtordinal(1) == NULL);
    ASSERT_TRUE( container.EntryAtordinal(2) == NULL);

    // attempt to insert overlapping identifiers
    test_container::entry_type 
        entry_three_duplicate_name(oid("hassan",0),ex);
    test_container::entry_type 
        entry_three_duplicate_ordinal(oid("monkey",3),ex);
   
 
    ASSERT_TRUE( container.SetEntry(entry_three_duplicate_name) 
        == OP_NAME_USED); 
    ASSERT_TRUE( container.SetEntry(entry_three_duplicate_ordinal) 
        == OP_ORDINAL_USED); 
    ASSERT_TRUE ( container.SetEntry(entry) == OP_ALLREADY_CONTAINS_ENTRY);

    //double check that a resize wasn't triggered
    ASSERT_TRUE( container.size() == 4);
    ASSERT_TRUE( container.occupied_size() == 1);

    // add an entry into a lower index and test the entire data
    // container.
    test_container::entry_type entry_at_zero(oid("whateva",0),ex); 
    ASSERT_TRUE( container.SetEntry(entry_at_zero) == OP_NO_ERROR);
    
    ASSERT_TRUE( container.size() == 4);
    ASSERT_TRUE( container.occupied_size() == 2);

    ASSERT_TRUE( container.EntryAtordinal(0)->is_initialized());
    ASSERT_TRUE ( container.EntryAtordinal(0)->name() == "whateva");
    ASSERT_TRUE ( container.EntryAtordinal(0)->ordinal() == 0);
    
    ASSERT_TRUE( container.EntryWithName("whateva")->is_initialized());
    ASSERT_TRUE ( container.EntryWithName("whateva")->name() == "whateva");
    ASSERT_TRUE ( container.EntryWithName("whateva")->ordinal() == 0);

    ASSERT_TRUE( container.EntryAtordinal(3)->is_initialized());
    ASSERT_TRUE ( container.EntryAtordinal(3)->name() == "hassan");
    ASSERT_TRUE ( container.EntryAtordinal(3)->ordinal() == 3);
    
    ASSERT_TRUE( container.EntryWithName("hassan")->is_initialized());
    ASSERT_TRUE ( container.EntryWithName("hassan")->name() == "hassan");
    ASSERT_TRUE ( container.EntryWithName("hassan")->ordinal() == 3);

    ASSERT_TRUE( container.EntryAtordinal(1) == NULL);
    ASSERT_TRUE( container.EntryAtordinal(2) == NULL);
    
    // add that causes a resize
    test_container::entry_type entry_at_five(oid("lambda",5),ex);
    ASSERT_TRUE( container.SetEntry(entry_at_five) == OP_NO_ERROR);
    ASSERT_TRUE( container.size() == 6 );
    ASSERT_TRUE( container.occupied_size() == 3);

    // check new entry, then recheck check the previous state
    ASSERT_TRUE( container.EntryAtordinal(5)->is_initialized());
    ASSERT_TRUE ( container.EntryAtordinal(5)->name() == "lambda");
    ASSERT_TRUE ( container.EntryAtordinal(5)->ordinal() == 5);
    
    ASSERT_TRUE( container.EntryWithName("lambda")->is_initialized());
    ASSERT_TRUE ( container.EntryWithName("lambda")->name() == "lambda");
    ASSERT_TRUE ( container.EntryWithName("lambda")->ordinal() == 5);

    ASSERT_TRUE( container.EntryAtordinal(0)->is_initialized());
    ASSERT_TRUE ( container.EntryAtordinal(0)->name() == "whateva");
    ASSERT_TRUE ( container.EntryAtordinal(0)->ordinal() == 0);
    
    ASSERT_TRUE( container.EntryWithName("whateva")->is_initialized());
    ASSERT_TRUE ( container.EntryWithName("whateva")->name() == "whateva");
    ASSERT_TRUE ( container.EntryWithName("whateva")->ordinal() == 0);

    ASSERT_TRUE( container.EntryAtordinal(3)->is_initialized());
    ASSERT_TRUE ( container.EntryAtordinal(3)->name() == "hassan");
    ASSERT_TRUE ( container.EntryAtordinal(3)->ordinal() == 3);
    
    ASSERT_TRUE( container.EntryWithName("hassan")->is_initialized());
    ASSERT_TRUE ( container.EntryWithName("hassan")->name() == "hassan");
    ASSERT_TRUE ( container.EntryWithName("hassan")->ordinal() == 3);

    ASSERT_TRUE( container.EntryAtordinal(1) == NULL);
    ASSERT_TRUE( container.EntryAtordinal(2) == NULL);
    ASSERT_TRUE( container.EntryAtordinal(4) == NULL);

    std::stringstream ss;
    SF::OBinaryStream os(ss);
    os << container;
    
    test_container container2;
    
    SF::IBinaryStream is(ss);
    is >> container2;

    ASSERT_TRUE( container2.size() == 6 );
    ASSERT_TRUE( container2.occupied_size() == 3);

    // check new entry, then recheck check the previous state
    ASSERT_TRUE( container2.EntryAtordinal(5)->is_initialized());
    ASSERT_TRUE ( container2.EntryAtordinal(5)->name() == "lambda");
    ASSERT_TRUE ( container2.EntryAtordinal(5)->ordinal() == 5);
    
    ASSERT_TRUE( container2.EntryWithName("lambda")->is_initialized());
    ASSERT_TRUE ( container2.EntryWithName("lambda")->name() == "lambda");
    ASSERT_TRUE ( container2.EntryWithName("lambda")->ordinal() == 5);

    ASSERT_TRUE( container2.EntryAtordinal(0)->is_initialized());
    ASSERT_TRUE ( container2.EntryAtordinal(0)->name() == "whateva");
    ASSERT_TRUE ( container2.EntryAtordinal(0)->ordinal() == 0);
    
    ASSERT_TRUE( container2.EntryWithName("whateva")->is_initialized());
    ASSERT_TRUE ( container2.EntryWithName("whateva")->name() == "whateva");
    ASSERT_TRUE ( container2.EntryWithName("whateva")->ordinal() == 0);

    ASSERT_TRUE( container2.EntryAtordinal(3)->is_initialized());
    ASSERT_TRUE ( container2.EntryAtordinal(3)->name() == "hassan");
    ASSERT_TRUE ( container2.EntryAtordinal(3)->ordinal() == 3);
    
    ASSERT_TRUE( container2.EntryWithName("hassan")->is_initialized());
    ASSERT_TRUE ( container2.EntryWithName("hassan")->name() == "hassan");
    ASSERT_TRUE ( container2.EntryWithName("hassan")->ordinal() == 3);

    ASSERT_TRUE( container2.EntryAtordinal(1) == NULL);
    ASSERT_TRUE( container2.EntryAtordinal(2) == NULL);
    ASSERT_TRUE( container2.EntryAtordinal(4) == NULL);
   
    // perhaps a operator == is needed ? 
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
