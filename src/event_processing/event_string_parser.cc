#include <event_processing/detail/event_string_parser.h>
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
 
    event_parse_grammar::event_parse_grammar() 
            :   event_parse_grammar::base_type(event_rule, "event_rule")
    { 
        rped_.reset();
     
        #define _CO \
        phoenix::bind(  &event_parser_error_descriptor::current_ordinal,    \
                        this->rped_)

        #define _CVT\
        phoenix::bind(  &event_parser_error_descriptor::current_value_type,   \
                        this->rped_)

        #define _WPV\
        phoenix::bind(  &event_parser_error_descriptor::was_parsing_value,    \
                        this->rped_)
    
        #define _HAS_ERROR\
        phoenix::bind(  &event_parser_error_descriptor::has_error, \
                        this->rped_)

        #define _SET_VALUE \
        phoenix::bind(  &event_parse_grammar::set_value, \
                        *this,_r1,_CO,_a,_pass)
       
        #define _TYPE(type_entry) \
        phoenix::bind( & EventTypeDescriptor::type, type_entry)

        quoted_string =  
            char_('"') > 
            lexeme[ *( (char_ - '"') [_val+=_1] | lit("\"\"")[_val+='"'] ) ]
             > char_('"');

        event_entry = 
            qi::ushort_[ _CO=_1] > '=' > 
            eps [_CVT = _TYPE(phoenix::bind(&EventDescriptorBase::TypeAt, _r2, _CO))] >>
            (       ( eps( _CVT == VALUE_INT4) >> p_int32_t 
                        [ _a = _1, _SET_VALUE ])
                |   ( eps( _CVT == VALUE_INT8) >> p_int64_t 
                        [ _a = _1, _SET_VALUE ])
                |   ( eps( _CVT == VALUE_STRING) >> quoted_string
                        [ _a = _1, _SET_VALUE] )
            )
        ; 
        
        event_rule = 
            qi::ushort_ > '(' >
            *(  eps [_WPV = true] 
                >> event_entry(_r1,_r2)
                > ( qi::char_(',') | ')' ) [ _WPV = false ]
            )
        ;
        
        qi::on_error<qi::fail>(event_rule, _HAS_ERROR=true);
    }
}
