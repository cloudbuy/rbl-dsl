#ifndef _EM_TYPES_H
#define _EM_TYPES_H
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/cstdint.hpp>
#include <vector>
#include <string>
#include <boost/variant.hpp>
#include <ostream>

using boost::int32_t;
using boost::int64_t;

namespace event_model
{
///////////////////////////////////////////////////////////////////////////////
    enum VALUE_TYPE
    {
        VALUE_UNINITIALIZED=0,
        VALUE_INT4,
        VALUE_INT8,
        VALUE_STRING
    };
///////////////////////////////////////////////////////////////////////////////    
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
///////////////////////////////////////////////////////////////////////////////
    struct undefined {} ; //to enable an empty slot in the event
    typedef boost::variant< 
                    undefined ,int32_t, int64_t, std::string> value_variant;
    typedef std::vector<value_variant> value_variant_vector;
///////////////////////////////////////////////////////////////////////////////
    template<typename MATCHED_TYPE> 
    class type_test_variant_visitor : boost::static_visitor<>
    {
    public:   
        typedef bool result_type;
        
        result_type operator()(MATCHED_TYPE & mt) const
        {
            return true;
        }
        
        template<typename UNMATCHED_TYPE>
        result_type operator()(UNMATCHED_TYPE & ut) const
        {
            return false;
        }
    };
////////////////////////////////////////////////////////////////////////////////
    class get_type_variant_visitor : boost::static_visitor<>
    {
    public:
        typedef VALUE_TYPE result_type;
        
        result_type operator()(std::string)    const { return VALUE_STRING;}
        result_type operator()(boost::int32_t) const { return VALUE_INT4; }
        result_type operator()(boost::int64_t) const { return VALUE_INT8; }

        template<typename UNMATCHED_TYPE>
        result_type operator()(UNMATCHED_TYPE & um) const 
                                { return VALUE_UNINITIALIZED; }
    };
///////////////////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////////////////
// no op to enable the boost::variant to be "output streamable"
inline std::ostream & operator<< (std::ostream & out, event_model::undefined & undef)
{
    return out;
}
///////////////////////////////////////////////////////////////////////////////
#endif
