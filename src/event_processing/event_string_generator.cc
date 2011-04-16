#include <event_processing/detail/event_string_generator.h>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace karma = boost::spirit::karma;
namespace phoenix = boost::phoenix;

void event_string_generator_grammar::
SetLocalValuePtrAtOrdinal(  const event_model::value_variant_vector & vvv, 
                            uint32_t ordinal)
{
    current_value_ptr = & vvv[ordinal];
} 

event_string_generator_grammar::
event_string_generator_grammar()
    : event_string_generator_grammar::base_type(base_rule),
      current_value_type(event_model::VALUE_UNINITIALIZED),
      current_ordinal(0)
{
    using karma::eps;
    using karma::char_;
    using karma::int_;
    using karma::_1;
    using karma::_r1;
    using karma::_r2;
    using karma::ascii::space;
    using karma::repeat;
    using event_model::value_variant_vector;
    using event_model::EventDescriptor;
    using namespace event_model; 
   
    #define _CO                                                             \
    phoenix::bind(&event_string_generator_grammar::current_ordinal,*this)

    #define _CVT_CURRENT(ordinal)                                           \
    phoenix::bind(&EventDescriptor::RowTypeAt, _r1, ordinal)
    
    #define _CVT_LOCAL                                                      \
    phoenix::bind(&event_string_generator_grammar::current_value_type,*this)
   
    #define _SET_CV_LOCAL(ordinal)                                          \
    phoenix::bind(  &event_string_generator_grammar::                       \
                    SetLocalValuePtrAtOrdinal, *this, _r2,ordinal)

    #define _CV_LOCAL                                                       \
    phoenix::bind(  &event_string_generator_grammar::                       \
                    current_value_ptr,*this)                   
    
    #define _VVV_SIZE                                                       \
    phoenix::bind( &value_variant_vector::size,                             \
                   _r2 )

    base_rule = 
        int_ [ _1 = phoenix::bind(&EventDescriptor::ordinal, _r1) ]         <<
        space << char_('(') << space                                        <<
        repeat(_VVV_SIZE) [
            eps[_CVT_LOCAL = _CVT_CURRENT(_CO)]                             << 
            eps[_SET_CV_LOCAL(_CO)]                                         << 
            ( ( eps( _CVT_LOCAL != VALUE_UNINITIALIZED)                     <<
                (
                    (   ( eps(_CO !=0) << space << char_(',') << space)     
                        | eps)                                              <<
                    int_[_1=_CO] << char_("=")                              <<
                    (   quoted_type_rule(*_CV_LOCAL) 
                        | terminal_type_rule(*_CV_LOCAL) )
                )
              ) | eps
            )                                                               <<
            eps[ _CO = _CO+1 ]                                              
        ]                                                                   <<
        space << char_(')')
    ;

    quoted_type_rule = 
        eps( _CVT_LOCAL == VALUE_STRING)                                    << 
        char_('\"')                                                         <<
        karma::stream(_r1)                                                  <<
        char_('\"')
    ;

    terminal_type_rule  = 
        karma::stream(_r1)
    ;
}
