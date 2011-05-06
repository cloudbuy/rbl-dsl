#ifndef _RELAY_DESCRIPTORS_SERIALIZATION_H
#define _RELAY_DESCRIPTORS_SERIALIZATION_H
#include <relay/Descriptors.h>
#include <event_model/detail/DescriptorsCommon-Serialization.h>
#include <SF/Serializer.hpp>

namespace event_model
{
  void serialize(SF::Archive & ar,RelayEventDescriptor & red) 
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
    ar & red.event_oid_type_pair_ & red.namespace_ordinal_;
  }
  void serialize(SF::Archive & ar, RelayNamespaceDescriptor & rnd)
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
    
    ar & rnd.name_ & rnd.ordinal_ & rnd.events_;
  }
}
#endif 
