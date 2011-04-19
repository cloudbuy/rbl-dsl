#ifndef _GENERATOR_DESCRIPTORS_H
#define _GENERATOR_DESCRIPTORS_H
#include "event_model/DescriptorsCommon.h"

namespace event_model
{
    class GeneratorEventDescriptor : public EventDescriptorBase
    {
    };
    typedef OidContainer<Oid,GeneratorEventDescriptor>  GeneratorEDC;

    class GeneratorNamespaceDescriptor
    {
    };
}
#endif
