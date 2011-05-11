#ifndef _RELAY_DESCRIPTORS_SERIALIZATION_H
#define _RELAY_DESCRIPTORS_SERIALIZATION_H
#include <relay/Descriptors.h>
#include <event_model/detail/DescriptorsCommon-Serialization.h>
#include <SF/Serializer.hpp>

namespace event_model
{
  inline void serialize(SF::Archive & ar,RelayEventDescriptor & red) 
  {
    #if 0
    if(!ar.isWrite())
    {
      RelayEventDescriptor red2;
      ar & red2;
    }
    else 
      ar & static_cast<EventDescriptorBase>(red); 
    #endif
    ar & red.m_event_oid_type_pair & red.m_namespace_ordinal;
  }
  inline void serialize(SF::Archive & ar, RelayNamespaceDescriptor & rnd)
  {
    #if 0
    if(!ar.isWrite())
    {
      RelayNamespaceDescriptor rnd2;
      ar & rnd2;
      std::cout << "size: " << rnd2.event_container_size() << std::endl;
    }
    else
      ar & static_cast<NamespaceDescriptorBase<RelayEventDescriptor> >(rnd);
    #endif
    
    ar & rnd.m_name & rnd.m_ordinal & rnd.m_events;
  }
}
#endif 
