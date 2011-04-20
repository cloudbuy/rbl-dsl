#ifndef _RELAY_DESCRIPTORS_H
#define _RELAY_DESCRIPTORS_H
#include "event_model/DescriptorsCommon.h"
#include "generator/Descriptors.h"

namespace event_model
{
//---------------------------------------------------------------------------//
// Class Declarations                                                        //
//---------------------------------------------------------------------------//
    // RelayEventDescriptor ///////////////////////////////////////////////////
    class RelayEventDescriptor : public EventDescriptorBase
    {
    };
    //-----------------------------------------------------------------------//
    
    // RelayNamespaceDescriptor ///////////////////////////////////////////////
    class RelayNamespaceDescriptor : 
        public  NamespaceDescriptorBase<RelayEventDescriptor>
    {
    };
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
}
#endif
