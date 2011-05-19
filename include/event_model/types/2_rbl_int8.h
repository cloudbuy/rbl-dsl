#ifndef RBL_EM_TYPE_INT8_H
#define RBL_EM_TYPE_INT8_H

#include <boost/cstdint.hpp>


namespace rubble { namespace event_model { namespace types {

typedef boost::int64_t rbl_int8;

template<> 
struct rbl_ordinal_type_traits<2>
{
  typedef rbl_int8 type;
};

struct rbl_int8_runtime_string
{
  const char * operator()()
  {
    return "int";
  }
};

struct rbl_int8_dsl_strings
{
  int count()
  {
    return 2;
  }
  const char ** operator()()
  {
    static const char * r[] = {"bigint","int8"};
    return  r;
  }  
};

template<>
struct rbl_type_type_traits<rbl_int8>
{
  static const rbl_type_ordinal_type ordinal = 2;
  typedef      rbl_int8_runtime_string runtime_string;
  typedef      rbl_int8_dsl_strings dsl_strings;
};
} } }

#endif

#ifdef RBL_TYPE_HEADER_SPIRIT_PARSING

#ifndef RBL_TYPE_INT8_STRING_PARSING
#define RBL_TYPE_INT8_STRING_PARSING
#include <boost/spirit/include/qi.hpp>
#include <string>
namespace rubble { namespace event_model { namespace types {

struct rbl_int8_parser_rule_definition
{
  typedef boost::spirit::qi::rule<std::string::iterator, rbl_int8()> rule_type;
  
  boost::spirit::qi::int_parser<rbl_int8> parser_int64_t;
  
  rbl_int8_parser_rule_definition & operator=
    (const rbl_int8_parser_rule_definition & rhs) 
  {
  }
  rule_type rule;
  
  rbl_int8_parser_rule_definition()
  {
    rule.name("rbl in8 rule");
    rule = parser_int64_t;  
  }
};


template<>
struct rbl_type_parser_rule<rbl_int8>
{
  typedef rbl_int8_parser_rule_definition string_parser;
};

} } }
#endif 
#endif
