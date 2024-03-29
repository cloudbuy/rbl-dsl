#ifndef RBL_EM_TYPE_INT4_H
#define RBL_EM_TYPE_INT4_H

#include <boost/cstdint.hpp>

namespace rubble { namespace event_model {

typedef boost::int32_t  rbl_int4;

template<>
struct rbl_type_type_traits<rbl_int4>
{
  struct rbl_int4_dsl_strings
  {
    int count()
    {
      return 2;
    }
    const char ** operator()()
    {
      static const char * r[] = {"int","int4"};
      return  r;
    }  
  };
  typedef rbl_int4 value_type;
  typedef rbl_int4_dsl_strings    dsl_strings;

};
} }
#endif

#ifdef RBL_TYPE_HEADER_SPIRIT_PARSING

#ifndef RBL_TYPE_INT4_STRING_PARSING
#define RBL_TYPE_INT4_STRING_PARSING
#include <boost/spirit/include/qi.hpp>
#include <string>
namespace rubble { namespace event_model { 

struct rbl_int4_parser_rule_definition
{
  typedef boost::spirit::qi::rule<std::string::iterator, rbl_int4()> rule_type;
  
  boost::spirit::qi::int_parser<rbl_int4> parser_int32_t;
  
  rbl_int4_parser_rule_definition & operator=
    (const rbl_int4_parser_rule_definition & rhs)
  {

  }
  
  rule_type rule;
  
  rbl_int4_parser_rule_definition()
  {
    static boost::spirit::qi::int_parser<rbl_int4> parser_int32_t;

    rule.name("rbl int4 rule");
    rule = parser_int32_t;  
  }
};

template<>
struct rbl_type_parser_rule<rbl_int4>
{
  typedef rbl_int4_parser_rule_definition string_parser;
};

} }
#endif 
#endif
