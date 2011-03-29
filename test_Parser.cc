#include "EventGrammarNew.h"
#include <gtest/gtest.h>
#include <string>

typedef std::string::const_iterator c_s_it;

using namespace event_model;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

TEST(parser_test, data_name)
{
    parser::Rules<c_s_it> rules;
    
    std::string data_name_string = "hasSan_Syed";
    c_s_it beg = data_name_string.begin();
    c_s_it end = data_name_string.end();
    
    bool res = qi::parse(beg,end,rules.data_name);
    ASSERT_TRUE(res);
    
    // "ha22San" won't fail because it will just match "ha"
    data_name_string = "2hAssAn_Syed";
    beg = data_name_string.begin();
    end = data_name_string.end();
    
    res = qi::parse(beg,end,rules.data_name);
    ASSERT_FALSE(res);
}

TEST(parser_test, identifier_pair)
{
    parser::Rules<c_s_it> rules;
    
    std::string identifier_pair_string = "0:hassan";
    c_s_it beg = identifier_pair_string.begin();
    c_s_it end = identifier_pair_string.end();
    
    bool res = qi::parse(beg,end,rules.identifier_pair);
    ASSERT_TRUE(res);

    identifier_pair_string = "0hassan";
    beg = identifier_pair_string.begin();
    end = identifier_pair_string.end();
    
    
    ASSERT_ANY_THROW( boost::spirit::qi::parse(beg,end,rules.identifier_pair));
}
TEST(parser_test, event_data_line)
{
    parser::Rules<c_s_it> rules;
    
    std::string data_line_string = "OPTIONAL 0:hassan INT4;";
    c_s_it beg = data_line_string.begin();
    c_s_it end = data_line_string.end();
    
    bool res = qi::parse(beg,end,rules.event_data_line, ascii::space);
    ASSERT_TRUE(res);

    // data_line_string = "0hassan";
    // beg = data_line_string.begin();
    // end = data_line_string.end();

}

int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
