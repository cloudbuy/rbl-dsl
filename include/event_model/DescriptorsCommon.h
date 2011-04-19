#ifndef _EM_DESCRIPTORS_COMMON_H
#define _EM_DESCRIPTORS_COMMON_H
#include <event_model/Types.h>
#include <boost/cstdint.hpp>
#include "detail/OidContainer.h"

namespace event_model
{
    using namespace primitives;
    
    typedef boost::uint8_t ordinal_type; 
    // Common 
    typedef OidConstrainedString<char, 32> OidName;
    typedef OidType<OidName, ordinal_type> Oid;
    
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
    //TODO I have to get rid of size_t and replace it with 2^8-1 limit
    //respecting operations and types, this involves changing stuff
    //in OidContainer.h and the unit tests.
    class EventDescriptorBase
    {
    public:
        EventDescriptorBase( Oid & oid, 
                             ordinal_type ordinal_, EventTypecontainer & etc);
        const Oid & oid() const;
        const ordinal_type namespace_ordinal() const;

        const std::size_t size() const ;
        const std::size_t occupied_size() const;
    protected:
        EventDescriptorPair oid_type_pair_;
        ordinal_type namespace_ordinal_;
    };
    
    template<class EventDescriptorContainer>
    class NamespaceDescriptorBase 
    {
        const std::string & name();
        const ordinal_type ordinal();
    private:
        std::string name_;
        ordinal_type ordinal_;
        EventDescriptorContainer EDC;
    };
// inline definitions /////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    inline const Oid & EventDescriptorBase::oid() const
    {
        return oid_type_pair_.Id();
    }
    inline const std::size_t EventDescriptorBase::size() const
    {
        return oid_type_pair_.entry().size();        
    }
    inline const std::size_t EventDescriptorBase::occupied_size() const
    {
        return oid_type_pair_.entry().occupied_size();
    }
    ///////////////////////////////////////////////////////////////////////////

}
#endif 

