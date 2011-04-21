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
        operator RelayEventDescriptor() const;
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
        operator RelayNamespaceDescriptor() const;
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
    MarshallEventDescriptor::operator RelayEventDescriptor() const
    {
        RelayEventDescriptor red(   event_oid_type_pair_.Id(),
                                    namespace_ordinal_,
                                    event_oid_type_pair_.entry());
        return red;
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
    
    MarshallNamespaceDescriptor::operator RelayNamespaceDescriptor() const
    {        
        RelayNamespaceDescriptor::EventDescriptorContainer redc;
        events_.SlicingPopulate(redc); 
        RelayNamespaceDescriptor casted(name_,ordinal_,redc);
        return casted;
    }
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
}
#endif
