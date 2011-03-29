#include "EventGrammarNew.h"
#include <gtest/gtest.h>
#include <string>

typedef std::string::iterator c_s_it;

using namespace event_model;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

TEST(parser_test, valid_name)
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

TEST(parser_test, identifier_pair)
{
    parser::IdentifierRules<c_s_it> rules;
    
    std::string identifier_pair_string = "1:hassan";
    c_s_it beg = identifier_pair_string.begin();
    c_s_it end = identifier_pair_string.end();
    
    Oid oid;    

    bool res = qi::parse(beg,end,rules.ordinal_string_identifier(phoenix::ref(oid)));
    ASSERT_TRUE(res);

    ASSERT_EQ(oid.ordinal(),  1);
    ASSERT_EQ(oid.name(),"hassan") << oid.name();
    //ASSERT_ANY_THROW( boost::spirit::qi::parse(beg,end,rules.identifier_pair));
}
/*
TEST(parser_test, event_data_line)
{
    parser::Rules<c_s_it> rules;
    
    std::string data_line_string = "OPTIONAL 0:hassan INT4;";
    c_s_it beg = data_line_string.begin();
    c_s_it end = data_line_string.end();
    
    bool res = qi::phrase_parse(beg,end,rules.event_data_line, ascii::space);
    ASSERT_TRUE(res);

    // data_line_string = "0hassan";
    // beg = data_line_string.begin();
    // end = data_line_string.end();

}
*/
int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
