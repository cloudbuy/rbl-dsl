#ifndef _EM_DESCRIPTORS_COMMON_H
#define _EM_DESCRIPTORS_COMMON_H
#include <event_model/Types.h>
#include <boost/cstdint.hpp>
#include "detail/OidContainer.h"

namespace event_model
{
    using namespace primitives;
    
    typedef boost::uint16_t ordinal_type; 
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
        EventDescriptorBase();
        EventDescriptorBase(    const Oid & oid, 
                                const ordinal_type ordinal_, 
                                const EventTypeContainer & etc);
         
        const ordinal_type namespace_ordinal() const;
        const ordinal_type ordinal() const;
        const OidName name() const;
        const Oid & oid() const;

        const EventTypeDescriptor * TypeAt(const ordinal_type ordinal) const;
        const std::size_t type_container_size() const ;
        const std::size_t type_container_occupied_size() const;
    protected:
        EventDescriptorPair event_oid_type_pair_;
        ordinal_type namespace_ordinal_;
    };
    
    template<class EventDescriptor_>
    class NamespaceDescriptorBase 
    {
    public:
        typedef EventDescriptor_ EventDescriptor;
        typedef OidContainer<Oid,EventDescriptor> EventDescriptorContainer;

        NamespaceDescriptorBase();
        NamespaceDescriptorBase( const std::string & name_in,
                                 const ordinal_type ordinal);

        const std::string & name();
        const ordinal_type ordinal();
        const std::size_t event_container_size() const;
        const std::size_t event_container_occupied_size() const;
        const OP_RESPONSE ContainsEventIdentifier(const Oid & oid) const;
        const EventDescriptor * 
        EventAt(const ordinal_type ordinal) const;
        const EventDescriptor * EventWithName
            (const OidName & name_in) const;
    protected:
        std::string name_;
        ordinal_type ordinal_;
        EventDescriptorContainer events_;
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
    inline EventDescriptorBase::EventDescriptorBase() 
        : namespace_ordinal_(), event_oid_type_pair_()
    {
    }
    inline EventDescriptorBase::EventDescriptorBase
    (   const Oid & oid, const ordinal_type ordinal_, 
        const EventTypeContainer & etc)
            : event_oid_type_pair_(oid,etc), 
              namespace_ordinal_(ordinal_) 
    {
    }

    inline const std::size_t EventDescriptorBase::type_container_size() const
    {
        return event_oid_type_pair_.entry().size();        
    }
    inline const ordinal_type EventDescriptorBase::ordinal() const
    {
        return event_oid_type_pair_.Id().ordinal();
    }
    inline const OidName EventDescriptorBase::name() const
    {
        return event_oid_type_pair_.Id().name();
    }

    inline const Oid & EventDescriptorBase::oid() const
    {
        return event_oid_type_pair_.Id();
    }
 
    const EventTypeDescriptor * EventDescriptorBase::
    TypeAt(const ordinal_type ordinal) const
    {
        return event_oid_type_pair_.entry()[ordinal];
    }

    inline const std::size_t EventDescriptorBase::
    type_container_occupied_size() const
    {
        return event_oid_type_pair_.entry().occupied_size();
    } 
    //-----------------------------------------------------------------------//
    

    // NamespaceDescriptorBase ////////////////////////////////////////////////
    template<class EventDescriptor>
    inline NamespaceDescriptorBase<EventDescriptor>
    ::NamespaceDescriptorBase()
    {

    }
    template<class EventDescriptor> 
    inline NamespaceDescriptorBase<EventDescriptor>::
    NamespaceDescriptorBase( const std::string & name_in,
                             const ordinal_type ordinal)
        : name_(name_in), ordinal_(ordinal)
    {
    }

    template<class EventDescriptor>
    inline const std::string & NamespaceDescriptorBase<EventDescriptor>
    ::name()
    {
        return name_;
    }
    template<class EventDescriptor>
    inline const ordinal_type NamespaceDescriptorBase<EventDescriptor>
    ::ordinal()
    {
        return ordinal_;
    }

    template<class EventDescriptor>
    inline const std::size_t NamespaceDescriptorBase<EventDescriptor>::
    event_container_size() const
    {
        return events_.size();
    }
    template<class EventDescriptor>
    inline const std::size_t NamespaceDescriptorBase<EventDescriptor>::
    event_container_occupied_size() const
    {
        return  events_.occupied_size();    
    }
    template<class EventDescriptor>
    inline const OP_RESPONSE NamespaceDescriptorBase<EventDescriptor>::
    ContainsEventIdentifier(const Oid & oid) const
    {
        return events_.ContainsEither(oid); 
    }

    template<class EventDescriptor>
    inline const EventDescriptor *
    NamespaceDescriptorBase<EventDescriptor>::
    EventAt(const ordinal_type ordinal) const
    {
        return events_[ordinal];        
    }
    template<class EventDescriptor>
    inline const EventDescriptor *
    NamespaceDescriptorBase<EventDescriptor>::
    EventWithName(const OidName & name_in) const
    {
        return events_[name_in];
    }
    //-----------------------------------------------------------------------//
}
#endif 

