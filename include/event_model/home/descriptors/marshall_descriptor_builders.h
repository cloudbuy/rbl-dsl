#ifndef RBL_EM_HOME_MARSHALL_DESCRIPTOR_BUILDER_H
#define RBL_EM_HOME_MARSHALL_DESCRIPTOR_BUILDER_H
#include "marshall_descriptors.h"
namespace rubble { namespace event_model {
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
    // type container, used directly by parser ////////////////////////////////
        EventTypeContainer types;
    private:
        Oid self_oid_;      
        MarshallNamespaceDescriptorBuilder * mndb_;
    };
    //-----------------------------------------------------------------------//
    
    // MarshallNamespaceDescriptorBuilder /////////////////////////////////////
    class MarshallNamespaceDescriptorBuilder : 
        public MarshallNamespaceDescriptor
    {
    public:
        MarshallNamespaceDescriptorBuilder(); 
        MarshallNamespaceDescriptorBuilder( const std::string & name_in, 
                                            const ordinal_type ordinal_in);
        operator MarshallNamespaceDescriptor() const;
        void AddEventDescriptor( const MarshallEventDescriptorBuilder & medb, 
                                 bool & ok);

        void set_name(const std::string & _name);
        
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

    inline void MarshallNamespaceDescriptorBuilder::AddEventDescriptor
    (const MarshallEventDescriptorBuilder & medb, bool & ok)
    {
        ok = true;
        MarshallEventDescriptor med(    medb.oid(),
                                        m_ordinal,
                                        medb.types);

        t_edc::entry_type entry
            (medb.oid(), med );
        
        if( m_events.SetEntry(entry) != OP_NO_ERROR)
            ok = false; 
    }

    inline MarshallNamespaceDescriptorBuilder::
    MarshallNamespaceDescriptorBuilder( const std::string & name_in,
                                          const ordinal_type ordinal_in) 
        : MarshallNamespaceDescriptor( name_in , ordinal_in ) 
    { 
    }
    inline void MarshallNamespaceDescriptorBuilder::set_name
      (const std::string & s_in)
    {
      this->m_name = s_in;
    }
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
} }
#endif
