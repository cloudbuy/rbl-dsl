#ifndef _EP_EVENT_STRING_GENERATOR_H
#define _EP_EVENT_STRING_GENERATOR_H
#include <event_model/Types.h>
#include <event_model/DescriptorsCommon.h>
#include <boost/spirit/include/karma.hpp>
#include <string>
#include <ostream>

#define BASE_RULE_SIGNATURE std::back_insert_iterator<std::string>,     \
        void(   const event_model::EventDescriptorBase &,               \
                const event_model::value_variant_vector &),             \
        boost::spirit::karma::locals<uint32_t>
                

struct event_string_generator_grammar : 
    boost::spirit::karma::grammar<BASE_RULE_SIGNATURE>
{
    // state //////////////////////////////////////////////////////////////////
    uint16_t current_value_type;
    uint16_t current_ordinal;
    const event_model::value_variant * current_value_ptr;
    // local functions ////////////////////////////////////////////////////////
    void SetLocalValuePtrAtOrdinal( const event_model::value_variant_vector &, 
                                    uint32_t ordinal);
    // Grammar ////////////////////////////////////////////////////////////////
    boost::spirit::karma::rule
        <   BASE_RULE_SIGNATURE                         >   base_rule; 
    
    boost::spirit::karma::rule
        <   std::back_insert_iterator<std::string>, 
            void(   const event_model::value_variant &) >   quoted_type_rule;  
    
    boost::spirit::karma::rule
        <   std::back_insert_iterator<std::string>, 
            void(   const event_model::value_variant &) >   terminal_type_rule;
   // Constructor ///////////////////////////////////////////////////////////// 
    event_string_generator_grammar();
};

#undef BASE_RULE_SIGNATURE

#endif 
