#include "Descriptors.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <sstream>
#include <SF/OBinaryStream.hpp>
#include <SF/IBinaryStream.hpp>
#include <boost/cstdint.hpp>

using namespace event_model;

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

int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
