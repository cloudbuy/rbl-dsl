inline const bool 
EventTypeDescriptor::is_primitive() const {return primitive_; } 

inline const EVENT_DESCRIPTOR_QUALIFIER 
EventTypeDescriptor::qualifier() const
    { return qualifier_; }

inline const VALUE_TYPE 
EventTypeDescriptor::type() const { return type_; }     

inline EventTypeDescriptor::EventTypeDescriptor()
    :   qualifier_(ENTRY_UNINITIALIZED), 
        type_(VALUE_UNINITIALIZED), 
        primitive_(false) {} 

inline EventTypeDescriptor::EventTypeDescriptor( 
    EVENT_DESCRIPTOR_QUALIFIER _qualifier, 
    VALUE_TYPE _type,
    bool primitive_in)
:   qualifier_(_qualifier),
    type_(_type),
    primitive_(primitive_in) {}

inline void EventTypeDescriptor::serialize(SF::Archive & ar)
{
    ar & qualifier_ & type_ & primitive_;
}
///////////////////////////////////////////////////////////////////////////////
inline GeneratorEventDescriptor::GeneratorEventDescriptor()
    :  events_() {}
inline GeneratorEventDescriptor::GeneratorEventDescriptor
    ( const EventTypeContainer & etc) : events_(etc) {}
inline const EventTypeContainer & GeneratorEventDescriptor::events() const
    { return events_; }
///////////////////////////////////////////////////////////////////////////////
inline RelayEventDescriptor::RelayEventDescriptor()
    :  events_() {}

inline RelayEventDescriptor::RelayEventDescriptor
    (const  EventTypeContainer & etc) : events_(etc) {}

inline RelayEventDescriptor::operator GeneratorEventDescriptor()
    { return GeneratorEventDescriptor(events_); }

inline const EventTypeContainer & RelayEventDescriptor::events() const 
    { return events_; }

inline RelayEventDescriptor::RelayEventDescriptor
    (const MarshallEventDescriptor & rhs)
    : events_(rhs.events()) {}
///////////////////////////////////////////////////////////////////////////////
inline const EventTypeContainer & MarshallEventDescriptor::events() const 
    { return events_; }

inline MarshallEventDescriptor::MarshallEventDescriptor()
    : events_() {}

inline MarshallEventDescriptor::operator RelayEventDescriptor()
    { return RelayEventDescriptor(events_);  }

inline const EventTypeContainer & MarshallEventDescriptorBuilder::events()
    const { return events_; }
///////////////////////////////////////////////////////////////////////////////
inline const std::string & GeneratorNamespaceDescriptor::name() const
    { return name_; }

inline const GeneratorNamespaceDescriptor::EventDescriptorContainer & 
    GeneratorNamespaceDescriptor::events() const
        { return events_; }
///////////////////////////////////////////////////////////////////////////////

inline const std::string & RelayNamespaceDescriptor::name() const
    { return name_; }

inline const RelayNamespaceDescriptor::EventDescriptorContainer & 
    RelayNamespaceDescriptor::events() const
        { return events_; }
///////////////////////////////////////////////////////////////////////////////
inline MarshallNamespaceDescriptor::MarshallNamespaceDescriptor
    (const MarshallNamespaceDescriptorBuilder & nsb)
    : name_(nsb.name()), events_(nsb.events()) {}

inline const std::string & MarshallNamespaceDescriptor::name() const
    { return name_; }

inline const MarshallNamespaceDescriptor::EventDescriptorContainer & 
    MarshallNamespaceDescriptor::events() const
        { return events_; }
///////////////////////////////////////////////////////////////////////////////
inline const std::string & MarshallNamespaceDescriptorBuilder::name() const
    { return name_; }

inline const MarshallNamespaceDescriptorBuilder::EventDescriptorContainer & 
    MarshallNamespaceDescriptorBuilder::events() const
        { return events_; }

inline MarshallNamespaceDescriptorBuilder::MarshallNamespaceDescriptorBuilder
    (const std::string & s) : name_(s), events_()  { }
///////////////////////////////////////////////////////////////////////////////
