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
    :  types_() {}

inline RelayEventDescriptor::RelayEventDescriptor
    (const  EventTypeContainer & etc) : types_(etc) {}

inline RelayEventDescriptor::operator GeneratorEventDescriptor() const
    { return GeneratorEventDescriptor(types_); }

inline const EventTypeContainer & RelayEventDescriptor::types() const 
    { return types_; }

inline RelayEventDescriptor::RelayEventDescriptor
    (const MarshallEventDescriptor & rhs)
    : types_(rhs.types()) {}
///////////////////////////////////////////////////////////////////////////////
inline MarshallEventDescriptor::MarshallEventDescriptor(const EventTypeContainer & etc)
   : types_(etc) {};

inline const EventTypeContainer & MarshallEventDescriptor::types() const 
    { return types_; }

inline MarshallEventDescriptor::MarshallEventDescriptor()
    : types_() {}

inline MarshallEventDescriptor::operator RelayEventDescriptor() const
    { return RelayEventDescriptor(types_);  }
///////////////////////////////////////////////////////////////////////////////
inline const EventTypeContainer & MarshallEventDescriptorBuilder::events()
   const { return types_; }

inline MarshallEventDescriptorBuilder::operator MarshallEventDescriptor() const
    { return MarshallEventDescriptor(types_); }

inline const Oid & MarshallEventDescriptorBuilder::oid() const
    { return self_oid_; }
///////////////////////////////////////////////////////////////////////////////
inline GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor
    (const std::string & name, const RelayEDC & events)
        : name_(name), events_(events) {}

inline const std::string & GeneratorNamespaceDescriptor::name() const
    { return name_; }

inline const GeneratorNamespaceDescriptor::EventDescriptorContainer & 
    GeneratorNamespaceDescriptor::events() const
        { return events_; }
///////////////////////////////////////////////////////////////////////////////
inline RelayNamespaceDescriptor::RelayNamespaceDescriptor
    (const std::string & name , const MarshallEDC & events) 
        : name_(name),  events_(events) {}

inline const std::string & RelayNamespaceDescriptor::name() const
    { return name_; }

inline const RelayNamespaceDescriptor::EventDescriptorContainer & 
    RelayNamespaceDescriptor::events() const
        { return events_; }
///////////////////////////////////////////////////////////////////////////////
inline MarshallNamespaceDescriptor::MarshallNamespaceDescriptor(
    const std::string & name, 
    const MarshallEDC & EDC)
        : name_(name), events_(EDC), event(events_) {}

inline MarshallNamespaceDescriptor::operator RelayNamespaceDescriptor() const
{
    return RelayNamespaceDescriptor(name_,events_);
}

inline const std::string & MarshallNamespaceDescriptor::name() const
    { return name_; }

inline const MarshallNamespaceDescriptor::EventDescriptorContainer & 
    MarshallNamespaceDescriptor::events() const
        { return events_; }
///////////////////////////////////////////////////////////////////////////////
inline MarshallNamespaceDescriptorBuilder::MarshallNamespaceDescriptorBuilder
    (const std::string & s) : name_(s), events_()  { }

inline void MarshallNamespaceDescriptorBuilder::AddEventDescriptor
    (const MarshallEventDescriptorBuilder & medb)
{
    EventDescriptorContainer::entry_type entry( 
        medb.oid(), (
        MarshallEventDescriptor) medb);
    
    events_.SetEntry(entry); 
}

inline MarshallNamespaceDescriptorBuilder::operator 
    MarshallNamespaceDescriptor() const
{
    return MarshallNamespaceDescriptor(name_,events_);
}

inline const std::string & MarshallNamespaceDescriptorBuilder::name() const
    { return name_; }

inline const MarshallNamespaceDescriptorBuilder::EventDescriptorContainer & 
    MarshallNamespaceDescriptorBuilder::events() const
        { return events_; }

inline const OP_RESPONSE MarshallNamespaceDescriptorBuilder::ContainsEventIdentifier
    (const Oid & id) const{ return events_.ContainsEither(id); }

///////////////////////////////////////////////////////////////////////////////
