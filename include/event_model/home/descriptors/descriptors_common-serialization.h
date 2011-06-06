#ifndef RBL_EM_HOME_DESCRIPTORS_COMMON_SERIALIZATION_H
#define RBL_EM_HOME_DESCRIPTORS_COMMON_SERIALIZATION_H
#include <SF/Serializer.hpp>

namespace rubble { namespace event_model {
 
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
  ar & ndb.m_oid & ndb.m_events;
}

} }
#endif
