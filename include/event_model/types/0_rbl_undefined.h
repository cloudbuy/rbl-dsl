#ifndef RBL_EM_TYPE_UNDEFINED_H
#define RBL_EM_TYPE_UNDEFINED_H

#include <boost/spirit/include/support_unused.hpp>

namespace rubble { namespace event_model { namespace types {

typedef boost::spirit::unused_type rbl_undefined;


template<>
struct rbl_type_type_traits<rbl_undefined>
{
  struct rbl_undefined_runtime_string
  {
    const char * operator()()
    {
      return "undefined";
    }
  };

  struct rbl_undefined_dsl_strings
  {
    const unsigned char count()
    {
      return 0;
    }
    const char ** operator()()
    {
      return 0;
    }
  };
  typedef rbl_undefined value_type;
  typedef rbl_undefined_runtime_string runtime_string;
  typedef rbl_undefined_dsl_strings dsl_strings;
};

} } }
#endif

#ifdef RBL_TYPE_HEADER_SPIRIT_PARSING

#ifndef RBL_TYPE_UNDEFINED_STRING_PARSING
#define RBL_TYPE_UNDEFINED_STRING_PARSING
#include <boost/spirit/include/qi.hpp>
#include <string>
namespace rubble { namespace event_model { namespace types {

struct rbl_undefined_parser_rule_definition
{
  typedef boost::spirit::qi::rule<std::string::iterator, void()> rule_type;
  rule_type rule;
  
  rbl_undefined_parser_rule_definition()
  {
    rule.name("undefined parse rule");
    rule = boost::spirit::qi::eps; 
  }
};

template<>
struct rbl_type_parser_rule<rbl_undefined>
{
  typedef rbl_undefined_parser_rule_definition string_parser;
};
} } }
#endif 
#endif
