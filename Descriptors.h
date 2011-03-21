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
        EVENT_DESCRIPTOR_QUALIFIER qualifier;
        VALUE_TYPE  type;
        bool primitive;

        EventTypeDescriptor();
        EventTypeDescriptor( EVENT_DESCRIPTOR_QUALIFIER _qualifier,
                             VALUE_TYPE _type,
                             bool primitive_in);
        void serialize(SF::Archive & ar);
    };
    
    typedef OidContainer<Oid, EventTypeDescriptor> EventTypeContainer;

    class RelayEventDescriptor;

    class GeneratorEventDescriptor
    {
    public:
        GeneratorEventDescriptor();
        GeneratorEventDescriptor(const RelayEventDescriptor &);
        GeneratorEventDescriptor(const EventTypeContainer & );
        //GeneratorEventDescriptor & operator=(const RelayEventDescriptor & rhs)  {}
 
        EventTypeContainer events;
    };
   
    class MarshallEventDescriptor;
 
    class RelayEventDescriptor : public GeneratorEventDescriptor
    {
    public:
        RelayEventDescriptor();
        RelayEventDescriptor(const MarshallEventDescriptor &);
        RelayEventDescriptor(const EventTypeContainer &);
    };

    class MarshallEventDescriptorBuilder;
    
    class MarshallEventDescriptor : public RelayEventDescriptor
    {
    public:
        MarshallEventDescriptor();
        MarshallEventDescriptor(const MarshallEventDescriptorBuilder &);
    };

    class MarshallEventDescriptorBuilder : public MarshallEventDescriptor
    {
    
        void AddEventTypeEntry( const Oid & oid, 
                                const EventTypeDescriptor & ets);
    };

    class RelayNamespaceDescriptor;

    class GeneratorNamespaceDescriptor
    {
    public:
        typedef OidContainer<Oid,GeneratorEventDescriptor> 
            EventDescriptorContainer;

        GeneratorNamespaceDescriptor();
        GeneratorNamespaceDescriptor(const RelayNamespaceDescriptor &);
        
        std::string name;
        EventDescriptorContainer Events;
     };

    class MarshallNamespaceDescriptor;

    class RelayNamespaceDescriptor : public GeneratorNamespaceDescriptor
    {
    public:
        typedef OidContainer<Oid,RelayEventDescriptor>
            EventDescriptorContainer;

        RelayNamespaceDescriptor();
        RelayNamespaceDescriptor(const std::string &);
        
        EventDescriptorContainer Events;
    };

    class MarshallNamespaceDescriptor : public RelayNamespaceDescriptor
    {
    public:
        typedef OidContainer<Oid,MarshallEventDescriptor> 
            EventDescriptorContainer;
        MarshallNamespaceDescriptor();
        MarshallNamespaceDescriptor(const std::string & name_in);
        
        const EventDescriptorContainer Events;    
    };
    
    class MarshallNamespaceDescriptorBuilder 
        : public MarshallNamespaceDescriptor
    {
    public:
        MarshallNamespaceDescriptorBuilder();
        MarshallNamespaceDescriptorBuilder(const std::string &);
         
    };
}
#endif
