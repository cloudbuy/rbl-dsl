#include <gtest/gtest.h>
#include <RCF/Idl.hpp>
#include <RCF/TcpEndpoint.hpp>
#include <interface/RelayToServerInterface.h>
#include <event_model/Descriptors.h>


TEST(relay_tests, namespace_serializaton)
{
    using namespace event_model;

    RcfClient<I_NamespaceRepository> MarshNsRepoClient(RCF::TcpEndpoint(50001));
    RelayNamespaceDescriptor mnd
        = MarshNsRepoClient.GetRelayNamespaceDescriptor("baboons");
    
    ASSERT_EQ(mnd.name(), "baboons");

    ASSERT_TRUE(mnd.events[1] != NULL);
    ASSERT_TRUE(mnd.events[6] != NULL);
    EXPECT_EQ(mnd.events.occupied_size(),2);
    EXPECT_EQ(mnd.events.size(), 7);

    ASSERT_TRUE(mnd.events[1]->types[0] == NULL);
    ASSERT_TRUE(mnd.events[1]->types[1] != NULL);
    ASSERT_TRUE(mnd.events[1]->types[2] != NULL);

    EXPECT_EQ( mnd.events[1]->types.occupied_size(), 2);
    EXPECT_EQ( mnd.events[1]->types.size(),3);

    EXPECT_EQ( mnd.events[1]->types.EntryAtordinal(1)->name(), "hassan");
    EXPECT_EQ( mnd.events[1]->types[1]->is_primitive(), true);
    EXPECT_EQ( mnd.events[1]->types[1]->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( mnd.events[1]->types[1]->type(), VALUE_INT4);

    EXPECT_EQ( mnd.events[1]->types.EntryAtordinal(2)->name(), "monkeys");
    EXPECT_EQ( mnd.events[1]->types[2]->is_primitive(), true);
    EXPECT_EQ( mnd.events[1]->types[2]->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( mnd.events[1]->types[2]->type(), VALUE_INT8);

    EXPECT_EQ( mnd.events[6]->types.occupied_size(), 2);
    EXPECT_EQ( mnd.events[6]->types.size(),3);

    EXPECT_EQ( mnd.events[6]->types.EntryAtordinal(1)->name(), "hassan");
    EXPECT_EQ( mnd.events[6]->types[1]->is_primitive(), true);
    EXPECT_EQ( mnd.events[6]->types[1]->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( mnd.events[6]->types[1]->type(), VALUE_INT4);

    EXPECT_EQ( mnd.events[6]->types.EntryAtordinal(2)->name(), "monkeys");
    EXPECT_EQ( mnd.events[6]->types[2]->is_primitive(), true);
    EXPECT_EQ( mnd.events[6]->types[2]->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( mnd.events[6]->types[2]->type(), VALUE_INT8);
}

int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
