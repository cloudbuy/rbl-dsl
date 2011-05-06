#ifndef _GENERATOR_DESCRIPTORS_H
#define _GENERATOR_DESCRIPTORS_H
#include <event_model/DescriptorsCommon.h>

//TODO centralize the SF forward declarations
namespace SF
{
    class Archive;
}

namespace event_model
{
    //---------------------------------------------------------------------------//
// Class Declarations                                                        // 
//---------------------------------------------------------------------------//
    // GeneratorEventDescriptor ///////////////////////////////////////////////
    class GeneratorEventDescriptor : public EventDescriptorBase
    {
    public:
        GeneratorEventDescriptor();
        GeneratorEventDescriptor(   const  Oid & oid,
                                    const  ordinal_type ordinal_in,
                                    const  EventTypeContainer & edc_in);
        friend void serialize(  SF::Archive & ar, 
                                GeneratorEventDescriptor & ged);
    };
    //-----------------------------------------------------------------------//
 
    // GeneratorNamespaceDescriptor ///////////////////////////////////////////   
    class GeneratorNamespaceDescriptor
        : public  NamespaceDescriptorBase<GeneratorEventDescriptor>
    {
    public:
        GeneratorNamespaceDescriptor();
        GeneratorNamespaceDescriptor(   const   std::string & name_in,
                                        const   ordinal_type ordinal_in,
                                        const   EventDescriptorContainer &);
        friend void serialize(  SF::Archive & ar,
                                GeneratorNamespaceDescriptor & gnd );
    };
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
    // GenereatorEventDescriptor //////////////////////////////////////////////
    GeneratorEventDescriptor::GeneratorEventDescriptor()
    {
    }
    GeneratorEventDescriptor::GeneratorEventDescriptor(   
                                    const  Oid & oid,
                                    const  ordinal_type ordinal_in,
                                    const  EventTypeContainer & edc_in)
        : EventDescriptorBase(oid,ordinal_in,edc_in)
    {
    }
    //-----------------------------------------------------------------------//
    
    // GeneratorNamespaceDescriptor ///////////////////////////////////////////
    GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor()
        : NamespaceDescriptorBase<GeneratorEventDescriptor>()
    {
        
    }
    GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor
        (   const  std::string & name_in, 
            const  ordinal_type ordinal_in, 
            const EventDescriptorContainer & edc_in)
        : NamespaceDescriptorBase<GeneratorEventDescriptor>
            (name_in,ordinal_in,edc_in)
    {
    }
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
}
#endif
