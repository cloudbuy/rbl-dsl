#include "event_model/descriptor_karma_generators.h"

namespace rubble { namespace event_model { 
  EventTypeContainerGenerator::EventTypeContainerGenerator()
  : EventTypeContainerGenerator::base_type(base_rule)
  {
    using karma::eps;
    using karma::_r1;

    base_rule = 
      eps
    ;
  }

  BaseEventDescriptorGenerator::BaseEventDescriptorGenerator()
    : BaseEventDescriptorGenerator::base_type(base_rule)
  {
    using karma::eps;
    using karma::_r1;
    using karma::char_;
    using karma::stream;
    using karma::eol;
    using karma::int_;
    using karma::repeat;
    using karma::_a;
    using karma::_b;
    using karma::int_;
    using karma::lit;
    
    #define _EVENT_ORDINAL                                          \
    phoenix::bind(&event_model::EventDescriptorBase::ordinal,_r1)
      
    #define _EVENT_NAME                                             \
    phoenix::bind(&event_model::EventDescriptorBase::name,_r1)
    
    #define _TYPE_CONTAINER_SIZE                                    \
    phoenix::bind(&EventDescriptorBase::type_container_size,_r1)

    #define _CURRENT_VALUE_TYPE                                      \
    phoenix::bind(&EventDescriptorBase::TypeValueTypeAt,_r1,_a)

    base_rule = eps; 
# if 0
      int_ (_EVENT_ORDINAL) << char_(':') << stream(_EVENT_NAME) << eol 
      << char_('{') << eol 
      << eps[_a = 0] <<
      repeat(_TYPE_CONTAINER_SIZE) [
        eps[ _b = _CURRENT_VALUE_TYPE] <<
        (   ( eps( _b == VALUE_UNINITIALIZED)             )
          | ( eps(_b == VALUE_INT4)   <<  int_(_a)  
                << lit("int4")   << eol                   ) 
          | ( eps(_b == VALUE_INT8)   <<  int_(_a)  
                << lit("int8")   << eol                   )
          | ( eps(_b == VALUE_STRING) <<  int_(_a)  
                << lit("string") << eol ) 
          |   eps ) << 
      eps[ _a = _a+1 ]   
      ] <<
      char_('}') << eol
    ;
#endif
    #undef _EVENT_ORDINAL
    #undef _EVENT_NAME
    #undef _TYPE_CONTAINER_SIZE
    #undef _CURRENT_VALUE_TYPE
  }
} } 
