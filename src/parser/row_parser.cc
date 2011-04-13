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
            :   row_parse_grammar::base_type(row_rule, "row_rule")
    { 
        rped_.reset();
     
        #define _CO \
        phoenix::bind(  &row_parser_error_descriptor::current_ordinal,      \
                        this->rped_)

        #define _CVT\
        phoenix::bind(  &row_parser_error_descriptor::current_value_type,   \
                        this->rped_)

        #define _WPV\
        phoenix::bind(  &row_parser_error_descriptor::was_parsing_value,    \
                        this->rped_)
    
        #define _HAS_ERROR\
        phoenix::bind(  &row_parser_error_descriptor::has_error, \
                        this->rped_)

        #define _SET_VALUE \
        phoenix::bind(  &row_parse_grammar::set_value, \
                        *this,_r1,_CO,_a,_pass)
       

        quoted_string =  
            char_('"') > 
            lexeme[ *( (char_ - '"') [_val+=_1] | lit("\"\"")[_val+='"'] ) ]
             > char_('"');

        row_entry = 
            qi::ushort_[ _CO=_1] > '=' > 
            eps [_CVT = phoenix::bind(&EventDescriptor::RowTypeAt, _r2, _CO)] >>
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
