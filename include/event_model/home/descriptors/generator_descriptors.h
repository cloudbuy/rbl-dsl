#ifndef RBL_EM_HOME_GENERATOR_DESCRIPTORS_H
#define RBL_EM_HOME_GENERATOR_DESCRIPTORS_H
#include "descriptors_common.h"

//TODO centralize the SF forward declarations
namespace SF
{
    class Archive;
}

namespace rubble { namespace event_model { 
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
                                        const   t_edc &);
        friend void serialize(  SF::Archive & ar,
                                GeneratorNamespaceDescriptor & gnd );
    };
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
    // GenereatorEventDescriptor //////////////////////////////////////////////
    inline GeneratorEventDescriptor::GeneratorEventDescriptor()
    {
    }
    inline GeneratorEventDescriptor::GeneratorEventDescriptor(   
                                    const  Oid & oid,
                                    const  ordinal_type ordinal_in,
                                    const  EventTypeContainer & edc_in)
        : EventDescriptorBase(oid,ordinal_in,edc_in)
    {
    }
    //-----------------------------------------------------------------------//
    
    // GeneratorNamespaceDescriptor ///////////////////////////////////////////
    inline GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor()
        : NamespaceDescriptorBase<GeneratorEventDescriptor>()
    {
        
    }
    inline GeneratorNamespaceDescriptor::GeneratorNamespaceDescriptor
        (   const  std::string & name_in, 
            const  ordinal_type ordinal_in, 
            const t_edc & edc_in)
        : NamespaceDescriptorBase<GeneratorEventDescriptor>
            (name_in,ordinal_in,edc_in)
    {
    }
    
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
} }
#endif
