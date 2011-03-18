#include "CommonDescriptors.h"

namespace event_model
{
    class MarshallEventDescriptor
    {
    public:
    private:
        EventTypeContainer etc_;
    };
    
    class MarshallEventDescriptorBuilder
    {
    public:
        MarshallEventDescriptorBuilder();
        inline void AddEventType(   const EventTypeContainer::entry_type & et,
                                    bool & pass); 
    private:
        EventTypeContainer etc_;
    };
}
