#include <event_processing/EventSet.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

namespace event_model
{
    Event::Event(const EventDescriptor & event_descriptor)
        : ed_(event_descriptor), event_data_(event_descriptor.entry().size()) {}
  
    void Event::reset_event()
    {
        event_data_.assign(ed_.entry().size(), undefined());
    }
 
    void Event::reset_parser()
    {
        reset_event();
        if(epg_scptr_!=NULL)
            epg_scptr_->reset();
    }

    bool Event::operator >> (std::string & str) const 
    {
        namespace phoenix = boost::phoenix;

        std::back_insert_iterator<std::string> sink(str);
        
        if(epgg_scptr_ == NULL)
            epgg_scptr_.reset(new event_string_generator_grammar());

        // reset something ? 

        return boost::spirit::karma::generate(
            sink,
            (*epgg_scptr_)(phoenix::cref(ed_),phoenix::cref(event_data_)),
            event_data_);
    }
 
    bool Event::operator << (const std::string & str)
    { 
        namespace phoenix = boost::phoenix;
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;

        if( epg_scptr_ == NULL)
            epg_scptr_.reset(new event_parse_grammar()); 
        else
            reset_parser();
    
        std::string::const_iterator beg = str.begin();
        std::string::const_iterator end = str.end();
        
        bool res = qi::phrase_parse(
            beg, end, 
            (*epg_scptr_)( phoenix::ref(event_data_),phoenix::cref(ed_)), 
            ascii::space);

        return res;
    }
    
    const value_variant_vector & Event::get_event_vector() const
    {
        return event_data_; 
    }

 
    EventSet::EventSet(const EventDescriptor & event_descriptor)
        : ed_(event_descriptor), events_() {}
};
