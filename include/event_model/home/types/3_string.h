#ifndef RBL_EM_TYPE_STRING_H
#define RBL_EM_TYPE_STRING_H
#include <string>

namespace rubble { namespace event_model {

typedef std::string rbl_string;

template<>
struct rbl_type_type_traits<rbl_string>
{
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
  typedef std::string value_type;
  typedef rbl_string_dsl_strings dsl_strings;

};
} }

#endif
