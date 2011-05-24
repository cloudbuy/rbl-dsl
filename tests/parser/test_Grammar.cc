#include <gtest/gtest.h>
#include <string>
#include <marshall/parser/detail/EventModelGrammar.h>

typedef std::string::iterator c_s_it;
using rubble::event_model::descriptors::Oid;

using namespace rubble::event_model::descriptors;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

TEST(grammar_test, valid_name)
{
    parser::IdentifierRules<c_s_it> rules;
    
    std::string valid_char_str = "hasSan_Syed";
    c_s_it beg = valid_char_str.begin();
    c_s_it end = valid_char_str.end();
    std::string s; 
    bool res = qi::phrase_parse(beg,end,rules.valid_char_str, ascii::space,s);
    ASSERT_TRUE(res);
    ASSERT_EQ(s, valid_char_str);
    
    // "ha22San" won't fail because it will just match "ha"
    valid_char_str = "2hAssAn_Syed";
    beg = valid_char_str.begin();
    end = valid_char_str.end();
    
    res = qi::phrase_parse(beg,end,rules.valid_char_str,ascii::space,s);
    ASSERT_FALSE(res);
}

#if 0
TEST(grammar_test, identifier_pair)
{
    parser::IdentifierRules<c_s_it> rules;
    
    std::string identifier_pair_string = "1:hassan";
    c_s_it      beg = identifier_pair_string.begin();
    c_s_it      end = identifier_pair_string.end();
    
    Oid oid;    

    bool res = qi::parse(beg,end,rules.ordinal_string_identifier(phoenix::ref(oid)));
    ASSERT_TRUE(res);

    ASSERT_EQ(oid.ordinal(),  1);
    ASSERT_EQ(oid.name(),"hassan") << oid.name();
    
    identifier_pair_string = "1hassan";
    beg = identifier_pair_string.begin();
    end = identifier_pair_string.end();
    ASSERT_ANY_THROW( boost::spirit::qi::parse(beg,end,rules.ordinal_string_identifier(phoenix::ref(oid))));
}

TEST(grammar_test, event_data_line)
{
    parser::skipper<c_s_it> skipper;
    parser::CompoundRules<c_s_it, parser::skipper<c_s_it> > compound_rules;
         
    std::string data_line_string = "OPTIONAL 1:hassan INT4;";
    c_s_it beg = data_line_string.begin();
    c_s_it end = data_line_string.end();
        
    bool res=true;
    MarshallNamespaceDescriptorBuilder mndb("hassan",0);
    MarshallEventDescriptorBuilder medb;
    medb.Init( Oid("name", 0), mndb, res);
    ASSERT_TRUE(res); 

    res = qi::phrase_parse(
        beg, end, 
        compound_rules.event_type_line(phoenix::ref(medb)),
        skipper 
    );
    ASSERT_TRUE(res);
    
    ASSERT_TRUE(medb.types[0] == NULL);
    ASSERT_TRUE(medb.types[1] != NULL);
    ASSERT_TRUE(medb.types[2] == NULL);

    ASSERT_EQ(medb.types[1]->qualifier(), ENTRY_OPTIONAL);
    ASSERT_EQ(medb.types[1]->is_primitive(),true);
    ASSERT_EQ(medb.types[1]->type(), VALUE_INT4);

    ASSERT_EQ(medb.types.EntryAtordinal(1)->Id().name(), "hassan");
}

TEST(grammar_test, event_descriptor)
{
    parser::skipper<c_s_it> skipper;
    parser::CompoundRules<c_s_it, parser::skipper<c_s_it> > compound_rules;

    std::string data_line_string = "EVENT 1:hassan              \
                                    //comment               \n  \
                                    {                           \
                                        OPTIONAL 1:hassan INT4; \
                                    // comment              \n  \
                                        REQUIRED 2:monkeys INT8;\
                                    }";
    c_s_it beg = data_line_string.begin();
    c_s_it end = data_line_string.end();
        
    MarshallNamespaceDescriptorBuilder mndb("hassan",0);
    
    bool res =  qi::phrase_parse(
                    beg,end,
                    compound_rules.event_descriptor(phoenix::ref(mndb)),
                   skipper 
                );
    ASSERT_TRUE(res);
    ASSERT_TRUE(mndb.EventAt(1) != NULL);
    EXPECT_EQ(mndb.event_container_occupied_size(),1);
    EXPECT_EQ(mndb.event_container_size(), 2);
    
    ASSERT_TRUE(mndb.EventAt(1)->TypeAt(0) == NULL);
    ASSERT_TRUE(mndb.EventAt(1)->TypeAt(1) != NULL); 
    ASSERT_TRUE(mndb.EventAt(1)->TypeAt(2) != NULL);
    
    EXPECT_EQ( mndb.EventAt(1)->type_container_occupied_size(), 2);
    EXPECT_EQ( mndb.EventAt(1)->type_container_size(),3);
    
    EXPECT_EQ( mndb.EventAt(1)->TypeOidAt(1)->name(), "hassan");
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(1)->is_primitive(), true);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(1)->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(1)->type(), VALUE_INT4);
    
    EXPECT_EQ( mndb.EventAt(1)->TypeOidAt(2)->name(), "monkeys");
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(2)->is_primitive(), true);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(2)->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(2)->type(), VALUE_INT8);
}


