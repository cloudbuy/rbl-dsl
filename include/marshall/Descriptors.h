#ifndef _MARSHALL_DESCRIPTORS_H
#define _MARSHALL_DESCRIPTORS_H
#include "relay/Descriptors.h"

namespace event_model
{
    class MarshallEventDescriptor : public EventDescriptorBase
    {
    };
    typedef OidContainer<Oid,MarshallEventDescriptor>   MarshallEDC;
    
    class MarshallNamespaceDescriptor
    {
    };
}
#endif
