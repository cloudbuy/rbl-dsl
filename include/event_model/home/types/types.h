#ifndef RBL_EM_HOME_TYPES_H
#define RBL_EM_HOME_TYPES_H
#include "meta_primitives.h"

namespace rubble { namespace event_model { 

  /// the runtime type used to represent the ordinal
  typedef uint8_t type_ordinal_type;

  /// the traits of interest.
  template<typename tag> struct rbl_type_type_traits;
  template<typename tag> struct rbl_type_parser_rule;

  } } // namespace

  /// adding a type has two steps; step one the types ned to be included
  #include "0_undefined.h"
  #include "1_int4.h"
  #include "2_int8.h"
  //#include "3_rbl_string.h"


  namespace rubble { namespace event_model { 

  // step two: the types of interest need to be wired up as  a mpl sequence
  typedef boost::mpl::vector3< 
    rbl_undefined, 
    rbl_int4, 
    rbl_int8> rbl_type_mpl_vector;
  //  rbl_string> rbl_type_mpl_vector;

  // create a variant out of the supported type, this is the concrete runtime 
  // representation of a type.
  typedef boost::make_variant_over<rbl_type_mpl_vector>::type 
    rbl_type_value_variant;

  // helper template forward functor
  template<typename T> 
  struct get_type_ordinal_f
  {
    typedef typename get_ordinal_in_sequence_f<rbl_type_mpl_vector, T>::pos pos;
  };

  } } 
  #endif

  #ifdef RBL_TYPE_HEADER_SPIRIT_PARSING
  #ifndef RBL_EM_TYPES_SPIRIT_PARSING__
  #define RBL_EM_TYPES_SPIRIT_PARSING__
  #include <boost/spirit/include/qi_symbols.hpp>
  #include <boost/spirit/include/karma_symbols.hpp>
  #include <algorithm>
  #include <iostream>


  namespace rubble { namespace event_model { 
  typedef boost::spirit::qi::symbols<char,type_ordinal_type> rbl_type_symbols_t;

  struct RblTypes : rbl_type_symbols_t
  {
    struct populate_symbols_f 
    { 
      template<typename T>
      void operator()(T, rbl_type_symbols_t & s)
      {
        type_ordinal_type ord = 
          get_ordinal_in_sequence_f<rbl_type_mpl_vector,T>::pos::value;
        typename rbl_type_type_traits<T>::dsl_strings dsl_strings;
        int dsl_string_count = dsl_strings.count();

        for(int i =0; i< dsl_string_count; ++i)
        {
          s.add(  dsl_strings()[i],
                  get_ordinal_in_sequence_f<rbl_type_mpl_vector, T>::pos::value);
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

  typedef boost::spirit::karma::symbols<type_ordinal_type, char const *> rbl_type_reverse_symbols_t;

  struct OrdinalToTypeST : rbl_type_reverse_symbols_t
  {
    struct populate_reverse_symbols_f
    {
      template<typename T>
      void operator()(T, rbl_type_reverse_symbols_t & me)
      {
        type_ordinal_type ord = 
          get_ordinal_in_sequence_f<rbl_type_mpl_vector,T>::pos::value;
        typename rbl_type_type_traits<T>::dsl_strings dsl_strings;

        me.add( ord, dsl_strings()[0]);
      }
    };

    OrdinalToTypeST()
    {
      boost::mpl::for_each<rbl_type_mpl_vector>(boost::bind<void>
        ( populate_reverse_symbols_f(),_1,*this));
    }
  };

  #if 0
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

  /// \TODO remove the following struct with the general ordinal_type_map defined above
  struct rbl_type_string_parser_rules
    : rbl_rule_array_t
  {
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

    struct set_rule_f
    {
      template<typename T> 
      void operator()(T, rbl_rule_array_t & s)
      {
         typedef typename get_ordinal_in_sequence_f<rbl_type_mpl_vector,T>::pos pos;
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
  #endif
} } // namespace


#endif 
#endif
