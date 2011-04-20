#ifndef _MARSHALL_DESCRIPTOR_BUILDER
#define _MARSHALL_DESCRIPTOR_BUILDER
#include "marshall/Descriptors.h"
namespace event_model
{
// Class Declarations /////////////////////////////////////////////////////////
    class MarshallNamespaceDescriptorBuilder;
    ///////////////////////////////////////////////////////////////////////////
    class MarshallEventDescriptorBuilder
    {
    public:
    // Constructor and Initialization /////////////////////////////////////////
        MarshallEventDescriptorBuilder();
        void Init(  const Oid & oid, 
                    MarshallNamespaceDescriptorBuilder & mndb,
                    bool & ok);
    // Add Events /////////////////////////////////////////////////////////////
        void AddEventType(  const Oid & oid, 
                            const EventTypeDescriptor & type,bool & ok); 
    // Getters ////////////////////////////////////////////////////////////////
        const Oid & oid() const;
    // Downcast To The Descriptor /////////////////////////////////////////////
        operator MarshallEventDescriptor() const;
    // type container, used directly by parser ////////////////////////////////
        EventTypeContainer types;
    private:
        Oid self_oid_;      
        MarshallNamespaceDescriptorBuilder * mndb_;
    };
    //-----------------------------------------------------------------------//
    
    // MarshallNamespaceDescriptor ////////////////////////////////////////////
    class MarshallNamespaceDescriptorBuilder : 
        public MarshallNamespaceDescriptor
    {
    public:
        MarshallNamespaceDescriptorBuilder(); 
        MarshallNamespaceDescriptorBuilder( const std::string & name_in, 
                                            const ordinal_type ordinal_in);

        void AddEventDescriptor( const MarshallEventDescriptorBuilder & medb, 
                                 bool & ok);
    
        operator MarshallNamespaceDescriptor() const;

        void set_name(std::string & _name);
        
    private:
    };
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
    // MarshallEventDescriptorBuilder /////////////////////////////////////////
    inline MarshallEventDescriptorBuilder::MarshallEventDescriptorBuilder()
    : types() {}

    inline const Oid & MarshallEventDescriptorBuilder::oid() const
    { return self_oid_; }

    inline void MarshallEventDescriptorBuilder::Init(   
        const Oid & oid, 
        MarshallNamespaceDescriptorBuilder & mndb,
        bool & ok)
    {
        self_oid_ = oid;
        mndb_= &mndb;
        ok = true; 
        
        OP_RESPONSE resp = mndb_->ContainsEventIdentifier(oid);
        
        if(resp == OP_NO_ERROR) return;
        //TODO set error string
        ok = false;
        return;
    }

    inline void MarshallEventDescriptorBuilder::AddEventType
        (const Oid & oid, const EventTypeDescriptor & type, bool & ok)
    {
        ok=true;
        if(types.ContainsEither(oid) == OP_NO_ERROR) {
            EventTypeContainer::entry_type entry(oid,type);
            types.SetEntry(entry);
            return;
        }
        ok = false;
    }
    //-----------------------------------------------------------------------//



    // MarshallNamespaceDescriptorBuilder /////////////////////////////////////
    inline MarshallNamespaceDescriptorBuilder::
    MarshallNamespaceDescriptorBuilder()
        : MarshallNamespaceDescriptor() 
    { 
    }

    inline MarshallNamespaceDescriptorBuilder::
    MarshallNamespaceDescriptorBuilder( const std::string & name_in,
                                          const ordinal_type ordinal_in) 
        : MarshallNamespaceDescriptor( name_in , ordinal_in ) 
    { 
    }
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
}
#endif
