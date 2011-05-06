#ifndef _EM_DESCRIPTORS_COMMON_SERIALIZATION_H
#define _EM_DESCRIPTORS_COMMON_SERIALIZATION_H
#include <event_model/DescriptorsCommon.h>
#include <event_model/detail/OidContainer-Serialization.h>
#include <SF/Serializer.hpp>

namespace event_model
{

inline void serialize(SF::Archive & ar, EventTypeDescriptor & etd)
{
  ar & etd.qualifier_ & etd.type_ & etd.primitive_;
}

inline void serialize(SF::Archive & ar, EventDescriptorBase & edb)
{ 
  ar & edb.event_oid_type_pair_ & edb.namespace_ordinal_;
}

template<typename NDBType>
inline void serialize(SF::Archive & ar, NamespaceDescriptorBase<NDBType> & ndb)
{
  ar & ndb.name_ & ndb.ordinal_ & ndb.events_;
}

}// namespace event_model
#endif
