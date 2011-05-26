#ifndef _EM_DESCRIPTOR_KARMA_GENERATORS
#define _EM_DESCRIPTOR_KARMA_GENERATORS
#include "event_model/DescriptorsCommon.h"
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <string>

namespace rubble { namespace event_model { namespace descriptors {
  namespace karma = boost::spirit::karma;
  namespace phoenix = boost::phoenix;

  #define TYPE_CONTAINER_BASE_RULE_SIGNATURE                \
      std::back_insert_iterator<std::string>,               \
      void (const EventTypeDescriptor &),                   \
      boost::spirit::karma::locals<uint32_t>


  struct EventTypeContainerGenerator: 
      boost::spirit::karma::grammar<TYPE_CONTAINER_BASE_RULE_SIGNATURE>
  {
      boost::spirit::karma::rule <TYPE_CONTAINER_BASE_RULE_SIGNATURE> base_rule;
      
      EventTypeContainerGenerator(); 
      
  };

  #undef TYPE_CONTAINER_BASE_RULE_SIGNATURE 

  #define EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE            \
      std::back_insert_iterator<std::string>,             \
      void (const EventDescriptorBase &),                 \
      boost::spirit::karma::locals<uint32_t,uint32_t>


  struct  BaseEventDescriptorGenerator 
      : boost::spirit::karma::grammar<EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE>
  {
      karma::rule<EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE> base_rule;

      BaseEventDescriptorGenerator();
  };
} } }
#undef  EVENT_DESCRIPTOR_BASE_RULE_SIGNATURE

#endif //_EM_DESCRIPTOR_KARMA_GENERATORS
