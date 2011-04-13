#ifndef _EM_ROW_AND_TABLE_H
#define _EM_ROW_AND_TABLE_H
#include <event_model/Types.h>
#include <event_model/Descriptors.h>
#include "detail/event_string_parser.h"
#include <boost/scoped_ptr.hpp>

namespace event_model
{
    class Event
    {
    public:
        Event(const EventDescriptor & event_descriptor);
        bool operator >>(std::string & str) const; 
        bool operator <<(const std::string & str);
        const value_variant_vector & get_event_vector() const;
   
        void reset_event(); 

        const event_parser_error_descriptor * 
            get_event_parser_error_descriptor() const 
        { 
            if(epg_scptr_ != NULL)
                return &epg_scptr_->rped_; 
            else
                return NULL;
        }
    private:
        value_variant_vector event_data_;
        const EventDescriptor & ed_;

        boost::scoped_ptr<event_parse_grammar> epg_scptr_;
        void reset_parser();
    };
   
     
    
    class EventSet
    {
    public:
        EventSet(const EventDescriptor & event_descriptor);

        bool push(const Event & event);
        bool pop(Event & event) const;
        typedef std::vector<value_variant_vector> event_vector;
    private:
        event_vector events_;
        const EventDescriptor & ed_;
        
    };
};
#endif 
