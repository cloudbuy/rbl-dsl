#ifndef _EM_ROW_PARSER_H
#define _EM_ROW_PARSER_H

#include <boost/spirit/include/qi.hpp>
#include <event_model/Types.h>
#include <event_model/Descriptors.h>
#include <string>

namespace event_model
{
    VALUE_TYPE RowTypeAt(   const table_descriptor & td_,
                            const uint32_t ordinal);   

    struct row_parse_grammar : 
        boost::spirit::qi::grammar< std::string::const_iterator, 
            void(value_variant_vector &,const table_descriptor &),
            boost::spirit::qi::ascii::space_type> 
    {
        boost::spirit::qi::int_parser< int32_t,  10, 1,10>  p_int32_t;
        boost::spirit::qi::int_parser< int64_t, 10, 1, 19>  p_int64_t;
            
        boost::spirit::qi::rule<   std::string::const_iterator,
                    void(value_variant_vector &, const table_descriptor &),
                    boost::spirit::ascii::space_type>  row_entry;

        boost::spirit::qi::rule<   std::string::const_iterator, 
                    void(value_variant_vector &, const table_descriptor &),
                    boost::spirit::ascii::space_type> row_rule;

        uint16_t current_ordinal;
        uint32_t current_value_type;
        bool was_parsing_value;
        bool has_error;
    
        row_parse_grammar();
    };
}
#endif
