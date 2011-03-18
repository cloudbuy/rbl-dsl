#ifndef _EM_MARSHALL_EVENT_DESCRIPTOR_BUILDER_H
#define _EM_MARSHALL_EVENT_DESCRIPTOR_BUILDER_H 

namespace event_model
{

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

#endif 
