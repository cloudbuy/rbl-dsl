#ifndef RBL_EM_TYPES_H
#define RBL_EM_TYPES_H

namespace rubble { namespace event_model { namespace types {

typedef unsigned char rbl_type_ordinal_type;

template<rbl_type_ordinal_type ordinal> struct rbl_ordinal_type_traits;
template<typename tag> struct rbl_type_type_traits;
template<typename tag> struct rbl_type_parser_rule;


} } } // namespace

#include "0_rbl_undefined.h"
#include "1_rbl_int4.h"
#include "2_rbl_int8.h"
//#include "3_rbl_string.h"

#include <boost/mpl/vector/vector10.hpp>
#include <boost/variant/variant.hpp>

namespace rubble { namespace event_model { namespace types {

typedef boost::mpl::vector3< 
  rbl_undefined, 
  rbl_int4, 
  rbl_int8> rbl_type_mpl_vector;
//  rbl_string> rbl_type_mpl_vector;

typedef boost::make_variant_over<rbl_type_mpl_vector>::type rbl_type_value_variant;

#ifdef RBL_TYPE_HEADER_SPIRIT_PARSING
#include <boost/mpl/transform.hpp>
struct get_rule_f
{
  template<class T1>
  struct apply
  {
    typedef typename rbl_type_parser_rule<T1>::string_parser type; 
  };
};

typedef boost::mpl::transform<rbl_type_mpl_vector,get_rule_f>::type 
  parser_rules_mpl_vector;

typedef boost::make_variant_over<parser_rules_mpl_vector> rbl_type_string_parser_rules;
#endif 

} } } // namespace


#endif
