#ifndef _EM_ROW_AND_TABLE_H
#define _EM_ROW_AND_TABLE_H
#include "event_model/DescriptorsCommon.h"
#include "detail/event_string_parser.h"
#include "detail/event_string_generator.h"
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>

/// \todo   add a verify function, and wire it up to the parser this also 
///         involves adding the failure case to the error struct.

/// \todo   implement logic to specify to the generator weather it uses
///         ordinals or identifiers. And the string parser needs to be able to 
///         handle both.

/// \todo   verify uin8_t ordinal use is sane with regards to the callee's
///         -- i.e., the uint32_t API that calls it, the callee's need to 
///         test for overflows

namespace event_model {
    class Event : boost::noncopyable
    {
    public:
        Event(const EventDescriptorBase & event_descriptor);
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
        const EventDescriptorBase & ed_;

        boost::scoped_ptr<event_parse_grammar> epg_scptr_;
        mutable boost::scoped_ptr<event_string_generator_grammar> epgg_scptr_;

        void reset_parser();
    };
    
    class EventSet : boost::noncopyable
    {
    public:
        EventSet(const EventDescriptorBase & event_descriptor);

        bool push(const Event & event);
        bool pop(Event & event) const;
        typedef std::vector<value_variant_vector> event_vector;
    private:
        event_vector events_;
        const EventDescriptorBase & ed_;
        
    };
};
#endif 
