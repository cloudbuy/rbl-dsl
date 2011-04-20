#ifndef _MARSHALL_DESCRIPTORS_H
#define _MARSHALL_DESCRIPTORS_H
#include "relay/Descriptors.h"

namespace event_model
{
//---------------------------------------------------------------------------//
// Class Declarations                                                        //
//---------------------------------------------------------------------------//
    // MarshallEventDescriptor ////////////////////////////////////////////////
    class MarshallEventDescriptor : public EventDescriptorBase
    {
    public:
        MarshallEventDescriptor(); 
        MarshallEventDescriptor( const Oid & oid, const ordinal_type ordinal_, 
                                 const EventTypeContainer & etc);
    };
    //-----------------------------------------------------------------------//
    
    // MarshallNamespaceDescriptor ////////////////////////////////////////////
    class MarshallNamespaceDescriptor : 
        public NamespaceDescriptorBase<MarshallEventDescriptor>
    {
    public:
        MarshallNamespaceDescriptor();
        MarshallNamespaceDescriptor( const  std::string & name_in,
                                     const  ordinal_type ordinal_in);
    };
    //-----------------------------------------------------------------------// 

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
    // MarshallEvenDescriptor /////////////////////////////////////////////////
    inline MarshallEventDescriptor::MarshallEventDescriptor()
        :  EventDescriptorBase()
    {
    }
    inline MarshallEventDescriptor::MarshallEventDescriptor
    (const Oid & oid, const ordinal_type ordinal_, const EventTypeContainer & etc)
        : EventDescriptorBase(oid,ordinal_,etc)
    {
    }
    //-----------------------------------------------------------------------//

    // MarshallNamespaceDescriptor ////////////////////////////////////////////
    MarshallNamespaceDescriptor::MarshallNamespaceDescriptor()
        : NamespaceDescriptorBase<MarshallEventDescriptor>()
    {
    }
    MarshallNamespaceDescriptor::MarshallNamespaceDescriptor
        ( const std::string & name_in,const ordinal_type ordinal_in)
        : NamespaceDescriptorBase<MarshallEventDescriptor>(name_in, ordinal_in)
    {
    }
    //-----------------------------------------------------------------------//

}
#endif
