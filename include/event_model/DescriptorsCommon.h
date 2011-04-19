#ifndef _EM_DESCRIPTORS_COMMON_H
#define _EM_DESCRIPTORS_COMMON_H
#include <event_model/Types.h>
#include <boost/cstdint.hpp>
#include "detail/OidContainer.h"

namespace event_model
{
    using namespace primitives;
   
    // Common 
    typedef OidConstrainedString<char, 32> OidName;
    typedef OidType<OidName, boost::uint8_t> Oid;
    
    enum EVENT_DESCRIPTOR_QUALIFIER
    {
        ENTRY_UNINITIALIZED = 0,
        ENTRY_REQUIRED,
        ENTRY_OPTIONAL,
    };
    
    class EventTypeDescriptor
    {
    public:
        
        EventTypeDescriptor();
        explicit EventTypeDescriptor(   EVENT_DESCRIPTOR_QUALIFIER _qualifier,
                                        VALUE_TYPE _type,
                                        bool primitive_in);
        
        // no operator= or copy constructor as bitwise copy is fine

        const bool is_primitive() const;
        const EVENT_DESCRIPTOR_QUALIFIER qualifier() const;    
        const VALUE_TYPE type() const;
        
        void set_is_primitive(bool _is_primitive);
        void set_qualifier(EVENT_DESCRIPTOR_QUALIFIER _qualifier);
        void set_type(VALUE_TYPE _type);
 
        void serialize(SF::Archive & ar);
    private:
        EVENT_DESCRIPTOR_QUALIFIER qualifier_;
        VALUE_TYPE  type_;
        bool primitive_;
    };
    
    typedef OidContainer<Oid, EventTypeDescriptor>  EventTypeContainer;
    
    typedef OidContainerEntryType<Oid,EventTypeContainer> 
                                                    EventDescriptorPair;
    class EventDescriptorBase
    {
        EventDescriptorPair oid_type_pair;
        
        EventTypeContainer & types();
        Oid & oid();
    };
}
#endif 

