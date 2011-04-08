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
    
    template<typename MATCHED_TYPE> 
    class type_test : boost::static_visitor<>
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

    struct row_parse_grammar : 
        boost::spirit::qi::grammar< std::string::const_iterator, 
            void(value_variant_vector &,const table_descriptor &),
            boost::spirit::qi::ascii::space_type> 
    {
        value_variant * GetValuePtrAt (
            value_variant_vector & v, 
            uint32_t ordinal, 
            bool & ok_)
        {
            ok_ = true;                                                     
            if( ordinal < v.size() ) 
            {                                      
                if(boost::apply_visitor(type_test<undefined>(), v[ordinal]))
                {                                                           
                    ok_ = true;       
                    std::cout << "good " << &v[ordinal] << std::endl;
                    return &v[ordinal];
                }
                else
                {                                                       
                    double_assignment = true;
                    ok_ = false;
                    std::cout << "1 null" << std::endl;
                    return NULL;
                }                                                           
            }                                                               
            else 
            {                                                          
                out_of_range = true;                                        
                ok_ = false;
                std::cout << "2 null" << std::endl;
                return NULL;            
            }                                                               
        }

        boost::spirit::qi::int_parser< int32_t,  10, 1,10>  p_int32_t;
        boost::spirit::qi::int_parser< int64_t, 10, 1, 19>  p_int64_t;
            
        boost::spirit::qi::rule<   std::string::const_iterator,
                    void(value_variant_vector &, const table_descriptor &),
                    boost::spirit::ascii::space_type>  row_entry;

        boost::spirit::qi::rule<   std::string::const_iterator, 
                    void(value_variant_vector &, const table_descriptor &),
                    boost::spirit::ascii::space_type> row_rule;

        uint16_t current_ordinal;
        uint16_t current_value_type;
        bool was_parsing_value;

        bool has_error;
        bool double_assignment;

        bool out_of_range; 
        row_parse_grammar();
    };

    }
#endif
