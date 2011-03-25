#include "Descriptors.h"

namespace event_model
{
    using namespace primitives;
    
    void MarshallEventDescriptorBuilder::Init(   
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
    void MarshallEventDescriptorBuilder::AddEventType
        (const Oid & oid, const EventTypeDescriptor & type, bool & ok)
    {
        ok=true;
        if(types_.ContainsEither(oid) == OP_NO_ERROR) {
            EventTypeContainer::entry_type entry(oid,type);
            types_.SetEntry(entry);
            return;
        }
        ok = false;
    }
};
