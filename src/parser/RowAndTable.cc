#include <event_model/RowAndTable.h>
#include <boost/spirit/include/qi.hpp>

namespace event_model
{
    Row::Row(const EventDescriptor & row_descriptor)
        : td_(row_descriptor), row_data_(row_descriptor.entry().size()) {}
  
    void Row::reset_row()
    {
        row_data_.assign(td_.entry().size(), undefined());
    }
 
    void Row::reset_parser()
    {
        reset_row();
        if(rpg_scptr_!=NULL)
            rpg_scptr_->reset();
    }
 
    bool Row::operator << (const std::string & str)
    { 
        namespace phoenix = boost::phoenix;
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;

        if( rpg_scptr_ == NULL)
            rpg_scptr_.reset(new row_parse_grammar()); 
        else
            reset_parser();
    
        std::string::const_iterator beg = str.begin();
        std::string::const_iterator end = str.end();
        
        bool res = qi::phrase_parse(
            beg, end, 
            (*rpg_scptr_)( phoenix::ref(row_data_),phoenix::cref(td_)), 
            ascii::space);

        return res;
    }
    
    const value_variant_vector & Row::get_row_vector() const
    {
        return row_data_; 
    }

 
    Table::Table(const EventDescriptor & row_descriptor)
        : td_(row_descriptor), rows_() {}
};
