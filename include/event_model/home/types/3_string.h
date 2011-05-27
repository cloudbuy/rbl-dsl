#ifndef RBL_EM_TYPE_STRING_H
#define RBL_EM_TYPE_STRING_H
#include <string>

#ifdef RBL_TYPE_HEADER_SPIRIT_CONCERNS
#include <boost/spirit/include/qi_string.hpp>
#endif

namespace rubble { namespace event_model {

typedef std::string rbl_string;

template<> 
struct rbl_ordinal_type_traits<3>
{
  typedef rbl_string type;
};

struct rbl_string_runtime_string
{
  const char * operator()()
  {
    return "int";
  }
};

struct rbl_string_dsl_strings
{
  int count()
  {
    return 1;
  }
  const char ** operator()()
  {
    static const char * r[] = {"string"};
    return  r;
  }  
};

template<>
struct rbl_type_type_traits<rbl_string>
{
  static const rbl_type_ordinal_type ordinal = 3;
  typedef      rbl_string_runtime_string runtime_string;
  typedef      rbl_string_dsl_strings dsl_strings;
  
#ifdef RBL_TYPE_HEADER_SPIRIT_CONCERNS
  typedef std::string string_parser;
#endif
};


} }

#endif
