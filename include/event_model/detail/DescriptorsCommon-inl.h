#ifndef _EM_DESCRIPTORS_INL_COMMON_H
#define _EM_DESCRIPTORS_INL_COMMON_H
#include <event_model/DescriptorsCommon.h>
namespace event_model
{
using namespace primitives;
// EventTypeDescriptor ////////////////////////////////////////////////////////
inline EventTypeDescriptor::EventTypeDescriptor()
: qualifier_(ENTRY_UNINITIALIZED), 
  type_(VALUE_UNINITIALIZED), 
  primitive_(false) {} 

inline EventTypeDescriptor::EventTypeDescriptor( 
  EVENT_DESCRIPTOR_QUALIFIER _qualifier, 
  VALUE_TYPE _type,
  bool primitive_in)
  : qualifier_(_qualifier),
    type_(_type),
    primitive_(primitive_in) {}

inline const bool 
EventTypeDescriptor::is_primitive() const {return primitive_; } 

inline const EVENT_DESCRIPTOR_QUALIFIER 
EventTypeDescriptor::qualifier() const
  { return qualifier_; }

inline const VALUE_TYPE 
EventTypeDescriptor::type() const { return type_; }     

inline void EventTypeDescriptor::set_is_primitive(bool _is_primitive)
{
  primitive_ = _is_primitive;
}
inline void EventTypeDescriptor::set_qualifier
  (EVENT_DESCRIPTOR_QUALIFIER _qualifier)
{
  qualifier_ = _qualifier;
}
inline void EventTypeDescriptor::set_type(VALUE_TYPE _type)
{
  type_ = _type;
}
//---------------------------------------------------------------------------//

// EventDescriptorBase ////////////////////////////////////////////////////////
inline EventDescriptorBase::EventDescriptorBase() 
  : namespace_ordinal_(), event_oid_type_pair_()
{
}
inline EventDescriptorBase::EventDescriptorBase
  ( const Oid & oid,
    const ordinal_type ordinal_,
    const EventTypeContainer & etc)
      : event_oid_type_pair_(oid,etc), 
        namespace_ordinal_(ordinal_) {}

inline const std::size_t EventDescriptorBase::type_container_size() const
{
  return event_oid_type_pair_.entry().size();        
}
inline const ordinal_type EventDescriptorBase::ordinal() const
{
  return event_oid_type_pair_.Id().ordinal();
}
inline const OidName EventDescriptorBase::name() const
{
  return event_oid_type_pair_.Id().name();
}

inline const Oid & EventDescriptorBase::oid() const
{
  return event_oid_type_pair_.Id();
}

inline const Oid * EventDescriptorBase::TypeOidAt(const ordinal_type ordinal) const
{
  const EventTypeContainer::entry_type * et = 
    event_oid_type_pair_.entry().EntryAtordinal(ordinal);
  
  if(et != NULL)
    return & et->Id();
  else
    return NULL;
}

inline const Oid * EventDescriptorBase::TypeOidWithName(const OidName & name) const
{
  const EventTypeContainer::entry_type * et = 
    event_oid_type_pair_.entry().EntryWithName(name);
  
  if(et != NULL)
    return & et->Id();
  else
    return NULL;

}

const EventTypeDescriptor * EventDescriptorBase::
TypeAt(const ordinal_type ordinal) const
{
  return event_oid_type_pair_.entry()[ordinal];
}

inline const std::size_t EventDescriptorBase::
type_container_occupied_size() const
{
  return event_oid_type_pair_.entry().occupied_size();
} 
//-----------------------------------------------------------------------//
  
// NamespaceDescriptorBase ////////////////////////////////////////////////
template<class EventDescriptor>
inline NamespaceDescriptorBase<EventDescriptor>
::NamespaceDescriptorBase()
{

}

template<class EventDescriptor> 
inline NamespaceDescriptorBase<EventDescriptor>::
NamespaceDescriptorBase(  const std::string & name_in,
                          const ordinal_type ordinal)
  : name_(name_in), ordinal_(ordinal), events_()
{
}

template<class EventDescriptor> 
inline NamespaceDescriptorBase<EventDescriptor>::
NamespaceDescriptorBase(  const std::string & name_in,
                          const ordinal_type ordinal,
                          const EventDescriptorContainer & con)
  : name_(name_in), ordinal_(ordinal), events_(con)
{
}

template<class EventDescriptor>
inline const std::string & NamespaceDescriptorBase<EventDescriptor>
::name()
{
  return name_;
}
template<class EventDescriptor>
inline const ordinal_type NamespaceDescriptorBase<EventDescriptor>
::ordinal()
{
  return ordinal_;
}

template<class EventDescriptor>
inline const std::size_t NamespaceDescriptorBase<EventDescriptor>::
event_container_size() const
{
  return events_.size();
}
template<class EventDescriptor>
inline const std::size_t NamespaceDescriptorBase<EventDescriptor>::
event_container_occupied_size() const
{
  return  events_.occupied_size();    
}
template<class EventDescriptor>
inline const OP_RESPONSE NamespaceDescriptorBase<EventDescriptor>::
ContainsEventIdentifier(const Oid & oid) const
{
  return events_.ContainsEither(oid); 
}

template<class EventDescriptor>
inline const EventDescriptor *
NamespaceDescriptorBase<EventDescriptor>::
EventAt(const ordinal_type ordinal) const
{
  return events_[ordinal];        
}
template<class EventDescriptor>
inline const EventDescriptor *
NamespaceDescriptorBase<EventDescriptor>::
EventWithName(const OidName & name_in) const
{
  return events_[name_in];
}
//---------------------------------------------------------------------------//
}
#endif
