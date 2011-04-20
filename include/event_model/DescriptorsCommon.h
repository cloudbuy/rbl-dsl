#ifndef _EM_DESCRIPTORS_COMMON_H
#define _EM_DESCRIPTORS_COMMON_H
#include <event_model/Types.h>
#include <boost/cstdint.hpp>
#include "detail/OidContainer.h"

namespace event_model
{
    using namespace primitives;
    
    typedef boost::uint16_t ordinal_type; 
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
                             ordinal_type ordinal_, EventTypeContainer & etc);
         
        const ordinal_type namespace_ordinal() const;

        const std::size_t event_container_size() const ;
        const std::size_t event_container_occupied_size() const;
    protected:
        EventDescriptorPair event_oid_type_pair_;
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
//---------------------------------------------------------------------------//
// inline definitions                                                        //
//---------------------------------------------------------------------------//

    // EventTypeDescriptor ////////////////////////////////////////////////////
    inline EventTypeDescriptor::EventTypeDescriptor()
    :   qualifier_(ENTRY_UNINITIALIZED), 
        type_(VALUE_UNINITIALIZED), 
        primitive_(false) {} 

    inline EventTypeDescriptor::EventTypeDescriptor( 
        EVENT_DESCRIPTOR_QUALIFIER _qualifier, 
        VALUE_TYPE _type,
        bool primitive_in)
        : qualifier_(_qualifier),
          type_(_type),
          primitive_(primitive_in) {}

    inline const bool 
    EventTypeDescriptor::is_primitive() const {return primitive_; } 

    inline const EVENT_DESCRIPTOR_QUALIFIER 
    EventTypeDescriptor::qualifier() const
        { return qualifier_; }

    inline const VALUE_TYPE 
    EventTypeDescriptor::type() const { return type_; }     

    inline void EventTypeDescriptor::set_is_primitive(bool _is_primitive)
    {
        primitive_ = _is_primitive;
    }
    inline void EventTypeDescriptor::set_qualifier
        (EVENT_DESCRIPTOR_QUALIFIER _qualifier)
    {
        qualifier_ = _qualifier;
    }
    inline void EventTypeDescriptor::set_type(VALUE_TYPE _type)
    {
        type_ = _type;
    }

    inline void EventTypeDescriptor::serialize(SF::Archive & ar)
    {
        ar & qualifier_ & type_ & primitive_;
    }
    //-----------------------------------------------------------------------//

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

