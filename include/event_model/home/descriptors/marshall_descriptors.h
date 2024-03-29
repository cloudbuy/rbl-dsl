#ifndef RBL_EM_HOME_MARSHALL_DESCRIPTORS_H
#define RBL_EM_HOME_MARSHALL_DESCRIPTORS_H
#include "descriptors_common.h"
#include "relay_descriptors.h"
#include <boost/shared_ptr.hpp>

namespace rubble { namespace event_model { 
//---------------------------------------------------------------------------//
// Class Declarations                                                        //
//---------------------------------------------------------------------------//
    // MarshallEventDescriptor ////////////////////////////////////////////////
    class MarshallEventDescriptor : public RelayEventDescriptor    
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
        typedef  boost::shared_ptr<MarshallNamespaceDescriptor> t_shp;
        // when a mnd is created, all of its state needs to be present.
        // therefore the following constructor should be removed.`
        // TODO remove following two contructors
        MarshallNamespaceDescriptor();
        MarshallNamespaceDescriptor( const  std::string & name_in,
                                     const  ordinal_type ordinal_in);
        MarshallNamespaceDescriptor( const  std::string & name_in,
                                     const  ordinal_type ordinal_in,
                                     const t_edc & edc);
        operator RelayNamespaceDescriptor() const;
    };
    //-----------------------------------------------------------------------// 

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
    // MarshallEvenDescriptor /////////////////////////////////////////////////
    inline MarshallEventDescriptor::MarshallEventDescriptor()
        :  RelayEventDescriptor()
    {
    }
    inline MarshallEventDescriptor::MarshallEventDescriptor
    (const Oid & oid, const ordinal_type ordinal_, const EventTypeContainer & edc)
        : RelayEventDescriptor(oid,ordinal_,edc)
    {
    }
    //-----------------------------------------------------------------------//

    // MarshallNamespaceDescriptor ////////////////////////////////////////////
    inline MarshallNamespaceDescriptor::MarshallNamespaceDescriptor()
        : NamespaceDescriptorBase<MarshallEventDescriptor>()
    {
    }
    inline MarshallNamespaceDescriptor::MarshallNamespaceDescriptor
        ( const std::string & name_in,const ordinal_type ordinal_in)
        : NamespaceDescriptorBase<MarshallEventDescriptor>(name_in, ordinal_in)
    {
    }
    inline MarshallNamespaceDescriptor::MarshallNamespaceDescriptor
        ( const std::string & name_in,const ordinal_type ordinal_in, const t_edc & edc)
        : NamespaceDescriptorBase<MarshallEventDescriptor>(name_in, ordinal_in, edc)
    {
    }
    
    inline MarshallNamespaceDescriptor::operator RelayNamespaceDescriptor() const
    {        
        RelayNamespaceDescriptor::t_edc redc;
        m_events.SlicingPopulate(redc); 
        RelayNamespaceDescriptor casted(name(),ordinal(),redc);
        return casted;
    }
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
} }
#endif
