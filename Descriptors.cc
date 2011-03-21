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
        : identifier(), events() {}
    GeneratorEventDescriptor::GeneratorEventDescriptor(const Oid & identifier)
        : identifier(identifier), events() {}

    RelayEventDescriptor::RelayEventDescriptor()
        : GeneratorEventDescriptor() {}
    RelayEventDescriptor::RelayEventDescriptor( const Oid & identifier )
        :   GeneratorEventDescriptor(identifier){}
   
    MarshallEventDescriptor::MarshallEventDescriptor()
        : RelayEventDescriptor() {}
    MarshallEventDescriptor::MarshallEventDescriptor(const Oid & identifier)
        : RelayEventDescriptor( identifier) {}
 
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
        : name() {}
    GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor(
        const std::string & name_in)
        :   name(name_in) {}

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
