#include <event_model/RowAndTable.h>
#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

namespace event_model
{
                     #define  ADD_ENTRY(TP,prefix)                                    \
            void AddEntry_##prefix( value_variant_vector & v, uint32_t ordinal,\
                            TP entry, bool & ok_)                            \
            {                                                                \
                ok_ = true;                                                  \
                if( ordinal < v.size() )                                     \
                v[ordinal] = entry;                                          \
                else ok_ = false;                                            \
            }
            ADD_ENTRY(int32_t,i32);
            ADD_ENTRY(int64_t,i64);


        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;
        namespace phoenix = boost::phoenix;

        using qi::_1;
        using qi::_a;
        using qi::_b;
        using qi::eps;
        using qi::_r1;
        using qi::_r2;
        using qi::_pass;

        struct row_parse_grammar : 
            qi::grammar< std::string::const_iterator, 
                         void(value_variant_vector &,const table_descriptor &),
                         ascii::space_type> 
        {
                       
            qi::int_parser< int32_t,  10, 1,10>  p_int32_t;
            qi::int_parser< int64_t, 10, 1, 19>  p_int64_t;
            
            
            qi::rule<   std::string::const_iterator,
                        void(value_variant_vector &, const table_descriptor &),
                        qi::locals<uint8_t, uint32_t>,
                        ascii::space_type>  row_entry;

            qi::rule<   std::string::const_iterator, 
                        void(value_variant_vector &, const table_descriptor &),
                        ascii::space_type> row_rule;

            row_parse_grammar() 
                : row_parse_grammar::base_type(row_rule, "row_rule")
            {  
                row_entry = 
                    qi::ushort_[_a=_1] > '=' > 
                    eps(_b = phoenix::bind(&RowTypeAt, _r2, _a)) >> 
                    (  ( eps(_b == VALUE_INT4) >> p_int32_t )
                            [phoenix::bind(&AddEntry_i32, _r1,_a, _1, _pass)] 
                    | ( eps(_b == VALUE_INT8) >> p_int64_t )
                        [phoenix::bind(&AddEntry_i64, _r1,_a, _1, _pass)]
                    | ( eps(_b == VALUE_STRING) )
                    )
                ;
                
                row_rule = 
                    qi::ushort_ > '(' 
                    >   row_entry(_r1,_r2) 
                    > *(',' > row_entry(_r1,_r2))
                    > ')'
                ;
            }
        };
    
    Row::Row(const table_descriptor & row_descriptor)
        : td_(row_descriptor), row_data_(row_descriptor.entry().size()) {}
    
    bool Row::operator << (const std::string & str)
    { 
        namespace phoenix = boost::phoenix;

        std::string::const_iterator beg = str.begin();
        std::string::const_iterator end = str.end();
    
        row_parse_grammar rpg;

        bool res = qi::phrase_parse(
            beg, end, 
            rpg( phoenix::ref(row_data_),phoenix::cref(td_)), 
            ascii::space);

        return res;
    }
    
    VALUE_TYPE RowTypeAt(   const table_descriptor & td_,
                            uint32_t ordinal)
    {
        const EventTypeContainer::entry_type * etd_e = td_.entry().EntryAtordinal(ordinal);
        if( etd_e == NULL)
        {
            return VALUE_UNINITIALIZED;
        }
        else return etd_e->entry().type();
        
    }
    const value_variant_vector & Row::get_row_vector() const
    {
        return row_data_; 
    }

 
    Table::Table(const table_descriptor & row_descriptor)
        : td_(row_descriptor), table_data_() {}
};
