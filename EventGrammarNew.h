#ifndef _SWAT_PARSER_DATA_ENTRY_H
#define _SWAT_PARSER_DATA_ENTRY_H

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "Descriptors.h"

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
namespace event_model
{
namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    using qi::int_;
    using ascii::char_;
    using qi::lit;
    using qi::_val;
    using qi::eps;
    using phoenix::at_c;
    using phoenix::push_back;
    using ascii::no_case;
    using qi::_1;
    using ascii::space;
    using namespace qi::labels;
    using qi::lexeme;
    using phoenix::ref;
    using phoenix::val;
    using qi::fail;
    using phoenix::construct;
    using qi::locals;
    using phoenix::bind;
    using qi::eol;
    using qi::_pass;

    struct swat_types_ : qi::symbols<char, unsigned>
    {
        swat_types_()
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

    template <typename Iterator>
        struct skipper : qi::grammar<Iterator>
        {
            skipper() : skipper::base_type(start)
            {
                using boost::spirit::ascii::char_;

                start = ascii::space | lit("//") >> *(ascii::char_ - qi::eol) >> qi::eol;
            }

            qi::rule<Iterator> start;
        };
    
    template<typename Iterator>
    struct Rules
    {
        swat_types_ swat_types;

        qi::rule<   Iterator,   std::string()> data_name;

        qi::rule<   Iterator,   void()> identifier_pair;

        qi::rule<   Iterator,   void()> event_data_line;

        qi::rule<   Iterator, void()> event_descriptor;
                                    

        Rules()
        {
            data_name.name("Data Name Identifier");
            data_name =
               lexeme[ + (char_("a","z") | char_("A","Z") | char_('_'))];

            identifier_pair.name("Data identifier pair (ordinal:name)");

            identifier_pair =
                lexeme[int_  > ':' > data_name];
            
            event_data_line = 
                -(  no_case[lit("optional")]
                    | no_case[lit("required")]
                )
                >> identifier_pair
                > no_case[ swat_types]
                > char_(';');
                
            event_descriptor = 
                no_case[lit("event")]
                > identifier_pair 
                > char_('{')
                > *(event_data_line)
                > char_('}')
            ;
        }
    };
}
}
#endif
