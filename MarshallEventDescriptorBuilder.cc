#include "MarshallEventDescriptor.h"
#include "MarshallEventDescriptorBuilder.h"

namespace event_model
{
    using namespace primitives;

    MarshallEventDescriptorBuilder::MarshallEventDescriptorBuilder()
        : etc_()
    {
        
    }

    void
    MarshallEventDescriptorBuilder::AddEventType(
        const EventTypeContainer::entry_type & eti, bool & pass_)
    {
        OP_RESPONSE resp = etc_.SetEntry( eti );
        if(resp == OP_NO_ERROR)
            pass_ = true;
        else
        {
        }
    }
};
