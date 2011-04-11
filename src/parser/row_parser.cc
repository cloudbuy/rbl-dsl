#include <parser/detail/row_parser.h>
#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

#include <boost/variant/apply_visitor.hpp>

// the following header doesn't feature in any of the exmaples of
// the qi library, however in order to chain multiple statements 
// in the semantic actions using a comma, this file needs to have
// been included.
#include <boost/spirit/home/phoenix/statement/sequence.hpp>


namespace event_model
{
    VALUE_TYPE RowTypeAt(   const table_descriptor & td_,
                            uint32_t ordinal)
    {
        const EventTypeContainer::entry_type * etd_e = 
            td_.entry().EntryAtordinal(ordinal);
        if( etd_e == NULL)
        {
            return VALUE_UNINITIALIZED;
        }
        else return etd_e->entry().type();
    }
 
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    using qi::_1;
    using qi::_a;
    using qi::_b;
    using qi::_c;
    using qi::eps;
    using qi::_r1;
    using qi::_r2;
    using qi::_pass;
    using qi::lexeme;
    using qi::char_;   
    using qi::_val;
    using qi::lit;
 
    row_parse_grammar::row_parse_grammar() 
            :   row_parse_grammar::base_type(row_rule, "row_rule"),
                was_parsing_value(false),
                has_error(false),
                double_assignment(false),
                out_of_range(false)
    {  
        #define _CO \
        phoenix::bind(&row_parse_grammar::current_ordinal,*this)

        #define _CVT\
        phoenix::bind(&row_parse_grammar::current_value_type,*this)

        #define _WPV\
        phoenix::bind(&row_parse_grammar::was_parsing_value, * this)
    
        #define _HAS_ERROR\
        phoenix::bind(&row_parse_grammar::has_error, * this)

        #define _SET_VALUE \
        phoenix::bind(  &row_parse_grammar::set_value, \
                        *this,_r1,_CO,_a,_pass)
       

        quoted_string =  
            char_('"') > 
            lexeme[ *( (char_ - '"') [_val+=_1] | lit("\"\"")[_val+='"'] ) ]
             > char_('"');

        row_entry = 
            qi::ushort_[ _CO=_1] > '=' > 
            eps [_CVT = phoenix::bind(&RowTypeAt, _r2, _CO)] >>
            (       ( eps( _CVT == VALUE_INT4) >> p_int32_t 
                        [ _a = _1, _SET_VALUE ])
                |   ( eps( _CVT == VALUE_INT8) >> p_int64_t 
                        [ _a = _1, _SET_VALUE ])
                |   ( eps( _CVT == VALUE_STRING) >> quoted_string
                        [ _a = _1, _SET_VALUE] )
            )
        ; 
        
        row_rule = 
            qi::ushort_ > '(' >
            *(  eps [_WPV = true] 
                >> row_entry(_r1,_r2)
                > ( qi::char_(',') | ')' ) [ _WPV = false ]
            )
        ;
        
        qi::on_error<qi::fail>(row_rule, _HAS_ERROR=true);
    }
}
