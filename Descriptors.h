#ifndef _EM_DESCRIPTORS_H
#define _EM_DESCRIPTORS_H

#include <boost/cstdint.hpp>
#include <RCF/Idl.hpp>
#include <SF/Archive.hpp>
#include "OidContainer.h"

namespace event_model
{
    using namespace primitives;
   
    // Common 
    typedef OidConstrainedString<char, 32> OidName;
    typedef OidType<OidName, boost::uint8_t> Oid;
    
    enum VALUE_TYPE
    {
        VALUE_UNINITIALIZED,
        VALUE_INT4,
        VALUE_INT8,
        VALUE_STRING
    };

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
 
        void serialize(SF::Archive & ar);
    private:
        EVENT_DESCRIPTOR_QUALIFIER qualifier_;
        VALUE_TYPE  type_;
        bool primitive_;
    };
 
    typedef OidContainer<Oid, EventTypeDescriptor> EventTypeContainer;
///////////////////////////////////////////////////////////////////////////////
    class RelayEventDescriptor;

    class GeneratorEventDescriptor
    {
    public:
        GeneratorEventDescriptor();
        explicit GeneratorEventDescriptor( const EventTypeContainer & etc);
        const EventTypeContainer & events() const;
    private: 
        EventTypeContainer events_;
    };
///////////////////////////////////////////////////////////////////////////////   
    class MarshallEventDescriptor;
 
    class RelayEventDescriptor     
    {
    public:
        RelayEventDescriptor();
        explicit RelayEventDescriptor(const  EventTypeContainer & etc);
        explicit RelayEventDescriptor(const MarshallEventDescriptor &);
   
        const EventTypeContainer & events() const;
        operator GeneratorEventDescriptor();
    private:
        EventTypeContainer events_;
    };
///////////////////////////////////////////////////////////////////////////////
    class MarshallEventDescriptorBuilder;
    
    class MarshallEventDescriptor     
    {
    public:
        MarshallEventDescriptor();
        
        const EventTypeContainer & events() const;
        operator RelayEventDescriptor();
    private:
        EventTypeContainer events_;
    };
///////////////////////////////////////////////////////////////////////////////
    class MarshallEventDescriptorBuilder     
    {
    public:
        void AddEventTypeEntry( const Oid & oid, 
            const EventTypeDescriptor & ets);
        
        const EventTypeContainer & events() const;
    private:
        EventTypeContainer events_;
    };

    typedef OidContainer<Oid,GeneratorEventDescriptor>  GeneratorEDC;
    typedef OidContainer<Oid,RelayEventDescriptor>      RelayEDC;
    typedef OidContainer<Oid,MarshallEventDescriptor>   MarshallEDC;

///////////////////////////////////////////////////////////////////////////////    
    class RelayNamespaceDescriptor;

    class GeneratorNamespaceDescriptor
    {
    public:
        typedef SlicingContainer<GeneratorEDC, RelayEDC> 
            EventDescriptorContainer;
        explicit GeneratorNamespaceDescriptor(
            const RelayNamespaceDescriptor &);
        
        const std::string & name() const ;
        const EventDescriptorContainer & events() const;
    private: 
        std::string name_;
        EventDescriptorContainer events_;
     };
///////////////////////////////////////////////////////////////////////////////
    class MarshallNamespaceDescriptor;

    class RelayNamespaceDescriptor     
    {
    public:
        typedef SlicingContainer<RelayEDC, MarshallEDC> 
            EventDescriptorContainer;
        explicit RelayNamespaceDescriptor(const MarshallNamespaceDescriptor &);
        
        const std::string & name() const ;
        const EventDescriptorContainer & events() const ;
    private: 
        std::string name_;
        EventDescriptorContainer events_;
    };
/////////////////////////////////////////////////////////////////////////////// 
    class MarshallNamespaceDescriptorBuilder;

    class MarshallNamespaceDescriptor
    {
    public:
        typedef MarshallEDC EventDescriptorContainer;

        explicit MarshallNamespaceDescriptor
            (const MarshallNamespaceDescriptorBuilder & nsb);
        
        const std::string & name() const ;
        const MarshallEDC & events() const;
    private:
        std::string name_;
        EventDescriptorContainer events_;
    };
///////////////////////////////////////////////////////////////////////////////    
    class MarshallNamespaceDescriptorBuilder  
    {
    public:
        typedef MarshallEDC EventDescriptorContainer;

        MarshallNamespaceDescriptorBuilder(const std::string &);
        const std::string & name() const ;
        const MarshallEDC & events() const ;
    private:
        std::string name_;
        EventDescriptorContainer events_;     
    };
///////////////////////////////////////////////////////////////////////////////   
    #include "Descriptors_inline.h"
}
#endif
