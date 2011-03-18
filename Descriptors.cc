#include "Descriptors.h"

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

        
    // Builders - MarshallEventDescriptorBuilder
    MarshallEventDescriptorBuilder::MarshallEventDescriptorBuilder()
        : etc_()
    {
        
    }

    void
    MarshallEventDescriptorBuilder::AddEventType(
        const EventTypeContainer::entry_type & et_in, bool & pass_)
    {
        OP_RESPONSE resp = etc_.SetEntry( et_in );
        if(resp == OP_NO_ERROR)
            pass_ = true;
        else
        {
            pass_ = false; 
        }
    }
    // Builders - MarshallNamespaceDescriptorBuilder
    MarshallNamespaceDescriptorBuilder::MarshallNamespaceDescriptorBuilder()
    :  ec_() {}
    
    void MarshallNamespaceDescriptorBuilder::AddEvent 
        ( const EventContainer::entry_type & et,bool & pass )
    {
        OP_RESPONSE resp = ec_.SetEntry(et);
        if( resp == OP_NO_ERROR)
            pass = true;
        else
        {
            pass=false;
        } 
    }
};
