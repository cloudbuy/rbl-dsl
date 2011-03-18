#ifndef _EM_COMMON_DESCRIPTORS_H
#define _EM_COMMON_DESCRIPTORS_H
#include <boost/cstdint.hpp>
#include <RCF/Idl.hpp>
#include <SF/Archive.hpp>

#include "OidContainer.h"


namespace event_model
{
    using namespace primitives;
    
    typedef OidConstrainedString<char, 32> OidName;
    typedef OidType<OidName, boost::uint8_t> Oid;
    
    enum VALUE_TYPE
    {
        VALUE_INT4 = 0,
        VALUE_INT8,
        VALUE_STRING
    };

    enum EVENT_DESCRIPTOR_QUALIFIER
    {
        ENTRY_REQUIRED = 0,
        ENTRY_OPTIONAL,
    };
    
    class EventTypeDescriptor
    {
    public:
        EVENT_DESCRIPTOR_QUALIFIER qualifier;
        VALUE_TYPE  type;

        EventTypeDescriptor();

        EventTypeDescriptor( EVENT_DESCRIPTOR_QUALIFIER _qualifier,
                             VALUE_TYPE _type );

        void serialize(SF::Archive & ar);
    private:
        bool primitive_;
    };


    typedef OidContainer<Oid,EventTypeDescriptor> EventTypeContainer;
}

#endif
