#include "EventGrammarNew.h"
#include <gtest/gtest.h>
#include <string>

typedef std::string::const_iterator c_s_it;

using namespace event_model;

TEST(parser_test, data_name)
{
    parser::Rules<c_s_it> rules;
    
    std::string data_name_string = "hasSan_Syed";
    c_s_it beg = data_name_string.begin();
    c_s_it end = data_name_string.end();
    
    bool res = boost::spirit::qi::parse(beg,end,rules.data_name);
    ASSERT_TRUE(res);
}

TEST(parser_test, identifier_pair)
{
    parser::Rules<c_s_it> rules;
    
    std::string identifier_pair_string = "0:hassan";
    c_s_it beg = identifier_pair_string.begin();
    c_s_it end = identifier_pair_string.end();
    
    bool res = boost::spirit::qi::parse(beg,end,rules.identifier_pair);
    ASSERT_TRUE(res);
}

int main(int argc,char ** argv)
{
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
