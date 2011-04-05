#include <event_model/RowAndTable.h>
#include <boost/spirit/include/qi.hpp>

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>


namespace event_model
{
    namespace 
    {
        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;

        using qi::_1;
        using qi::_a;
        using qi::_b;
        using qi::eps;
        using qi::_r1;
        using qi::_r2;
        
        qi::rule<   std::string::const_iterator,
                    void(value_variant_vector &,const table_descriptor &),
                    qi::locals<uint8_t, uint32_t>,
                    ascii::space_type>  
        event_entry( qi::int_
         //   qi::ushort_[_a=_1] > '=' >   
           // (       eps(_a == VALUE_INT4)
             //   |   eps(_a == VALUE_INT8)
               // |   eps(_a == VALUE_STRING)
            //)
        );

        qi::rule<   std::string::const_iterator, 
                    void(value_variant_vector &,const table_descriptor &),
                    qi::locals<uint8_t>,
                    ascii::space_type> 
        event_row_rule(
            qi::ushort_ //> '(' 
//            >   event_entry(_r1,_r2) 
  //          > *(',' > event_entry(_r1,_r2))
    //        > ')'
        );
    }

    Row::Row(const table_descriptor & row_descriptor)
        : td_(row_descriptor), row_data_(row_descriptor.entry().size()) {}
    
    bool Row::operator << (const std::string & str)
    { 
        namespace phoenix = boost::phoenix;

        std::string::const_iterator beg = str.begin();
        std::string::const_iterator end = str.end();

        bool res = qi::phrase_parse(
            beg, end, 
            event_row_rule( phoenix::ref(row_data_),phoenix::cref(td_)), 
            ascii::space);

        return true;
    }
    
    Table::Table(const table_descriptor & row_descriptor)
        : td_(row_descriptor), table_data_() {}
};
