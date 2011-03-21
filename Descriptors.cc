#include "Descriptors.h"

namespace event_model
{
    using namespace primitives;
    
    EventTypeDescriptor::EventTypeDescriptor()
        :   qualifier(ENTRY_UNINITIALIZED), 
            type(VALUE_UNINITIALIZED), 
            primitive(false) {} 

    EventTypeDescriptor::EventTypeDescriptor( 
        EVENT_DESCRIPTOR_QUALIFIER _qualifier, 
        VALUE_TYPE _type,
        bool primitive_in)
    :   qualifier(_qualifier),
        type(_type),
        primitive(primitive_in) {}

    void EventTypeDescriptor::serialize(SF::Archive & ar)
    {
        ar & qualifier & type & primitive;
    }
    
    GeneratorEventDescriptor::GeneratorEventDescriptor()
        :  events() {}
    
    GeneratorEventDescriptor::
    GeneratorEventDescriptor(const RelayEventDescriptor & rhs)
        : events(rhs.events) {}
    GeneratorEventDescriptor::
    GeneratorEventDescriptor(const EventTypeContainer & events_in)
        : events(events_in) {}


    RelayEventDescriptor::RelayEventDescriptor()
        : GeneratorEventDescriptor() {}
    RelayEventDescriptor::
    RelayEventDescriptor(const MarshallEventDescriptor & rhs)
        : GeneratorEventDescriptor(rhs.events) {}
    RelayEventDescriptor::RelayEventDescriptor
        (const EventTypeContainer & rhs) : GeneratorEventDescriptor(rhs) {}
     
    MarshallEventDescriptor::MarshallEventDescriptor()
        : RelayEventDescriptor() {}
    MarshallEventDescriptor::   
    MarshallEventDescriptor(const MarshallEventDescriptorBuilder & rhs)
        : RelayEventDescriptor(rhs.events) {}

 
    void MarshallEventDescriptorBuilder::AddEventTypeEntry( 
        const Oid & oid, 
        const EventTypeDescriptor & ets)
    {
        EventTypeContainer * event_ptr =  
            const_cast<EventTypeContainer *>(&events);
            
        event_ptr->SetEntry(EventTypeContainer::entry_type(oid, ets));  
    }

    //Namespace Objects
    GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor()
        : name(),Events() {}
    GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor(
        const RelayNamespaceDescriptor & rhs)
    {
       name = rhs.name;
        Events=rhs.Events;
    }

    RelayNamespaceDescriptor::RelayNamespaceDescriptor()
        : GeneratorNamespaceDescriptor() {}
    RelayNamespaceDescriptor::RelayNamespaceDescriptor(
        const std::string & name_in) : GeneratorNamespaceDescriptor(name_in) {}

    MarshallNamespaceDescriptor::MarshallNamespaceDescriptor()
        : RelayNamespaceDescriptor() {}
    MarshallNamespaceDescriptor::MarshallNamespaceDescriptor(
        const std::string & name_in) : RelayNamespaceDescriptor(name_in) {}
    
    MarshallNamespaceDescriptorBuilder::MarshallNamespaceDescriptorBuilder()
        : MarshallNamespaceDescriptor() {}
    MarshallNamespaceDescriptorBuilder::MarshallNamespaceDescriptorBuilder(
        const std::string & name_in) : MarshallNamespaceDescriptor(name_in) {}
    
};
