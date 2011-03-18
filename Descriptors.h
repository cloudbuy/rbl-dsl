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
                             VALUE_TYPE _type,
                             bool primitive_in);
        inline const bool is_primitive() const;
        void serialize(SF::Archive & ar);
    private:
        bool primitive_;
    };
    
    typedef OidContainer<Oid,EventTypeDescriptor> EventTypeContainer;

    // Generator
    class GeneratorEventDescriptor
    {
    public:
        GeneratorEventDescriptor();
        GeneratorEventDescriptor( const RelayEventDescriptor & red);
        void serialize(SF::Archive & ar);        
    private:
        EventTypeContainer etc_;
    };
    
    class GeneratorNamespaceDescriptor
    {
    public:
        GeneratorNamespaceDescriptor();
        GeneratorNamespaceDescriptor(const RelayNamespaceDescriptor & rnd);
        void serialize(SF::archive & ar);
    private:
        GeneratorEventDescriptor ged_;
    };   
    
    // relay
    class RelayEventDescriptor
    {
    public:
        RelayEventDescriptor();
        RelayEventDescriptor( const MarshallEventDescriptor & med);
        void serialize(SF::Archive & ar);
    private:
        EventTypecontainer etc_;
    };
    
    class RelayNamespaceDescriptor
    {
    public:
        RelayNamespaceDescriptor();
        RelayNamespaceDescriptor(const MarshallNamespaceDescriptor & mnd);
        void serialize(SF::archive & ar);
    private:
        RelayEventDescriptor etc_;
    };
    
    // Marshall
    class MarshallEventDescriptor
    {
    public:
    private:
        EventTypeContainer etc_;
    };
    
    class MarshallNamespaceDescriptor
    {
    public:
        MarshallNamespaceDescriptor();

        inline void set_name(const std::string name_in);
        inline const std::string & name() const;

    private:
        std::string name_;
        MarshallEventDescriptor med_;
    };
    
    // Builders
    class MarshallEventDescriptorBuilder
    {
    public:
        MarshallEventDescriptorBuilder();
        inline void AddEventType(   const EventTypeContainer::entry_type & et,
                                    bool & pass); 
    private:
        EventTypeContainer etc_;
    };

    class MarshallNamespaceDescriptorBuilder
    {
    };


}

