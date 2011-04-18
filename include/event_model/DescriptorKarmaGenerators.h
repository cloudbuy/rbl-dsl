#ifndef _EM_DESCRIPTOR_KARMA_GENERATORS
#define _EM_DESCRIPTOR_KARMA_GENERATORS
#include "Descriptors.h"
#include <boost/spirit/include/karma.hpp>

namespace karma = boost::spirit::karma;

#define BASE_RULE_SIGNATURE                             \
    iterator,                                           \
    void (const event_model::EventTypeDescriptor &),    \
    boost::spirit::karma::locals<uint32_t>
 
template<typename iterator>
struct EventTypeGeneratorGrammar : 
    boost::spirit::karma::grammar<BASE_RULE_SIGNATURE>
{
    boost::spirit::karma::rule <BASE_RULE_SIGNATURE> base_rule;

    EventTypeGeneratorGrammar() : EventTypeGeneratorGrammar::base_type(base_rule)
    {
        base_rule = karma::eps;
    }
};

#undef  BASE_RULE_SIGNATURE

#endif //_EM_DESCRIPTOR_KARMA_GENERATORS
