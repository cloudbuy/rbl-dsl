#ifndef _EM_TYPES_H
#define _EM_TYPES_H
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/cstdint.hpp>
#include <vector>
#include <string>
#include <boost/variant.hpp>

using boost::int32_t;
using boost::int64_t;

namespace event_model
{
    enum VALUE_TYPE
    {
        VALUE_UNINITIALIZED=0,
        VALUE_INT4,
        VALUE_INT8,
        VALUE_STRING
    };
    
    namespace parser
    {
        struct MarshallTypes : boost::spirit::qi::symbols<char, VALUE_TYPE>
        {
            MarshallTypes()
            {
                using namespace event_model;

                add
                    ("int", VALUE_INT4)
                    ("int4", VALUE_INT4)
                    ("int8", VALUE_INT8)
                    ("bigint", VALUE_INT8)
                    ("string", VALUE_STRING)
                ;
            }
        };
    };
    struct undefined {} ;
    typedef boost::variant< undefined ,int32_t, int64_t, std::string> value_variant;
    typedef std::vector<value_variant> value_variant_vector;
}
#endif
