#ifndef RBL_EM_HOME_GENERATOR_DESCRIPTORS_SERIALIZATION_H
#define RBL_EM_HOME_GENERATOR_DESCRIPTORS_SERIALIZATION_H
#include "generator_descriptors.h"
#include "descriptors_common-serialization.h" 
#include <SF/Serializer.hpp>

namespace rubble { namespace event_model {
  inline void serialize(SF::Archive & ar,GeneratorEventDescriptor & ged) 
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
    ar & ged.m_event_oid_type_pair & ged.m_namespace_ordinal;
  }
  inline void serialize(SF::Archive & ar, GeneratorNamespaceDescriptor & gnd)
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
    
    ar & gnd.m_oid & gnd.m_events;
  }
 
} }
#endif
