#ifndef _EM_DESCRIPTORS_COMMON_SERIALIZATION_H
#define _EM_DESCRIPTORS_COMMON_SERIALIZATION_H
#include <event_model/DescriptorsCommon.h>
#include <event_model/detail/OidContainer-Serialization.h>
#include <SF/Serializer.hpp>

namespace rubble { namespace event_model { namespace descriptors {

inline void serialize(SF::Archive & ar, EventTypeDescriptor & etd)
{
  using namespace rubble::event_model::types;
  rbl_types::ordinal_type ord;
  
  if(ar.isWrite()) {
    ord =  (rbl_types::ordinal_type) etd.m_type.which();
    ar & etd.m_qualifier & ord & etd.m_primitive;
  }
  else 
  {
    ar & etd.m_qualifier & ord & etd.m_primitive;
    etd.m_type = rbl_types::type_ordinal_map[ord];
  }
}

inline void serialize(SF::Archive & ar, EventDescriptorBase & edb)
{ 
  ar & edb.m_event_oid_type_pair & edb.m_namespace_ordinal;
}

template<typename NDBType>
inline void serialize(SF::Archive & ar, NamespaceDescriptorBase<NDBType> & ndb)
{
  ar & ndb.m_name & ndb.m_ordinal & ndb.m_events;
}

} } } 
#endif
