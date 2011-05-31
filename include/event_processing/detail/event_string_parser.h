#ifndef _EM_ROW_PARSER_H
#define _EM_ROW_PARSER_H

#include <boost/spirit/include/qi.hpp>
#include <event_model/types.h>
#include <event_model/descriptors_common.h>
#include <string>

namespace rubble { namespace event_model {
  struct event_parser_error_descriptor
  {
    event_parser_error_descriptor()
        { reset(); }
    
    void reset()
    {
      has_error = false;
      was_parsing_value = false;
      current_ordinal = 0;
      current_value_type = 0; 
      out_of_range = false;
      double_assignment = false;
    }

    uint16_t current_ordinal;
    uint16_t current_value_type;
    bool was_parsing_value;

    bool has_error;
    bool double_assignment;

    bool out_of_range; 
  };

  struct event_parse_grammar : 
      boost::spirit::qi::grammar< std::string::const_iterator, 
          void(value_variant_vector & , const EventDescriptorBase &),
          boost::spirit::qi::ascii::space_type> 
  {
    void set_value (
      value_variant_vector & v, 
      uint32_t ordinal,
      value_variant & v_in,
      bool & ok_)
    {
      ok_ = true;                                                     
      if( ordinal < v.size() ) 
      {                                      
        if(boost::apply_visitor( 
                type_test_variant_visitor<undefined>(), v[ordinal]))
        {
          ok_ = true;
          v[ordinal] = v_in;
        }
        else
        {                                                       
          rped_.double_assignment = true;
          ok_ = false;
          return;
        }                                                           
      }                                                               
      else 
      {                                                          
        rped_.out_of_range = true;                                        
        ok_ = false;
        return;
      }                                                               
    }

    boost::spirit::qi::int_parser< int32_t,  10, 1,10>  p_int32_t;
    boost::spirit::qi::int_parser< int64_t, 10, 1, 19>  p_int64_t;

    boost::spirit::qi::rule< 
      std::string::const_iterator,std::string(), 
      boost::spirit::ascii::space_type > quoted_string;

    boost::spirit::qi::rule<   std::string::const_iterator,
      void(value_variant_vector &, const EventDescriptorBase &),
      boost::spirit::qi::locals<value_variant>,
      boost::spirit::ascii::space_type>  event_entry;

    boost::spirit::qi::rule<   std::string::const_iterator, 
      void(value_variant_vector &, const EventDescriptorBase &),
      boost::spirit::ascii::space_type> event_rule;

    event_parser_error_descriptor rped_;
    
    void reset() { rped_.reset(); }
    
    event_parse_grammar();
  };
} }
#endif
