#include <event_processing/detail/event_string_generator.h>

namespace karma = boost::spirit::karma;


event_string_generator_grammar::event_string_generator_grammar()
    : event_string_generator_grammar::base_type(base_rule)
{
    using karma::eps;
    using karma::char_;

    base_rule = char_('(') << char_(')');
     
}
