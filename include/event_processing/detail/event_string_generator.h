#ifndef _EP_EVENT_STRING_GENERATOR_H
#define _EP_EVENT_STRING_GENERATOR_H
#include <event_model/Types.h>
#include <event_model/Descriptors.h>
#include <boost/spirit/include/karma.hpp>
#include <string>

#define BASE_RULE_SIGNATURE std::back_insert_iterator<std::string>,     \
        void(   const event_model::EventDescriptor &,                   \
                const event_model::value_variant_vector &)

struct event_string_generator_grammar : 
    boost::spirit::karma::grammar<BASE_RULE_SIGNATURE>
{
    boost::spirit::karma::rule<BASE_RULE_SIGNATURE> base_rule; 
    
    event_string_generator_grammar();
};

#undef BASE_RULE_SIGNATURE

#endif 
