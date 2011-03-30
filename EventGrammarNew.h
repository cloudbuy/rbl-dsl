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
    using qi::eol;
    using qi::eoi;
    using qi::omit;
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

    struct MarshallTypes : qi::symbols<char, VALUE_TYPE>
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
    struct IdentifierRules
    {
        qi::rule< Iterator,   std::string()> valid_char_str;
        qi::rule< Iterator, std::string() > string_identifier;
        qi::rule< Iterator, void(Oid &)> ordinal_string_identifier;

        IdentifierRules()
        {
            valid_char_str.name("Data Name Identifier");
            valid_char_str = + (char_("a","z") | char_("A","Z") | char_('_'));
            
            string_identifier = valid_char_str;
            
            ordinal_string_identifier = 
                int_[bind(&Oid::set_ordinal,_r1,_1)] 
                > ':' 
                > valid_char_str[bind(&Oid::set_name,_r1,_1)]
            ; 
        } 
    };

    template<typename Iterator, typename Skipper>
    struct CompoundRules
    {
        MarshallTypes marshall_types; 
        IdentifierRules<Iterator> identifier_rules;    
    
        qi::rule<   Iterator, 
                    void(MarshallEventDescriptorBuilder &), 
                    locals<Oid, EventTypeDescriptor>,
                    Skipper> event_type_line;

        qi::rule<   Iterator, 
                    void(MarshallNamespaceDescriptorBuilder &), 
                    locals<Oid, MarshallEventDescriptorBuilder>,
                    Skipper> event_descriptor;

        
        CompoundRules()
        { 
            event_type_line =
                eps [bind(&EventTypeDescriptor::set_is_primitive,_b,true)] 
                >> -(  no_case[lit("optional")[bind(&EventTypeDescriptor::set_qualifier,_b,ENTRY_OPTIONAL) ]]
                        | no_case[lit("required")[bind(&EventTypeDescriptor::set_qualifier,_b,ENTRY_REQUIRED)]]
                    )
                >> identifier_rules.ordinal_string_identifier(_a)
                > no_case[ marshall_types [bind(&EventTypeDescriptor::set_type,_b,_1)] ]
                > char_(';')[bind(&MarshallEventDescriptorBuilder::AddEventType,_r1,_a,_b,_pass)];
       
            event_descriptor = 
                    no_case[ lit("event")]
                >   identifier_rules.ordinal_string_identifier(_a)
                        [   bind(&MarshallEventDescriptorBuilder::
                                Init,_b,_a,_r1,_pass)]
                >   char_('{')
                >   *( event_type_line(_b))
                >   char_('}')  [   bind(&MarshallNamespaceDescriptorBuilder::
                                        AddEventDescriptor,_r1,_b,_pass) ]
            ;
        }
    };

    /* 
    template<typename Iterator>
    struct Rules
    {
        
        swat_types_ swat_types;

        
        
        qi::rule<   Iterator,   void(),ascii::space_type> event_data_line;

        qi::rule<   Iterator, void()> event_descriptor;
                                    

        Rules()
        {
                        
        

            identifier_pair.name("Data identifier pair (ordinal:name)");
            identifier_pair =
                int_  > ':' > valid_name;
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
    */
}
}
#endif
