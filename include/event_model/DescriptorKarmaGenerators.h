#ifndef _EM_DESCRIPTOR_KARMA_GENERATORS
#define _EM_DESCRIPTOR_KARMA_GENERATORS
#include "event_model/DescriptorsCommon.h"
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

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
    void (const event_model::EventDescriptorBase &),        \
    boost::spirit::karma::locals<uint32_t>


template<typename iterator>
struct  EventDescriptorGenerator 
    : boost::spirit::karma::grammar<EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE>
{
    karma::rule<EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE> base_rule;

    #define _EVENT_ORDINAL                                          \
    phoenix::bind(&event_model::EventDescriptorBase::ordinal,_r1)
    
    #define _EVENT_NAME                                             \
    phoenix::bind(&event_model::EventDescriptorBase::name,_r1)

    EventDescriptorGenerator() 
        : EventDescriptorGenerator::base_type(base_rule)
    {
        using karma::eps;
        using karma::_r1;
        using karma::char_;
        using karma::stream;
        using karma::eol;
        using karma::int_;

        base_rule = 
            int_ (_EVENT_ORDINAL) << char_(':') << stream(_EVENT_NAME) << eol 
            << char_('{') << 
            char_('}')
        ;    
    }
};

#undef _EVENT_ORDINAL
#undef _EVENT_NAME
#undef  EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE

#endif //_EM_DESCRIPTOR_KARMA_GENERATORS
