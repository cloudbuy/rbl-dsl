#ifndef _SWAT_PARSER_DATA_ENTRY_H
#define _SWAT_PARSER_DATA_ENTRY_H
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "../descriptors/descriptors_common.h"
#include "../descriptors/marshall_descriptor_builders.h"

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

#define RBL_TYPE_HEADER_SPIRIT_PARSING
#include "../types/types.h"
#undef  RBL_TYPE_HEADER_SPIRIT_PARSING
 

namespace rubble { namespace event_model {
 
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
    qi::rule< Iterator, void(Oid &)> ordinal_string_identifier;

    IdentifierRules()
    {
      valid_char_str.name("identifier");
      valid_char_str = + (char_("a","z") | char_("A","Z") | char_('_'));
  
      ordinal_string_identifier.name("ordinal-string identifier");
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
    RblTypes marshall_types; 
    IdentifierRules<Iterator> identifier_rules;    

    qi::rule< Iterator, 
              void( MarshallNamespaceDescriptorBuilder &,
                    MarshallEventDescriptorBuilder &), 
              locals<Oid, EventTypeDescriptor>,
              Skipper> event_type_line;

    qi::rule< Iterator, 
              void(MarshallNamespaceDescriptorBuilder &), 
              locals<Oid, MarshallEventDescriptorBuilder>,
              Skipper > event_descriptor;
    
    qi::rule< Iterator,
              void(MarshallNamespaceDescriptorBuilder &),
              Skipper > namespace_descriptor;

    
    CompoundRules()
    {
      #define SET_QUALIFIER(QUALIFIER) \
      bind(&EventTypeDescriptor::set_qualifier,_b, QUALIFIER) 
      
      #define SET_TYPE_USING_ORDINAL  \
      bind(&EventTypeDescriptor::set_type_using_ordinal,_b,_1)

      #define FINALIZE_ADD_ENTRY \
      bind(&MarshallEventDescriptorBuilder::AddEventType,_r2,_a,_b,_pass)

      #define SET_IS_VARIANT  \
      bind(&EventTypeDescriptor::set_is_variant,_b,phoenix::val(true))
 
      #define SET_REF_EVENT_IDENTIFIER                              \
      bind(&EventTypeDescriptor::set_reference_event_ordinal,_b,    \
      bind(& MarshallNamespaceDescriptorBuilder::                   \
        HasOrdinalWithName,_r1,_1,_pass) )

      event_type_line.name("event type entry line");
      event_type_line =
        eps [bind(&EventTypeDescriptor::set_is_primitive,_b,true)] >> 
        -(    no_case[lit("optional") [SET_QUALIFIER(ENTRY_OPTIONAL) ] ]
            | no_case[lit("required") [SET_QUALIFIER(ENTRY_REQUIRED) ] ] 
        ) >>
        identifier_rules.ordinal_string_identifier(_a) > 
        no_case[ marshall_types [ SET_TYPE_USING_ORDINAL ] ] > 
        ( ( eps(phoenix::bind(&EventTypeDescriptor::is_event,_b)) >
            lit('(') >
              identifier_rules.valid_char_str [ SET_REF_EVENT_IDENTIFIER ] >> 
            lit(')')
            )
          | eps 
        ) >
        -lit("[]") [ SET_IS_VARIANT] >>
        char_(';')[FINALIZE_ADD_ENTRY];
      
      event_descriptor.name("event descriptor"); 
      event_descriptor = 
        no_case[ lit("event")]
        > identifier_rules.ordinal_string_identifier(_a)
            [   bind(&MarshallEventDescriptorBuilder::
                Init,_b,_a,_r1,_pass)]
        > char_('{')
        > *( event_type_line(_r1,_b))
        > char_('}')  [ bind(&MarshallNamespaceDescriptorBuilder::
                        AddEventDescriptor,_r1,_b,_pass) ]
      ;
      
      namespace_descriptor =
        no_case[ lit("namespace")]
        > identifier_rules.valid_char_str 
            [   bind(&MarshallNamespaceDescriptorBuilder::
                    set_name,_r1,_1)]
        > char_('{')
        > *( event_descriptor(_r1))
        > char_('}')
      ;
      #undef SET_REF_EVENT_IDENTIFIER
      #undef SET_IS_VARIANT
      #undef FINALIZE_ADD_ENTRY
      #undef SET_TYPE_USING_ORDINAL
      #undef SET_QUALIFIER
    }
  };
} }
#endif
