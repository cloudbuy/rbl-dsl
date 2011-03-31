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
        
        void set_is_primitive(bool _is_primitive);
        void set_qualifier(EVENT_DESCRIPTOR_QUALIFIER _qualifier);
        void set_type(VALUE_TYPE _type);
 
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

        EventTypeContainer types;
        void serialize(SF::Archive & ar);
    private: 
    };
///////////////////////////////////////////////////////////////////////////////   
    class MarshallEventDescriptor;
 
    class RelayEventDescriptor     
    {
    public:
        RelayEventDescriptor();
        explicit RelayEventDescriptor(const  EventTypeContainer & etc);
        explicit RelayEventDescriptor(const MarshallEventDescriptor &);
        operator GeneratorEventDescriptor() const;
        void serialize(SF::Archive & ar);
        
        EventTypeContainer types;
    private:
    };
///////////////////////////////////////////////////////////////////////////////
    class MarshallEventDescriptorBuilder;
    
    class MarshallEventDescriptor     
    {
    public:
        MarshallEventDescriptor();
        explicit MarshallEventDescriptor(const EventTypeContainer & etc);
        operator RelayEventDescriptor() const;

        EventTypeContainer types;
    
    };
///////////////////////////////////////////////////////////////////////////////
    class MarshallNamespaceDescriptorBuilder;

    class MarshallEventDescriptorBuilder     
    {
    public:
        MarshallEventDescriptorBuilder();
        void Init(  const Oid & oid, 
                    MarshallNamespaceDescriptorBuilder & mndb,
                    bool & ok);
        void AddEventType(  const Oid & oid, 
                            const EventTypeDescriptor & type,bool & ok); 
        const Oid & oid() const;
        operator MarshallEventDescriptor() const;
        
        ContainerBuilder<EventTypeContainer> types;
    private:
        Oid self_oid_;
        MarshallNamespaceDescriptorBuilder * mndb_;
        ;
    };
///////////////////////////////////////////////////////////////////////////////
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
        GeneratorNamespaceDescriptor();
        explicit GeneratorNamespaceDescriptor(
            const std::string &, const RelayEDC &);
        void serialize(SF::Archive & ar);
        const std::string & name() const ;

        EventDescriptorContainer events;
    private: 
        std::string name_;
     };
///////////////////////////////////////////////////////////////////////////////
    class MarshallNamespaceDescriptor;

    class RelayNamespaceDescriptor     
    {
    public:
        typedef SlicingContainer<RelayEDC, MarshallEDC> 
            EventDescriptorContainer;
        explicit RelayNamespaceDescriptor(
            const std::string &, const MarshallEDC &);
        RelayNamespaceDescriptor();
        void serialize(SF::Archive & ar);
        const std::string & name() const ;
        operator GeneratorNamespaceDescriptor() const;
        
        EventDescriptorContainer events;
    private: 
        std::string name_;
    };
/////////////////////////////////////////////////////////////////////////////// 
    // the builder is declared above, before the event builder.
    class MarshallNamespaceDescriptor
    {
    public:
        typedef MarshallEDC EventDescriptorContainer;

        explicit MarshallNamespaceDescriptor
            (   const std::string & name, 
                const MarshallEDC & EDC);

        operator RelayNamespaceDescriptor() const;
 
        const std::string & name() const ;
        
        EventDescriptorContainer events;
    private:
        std::string name_;
    };
///////////////////////////////////////////////////////////////////////////////    
    class MarshallNamespaceDescriptorBuilder  
    {
    public:
        typedef ContainerBuilder<MarshallEDC> EventDescriptorContainer;
        MarshallNamespaceDescriptorBuilder(); 
        MarshallNamespaceDescriptorBuilder(const std::string &);

        void AddEventDescriptor( const MarshallEventDescriptorBuilder & medb, 
                                 bool & ok);
    
        operator MarshallNamespaceDescriptor() const;

        const std::string & name() const ;
        void set_name(std::string & _name);
        const OP_RESPONSE ContainsEventIdentifier(const Oid & id) const;
        
        EventDescriptorContainer events;
    private:
        std::string name_;
    };
///////////////////////////////////////////////////////////////////////////////   
    #include "Descriptors_inline.h"
}
#endif
