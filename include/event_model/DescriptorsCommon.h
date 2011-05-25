#ifndef _EM_DESCRIPTORS_COMMON_H
#define _EM_DESCRIPTORS_COMMON_H
#include <event_model/types2/rbl_types.h>
#include <boost/cstdint.hpp>
#include "detail/OidContainer.h"

namespace SF
{
  class Archive;
}

namespace rubble { namespace event_model { namespace descriptors {
//---------------------------------------------------------------------------// 
// Class Declarations                                                        //
//---------------------------------------------------------------------------//
using namespace primitives;
namespace rbl_types=rubble::event_model::types;

typedef boost::uint16_t ordinal_type; 
typedef OidConstrainedString<char, 32> OidName;
typedef OidType<OidName, ordinal_type> Oid;
//TODO try to nest this in EventTypeDescriptor
enum EVENT_DESCRIPTOR_QUALIFIER
{
  ENTRY_UNINITIALIZED = 0,
  ENTRY_REQUIRED,
  ENTRY_OPTIONAL,
};

// EventTypeDescriptor ////////////////////////////////////////////////////////
class EventTypeDescriptor
{
public:
  EventTypeDescriptor();
  template<typename T>
  explicit EventTypeDescriptor(   EVENT_DESCRIPTOR_QUALIFIER _qualifier,
                                  T,
                                  bool primitive_in);
  
  // no operator= or copy constructor as bitwise copy is fine

  const bool                              is_primitive()        const;
  const EVENT_DESCRIPTOR_QUALIFIER        qualifier()           const;    
  const rbl_types::ordinal_type           type()                const;
  
  void set_is_primitive(bool _is_primitive);
  void set_qualifier(EVENT_DESCRIPTOR_QUALIFIER _qualifier);
  template<typename T> void set_type(T);
  void set_type_using_ordinal(const rbl_types::ordinal_type ordinal);
private:

  friend void serialize(SF::Archive & ar, EventTypeDescriptor & etd);
  
  EVENT_DESCRIPTOR_QUALIFIER        m_qualifier;
  rbl_types::ordinal_type  m_type;
  bool m_primitive;
};

//---------------------------------------------------------------------------//

typedef OidContainer<Oid, EventTypeDescriptor>  EventTypeContainer;
typedef OidContainerEntryType<Oid,EventTypeContainer> EventDescriptorPair;

// EventDescriptorBase ////////////////////////////////////////////////////////
    
//TODO I have to get rid of size_t and replace it with 2^8-1 limit
//respecting operations and types, this involves changing stuff
//in OidContainer.h and the unit tests.
class EventDescriptorBase
{
public:
  EventDescriptorBase();
  EventDescriptorBase(    const Oid & oid, 
                          const ordinal_type ordinal_,  
                          const EventTypeContainer & etc);
   
  const ordinal_type            namespace_ordinal()                   const;
  const ordinal_type            ordinal()                             const;
  const OidName                 name()                                const;
  const Oid &                   oid()                                 const;
  
  const Oid *                   TypeOidAt(const ordinal_type ordinal) const;
  const Oid *                   TypeOidWithName(const OidName & name) const;

  const EventTypeDescriptor *   TypeAt(const ordinal_type ordinal)    const;
  const rbl_types::ordinal_type TypeValueTypeAt(const ordinal_type )  const;
  const std::size_t             type_container_size()                 const;
  const std::size_t             type_container_occupied_size()        const;
protected:
  friend void serialize(SF::Archive & ar, EventDescriptorBase & edb);
  
  EventDescriptorPair m_event_oid_type_pair;
  ordinal_type m_namespace_ordinal;
};
//---------------------------------------------------------------------------//
    
// NamespaceDescriptorBase ////////////////////////////////////////////////////
template<class EventDescriptor_>
class NamespaceDescriptorBase 
{
public:
  typedef EventDescriptor_ EventDescriptor;
  typedef OidContainer<Oid,EventDescriptor> t_edc;

  NamespaceDescriptorBase();
  NamespaceDescriptorBase(  const std::string & name_in,
                            const ordinal_type ordinal);
  NamespaceDescriptorBase(  const std::string & name_in,
                            const ordinal_type ordinal,
                            const t_edc & con);

  const std::size_t       event_container_size()                        const;
  const std::size_t       event_container_occupied_size()               const;
  const OP_RESPONSE       ContainsEventIdentifier(const Oid & oid)      const;
  const EventDescriptor * EventAt(const ordinal_type ordinal)           const;
  const EventDescriptor * EventWithName(const OidName & name_in)        const;
  const t_edc &           events()                                      const;

  const std::string & name();
  const ordinal_type ordinal();
  
protected:
  template<typename NDBType>
  friend void serialize(SF::Archive & ar, NamespaceDescriptorBase<NDBType> & ndb);

  std::string m_name;
  ordinal_type m_ordinal;
  t_edc m_events;
};
//---------------------------------------------------------------------------//
} } }
#include "detail/DescriptorsCommon-inl.h"
#endif 

