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
                ("int", SWAT_INT4)
                ("int4", SWAT_INT4)
                ("int8", SWAT_INT8)
                ("bigint", SWAT_INT8)
                ("string", SWAT_STRING)
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

        qi::rule<   Iterator,   void(oid_identifier &)> identifier_pair;

        qi::rule<   Iterator,   void(EventDescriptorBuilder &), 
            locals< EventDescriptorData,oid_identifier> > event_data_line;

        qi::rule<   Iterator, void(NamespaceDescriptorBuilder &),
            locals< EventDescriptorBuilder, oid_identifier> > event_descriptor;
                                    

        Rules()
        {
            data_name.name("Data Name Identifier");
            data_name =
               lexeme[ + (char_("a","z") | char_("A","Z") | char_('_'))];

            identifier_pair.name("Data identifier pair (ordinal:name)");

            identifier_pair =
                lexeme[int_ [ bind(&oid_identifier::ordinal,_r1) = _1] 
                > ':' > data_name [ bind(&oid_identifier::key,_r1) = _1]];
            
            event_data_line = 
                -(  no_case[lit("optional")]
                        [bind(&EventDescriptorData::qualifier,_a)= ENTRY_OPTIONAL] 
                    | no_case[lit("required")]
                        [bind(&EventDescriptorData::qualifier, _a) = ENTRY_REQUIRED]
                )
                >> identifier_pair(_b)
                > no_case[ swat_types [bind(&EventDescriptorData::type,_a) = _1]]
                > char_(';') [ bind( &EventDescriptorBuilder::AddEventDescriptorLine, _r1,_b,_a,_pass) ];
                
            event_descriptor = 
                no_case[lit("event")]
                > identifier_pair(_b) [ bind(&EventDescriptorBuilder::Init,_a,_b,_r1,_pass) ]
                > char_('{')
                > *(event_data_line(_a))
                > char_('}')
            ;
        }
    };
}
}
#endif
