#ifndef _GENERATOR_DESCRIPTORS_H
#define _GENERATOR_DESCRIPTORS_H
#include "event_model/DescriptorsCommon.h"

namespace event_model
{
    // GeneratorEventDescriptor ///////////////////////////////////////////////
    class GeneratorEventDescriptor : public EventDescriptorBase
    {
    };
    //-----------------------------------------------------------------------//
 
    // GeneratorNamespaceDescriptor ///////////////////////////////////////////   
    class GeneratorNamespaceDescriptor
        : public  NamespaceDescriptorBase<GeneratorEventDescriptor>
    {
    };
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
}
#endif
