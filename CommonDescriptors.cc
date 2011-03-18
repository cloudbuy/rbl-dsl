#include "CommonDescriptors.h"

namespace event_model
{
using namespace primitives;

EventTypeDescriptor::EventTypeDescriptor() {} 

EventTypeDescriptor::EventTypeDescriptor( 
    EVENT_DESCRIPTOR_QUALIFIER _qualifier, 
    VALUE_TYPE _type)
    :   qualifier(_qualifier),
        type(_type)
{
    
}

void EventTypeDescriptor::serialize(SF::Archive & ar)
{
    ar & qualifier & type & primitive_;
}

}
