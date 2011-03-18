#include "CommonDescriptors.h"

namespace event_model
{
using namespace primitives;

EventTypeDescriptor::EventTypeDescriptor() {} 

EventTypeDescriptor::EventTypeDescriptor( 
    EVENT_DESCRIPTOR_QUALIFIER _qualifier, 
    VALUE_TYPE _type,
    bool primitive_in)
:   qualifier(_qualifier),
    type(_type),
    primitive_(primitive_in)
{
    
}

void EventTypeDescriptor::serialize(SF::Archive & ar)
{
    ar & qualifier & type & primitive_;
}
const bool EventTypeDescriptor::is_primitive() const { return primitive_; }

}
