#ifndef _EM_DESCRIPTORS_COMMON_SERIALIZATION_H
#define _EM_DESCRIPTORS_COMMON_SERIALIZATION_H
#include <event_model/DescriptorsCommon.h>
#include <event_model/detail/OidContainer-Serialization.h>
#include <SF/Serializer.hpp>

namespace event_model
{

inline void serialize(SF::Archive & ar, EventTypeDescriptor & etd)
{
  ar & etd.m_qualifier & etd.m_type & etd.m_primitive;
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

}// namespace event_model
#endif
