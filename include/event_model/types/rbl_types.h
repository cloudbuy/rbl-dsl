#ifndef RBL_EM_TYPES_H
#define RBL_EM_TYPES_H
#include <boost/cstdint.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/find.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/bind.hpp>

namespace rubble { namespace event_model { namespace types {

typedef uint8_t rbl_type_ordinal_type;

template<rbl_type_ordinal_type ordinal> struct rbl_ordinal_type_traits;
template<typename tag> struct rbl_type_type_traits;
template<typename tag> struct rbl_type_parser_rule;


} } } // namespace

#include "0_rbl_undefined.h"
#include "1_rbl_int4.h"
#include "2_rbl_int8.h"
//#include "3_rbl_string.h"


namespace rubble { namespace event_model { namespace types {

typedef boost::mpl::vector3< 
  rbl_undefined, 
  rbl_int4, 
  rbl_int8> rbl_type_mpl_vector;
//  rbl_string> rbl_type_mpl_vector;

typedef boost::make_variant_over<rbl_type_mpl_vector>::type rbl_type_value_variant;

template<typename T> 
struct get_type_ordinal_f
{
  typedef typename boost::mpl::find<rbl_type_mpl_vector,T>::type it;
  typedef typename boost::mpl::end<rbl_type_mpl_vector>::type end;
  
  // A failure of the following assertion means that the type
  // is not supported, in other words it's not part of the sequence.
  BOOST_MPL_ASSERT_NOT (( boost::is_same< it, end >));
  
  typedef typename it::pos pos; 

  rbl_type_ordinal_type operator()()
  {
    return it::pos::value;
   }
};
} } } 


#ifdef RBL_TYPE_HEADER_SPIRIT_PARSING
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/array.hpp>
#include <algorithm>
#include <iostream>


namespace rubble { namespace event_model { namespace types {

typedef boost::spirit::qi::symbols<char,rbl_type_ordinal_type> rbl_type_symbols_t;
struct RblTypes : rbl_type_symbols_t
{
  struct populate_symbols_f 
  { 
    template<typename T>
    void operator()(T, rbl_type_symbols_t & s)
    {
      rbl_type_ordinal_type ord = get_type_ordinal_f<T>()();
      typename rbl_type_type_traits<T>::dsl_strings dsl_strings;
      int dsl_string_count = dsl_strings.count();

      for(int i =0; i< dsl_string_count; ++i)
      {
        s.add(dsl_strings()[i],get_type_ordinal_f<T>()());
      }
    }
  };

  /// \TODO replace with insert into vector<string>
  struct print_symbols_f
  {
    template<typename S,typename D>
    void operator()(S & symbols, D & data)
    {
      std::cout << symbols << ":" << (int) data << std::endl;
    }
  };

  void print_symbols()
  {
    this->for_each(print_symbols_f());
  }

  RblTypes()
  {
    boost::mpl::for_each<rbl_type_mpl_vector>(boost::bind<void>
      (populate_symbols_f(),_1,*this));
  }
};


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

typedef boost::make_variant_over<parser_rules_mpl_vector>::type 
  rbl_type_string_parser_variant;

typedef boost::array< rbl_type_string_parser_variant, 
                      boost::mpl::size<parser_rules_mpl_vector>::type::value >
  rbl_rule_array_t;

/// \TODO replace with insert into vector<string>
struct get_rule_at_f
  : public boost::static_visitor<>
{
  template<typename T>
  void operator()(const T & t) const
  {
    std::cout << t.rule.name() << std::endl;
  } 
};

struct rbl_type_string_parser_rules
  : rbl_rule_array_t
{
  struct set_rule_f
  {
    template<typename T> 
    void operator()(T, rbl_rule_array_t & s)
    {
       typedef typename get_type_ordinal_f<T>::pos pos;
       typedef typename boost::mpl::at<parser_rules_mpl_vector, pos>::type rule_type;
    
        s.at(pos::value) = rule_type();
    }
  };
  void print_symbol(const rbl_type_string_parser_variant & v) const
  {
    boost::apply_visitor( get_rule_at_f(), v);
  } 

  void print_symbol_names()
  {
    std::for_each(this->begin(), this->end(), 
      boost::bind<void>(&rbl_type_string_parser_rules::print_symbol,this,_1));
  }
  
  rbl_type_string_parser_rules()
  {
    boost::mpl::for_each<rbl_type_mpl_vector>
      (boost::bind<void>(set_rule_f(), _1,boost::ref(*this)));
  }
};

} } } // namespace


#endif 
#endif