TEST(grammar_test, namespace_descriptor)
{
    parser::skipper<c_s_it> skipper;
    parser::CompoundRules<c_s_it, parser::skipper<c_s_it> > compound_rules;
    
    std::string data_line_string = "namespace baboons               \
                                    //comment \n                    \
                                    {                               \
                                        EVENT 1:hassan //comment \n \
                                        { //comment \n              \
                                            OPTIONAL 1:hassan INT4; \
                                            REQUIRED 2:monkeys INT8;\
                                        }                           \
                                        //comment   \n              \
                                        EVENT 6:gorilla             \
                                        {                           \
                                            OPTIONAL 1:hassan INT4; \
                                            REQUIRED 2:monkeys INT8;\
                                        }                           \
                                    }";
    c_s_it beg = data_line_string.begin();
    c_s_it end = data_line_string.end();
    
    MarshallNamespaceDescriptorBuilder mndb;
    
    bool res =  qi::phrase_parse(
                    beg,end,
                    compound_rules.namespace_descriptor(phoenix::ref(mndb)),
                    skipper 
                );
    ASSERT_TRUE(res);
    ASSERT_TRUE(mndb.EventAt(1) != NULL);
    ASSERT_TRUE(mndb.EventAt(6) != NULL);
    EXPECT_EQ(mndb.event_container_occupied_size(),2);
    EXPECT_EQ(mndb.event_container_size(), 7);
    
    ASSERT_TRUE(mndb.EventAt(1)->TypeAt(0) == NULL);
    ASSERT_TRUE(mndb.EventAt(1)->TypeAt(1) != NULL); 
    ASSERT_TRUE(mndb.EventAt(1)->TypeAt(2) != NULL);
    
    EXPECT_EQ( mndb.EventAt(1)->type_container_occupied_size(), 2);
    EXPECT_EQ( mndb.EventAt(1)->type_container_size(),3);
    
    EXPECT_EQ( mndb.EventAt(1)->TypeOidAt(1)->name(), "hassan");
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(1)->is_primitive(), true);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(1)->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(1)->type(), VALUE_INT4);
    
    EXPECT_EQ( mndb.EventAt(1)->TypeOidAt(2)->name(), "monkeys");
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(2)->is_primitive(), true);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(2)->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( mndb.EventAt(1)->TypeAt(2)->type(), VALUE_INT8);
    
    EXPECT_EQ( mndb.EventAt(6)->type_container_occupied_size(), 2);
    EXPECT_EQ( mndb.EventAt(6)->type_container_size(),3);
    
    EXPECT_EQ( mndb.EventAt(6)->TypeOidAt(1)->name(), "hassan");
    EXPECT_EQ( mndb.EventAt(6)->TypeAt(1)->is_primitive(), true);
    EXPECT_EQ( mndb.EventAt(6)->TypeAt(1)->qualifier(), ENTRY_OPTIONAL);
    EXPECT_EQ( mndb.EventAt(6)->TypeAt(1)->type(), VALUE_INT4);
    
    EXPECT_EQ( mndb.EventAt(6)->TypeOidAt(2)->name(), "monkeys");
    EXPECT_EQ( mndb.EventAt(6)->TypeAt(2)->is_primitive(), true);
    EXPECT_EQ( mndb.EventAt(6)->TypeAt(2)->qualifier(), ENTRY_REQUIRED);
    EXPECT_EQ( mndb.EventAt(6)->TypeAt(2)->type(), VALUE_INT8);

}
#endif

#ifdef ISOLATED_GTEST_COMPILE
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
#endif
