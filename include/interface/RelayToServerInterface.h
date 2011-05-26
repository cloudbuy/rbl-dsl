#ifndef _RCF_INTERFACE_RELAY_TO_SERVER
#define _RCF_INTERFACE_RELAY_TO_SERVER

#include <RCF/Idl.hpp>
#include <relay/Descriptors-Serialization.h>

using rubble::event_model::descriptors::RelayNamespaceDescriptor;

RCF_BEGIN(I_NamespaceRepository, "I_NamespaceRepository")
    RCF_METHOD_R1(RelayNamespaceDescriptor, GetRelayNamespaceDescriptor, std::string)
RCF_END(I_NamespaceRepository)

#endif
