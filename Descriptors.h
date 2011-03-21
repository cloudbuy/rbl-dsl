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

    class GeneratorEventDescriptor
    {
    public:
        GeneratorEventDescriptor();
        GeneratorEventDescriptor(const Oid & identifier);

        const Oid identifier;
        const EventTypeContainer events;
    };
    
    class RelayEventDescriptor : public GeneratorEventDescriptor
    {
    public:
        RelayEventDescriptor();
        RelayEventDescriptor(const Oid & identifier);
    };

    class MarshallEventDescriptor : public RelayEventDescriptor
    {
    public:
        MarshallEventDescriptor();
        MarshallEventDescriptor(const Oid & identifier);
    };

    class MarshallEventDescriptorBuilder : public MarshallEventDescriptor
    {
    
        void AddEventTypeEntry( const Oid & oid, 
                                const EventTypeDescriptor & ets);
    };

    class GeneratorNamespaceDescriptor
    {
    public:
        GeneratorNamespaceDescriptor();
        GeneratorNamespaceDescriptor(const std::string &);
        const std::string name;
    };

    class RelayNamespaceDescriptor : public GeneratorNamespaceDescriptor
    {
    public:
        RelayNamespaceDescriptor();
        RelayNamespaceDescriptor(const std::string &);
    };

    class MarshallNamespaceDescriptor : public RelayNamespaceDescriptor
    {
    public:
        MarshallNamespaceDescriptor();
        MarshallNamespaceDescriptor(const std::string & name_in);
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
