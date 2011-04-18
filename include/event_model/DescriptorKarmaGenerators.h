#ifndef _EM_DESCRIPTOR_KARMA_GENERATORS
#define _EM_DESCRIPTOR_KARMA_GENERATORS
#include "Descriptors.h"
#include <boost/spirit/include/karma.hpp>

namespace karma = boost::spirit::karma;

#define TYPE_CONTAINER_BASE_RULE_SIGNATURE              \
    iterator,                                           \
    void (const event_model::EventTypeDescriptor &),    \
    boost::spirit::karma::locals<uint32_t>


template<typename iterator>
struct EventTypeContainerGenerator: 
    boost::spirit::karma::grammar<TYPE_CONTAINER_BASE_RULE_SIGNATURE>
{
    
    boost::spirit::karma::rule <TYPE_CONTAINER_BASE_RULE_SIGNATURE> base_rule;
    
    EventTypeContainerGenerator() : 
        EventTypeContainerGenerator::base_type(base_rule)
    {
        using karma::eps;
        using karma::_r1;
   
        base_rule =
            eps
        ;
    }
};

#undef TYPE_CONTAINER_BASE_RULE_SIGNATURE 

#define EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE            \
    iterator,                                           \
    void (const event_model::EventDescriptor &),        \
    boost::spirit::karma::locals<uint32_t>


template<typename iterator>
struct  EventDescriptorGenerator 
    : boost::spirit::karma::grammar<EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE>
{
    
    karma::rule<EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE> base_rule;
    
    EventDescriptorGenerator() 
        : EventDescriptorGenerator::base_type(base_rule)
    {
        using karma::eps;
        using karma::_r1;

        base_rule =
            eps
        ;    
    }
};

#undef  EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE

#endif //_EM_DESCRIPTOR_KARMA_GENERATORS
