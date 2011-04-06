#include <event_model/RowAndTable.h>
#include <boost/spirit/include/qi.hpp>

namespace event_model
{
    Row::Row(const table_descriptor & row_descriptor)
        : td_(row_descriptor), row_data_(row_descriptor.entry().size()) {}
    
    bool Row::operator << (const std::string & str)
    { 
        namespace phoenix = boost::phoenix;
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;

        std::string::const_iterator beg = str.begin();
        std::string::const_iterator end = str.end();
    
        row_parse_grammar rpg;
        
        bool res = qi::phrase_parse(
            beg, end, 
            rpg( phoenix::ref(row_data_),phoenix::cref(td_)), 
            ascii::space);

        return res;
    }
    
    const value_variant_vector & Row::get_row_vector() const
    {
        return row_data_; 
    }

 
    Table::Table(const table_descriptor & row_descriptor)
        : td_(row_descriptor), table_data_() {}
};
