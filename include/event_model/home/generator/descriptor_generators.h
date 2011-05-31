#ifndef RBL_EM_HOME_DESCRIPTOR_GENERATOR
#define RBL_EM_HOME_DESCRIPTOR_GENERATOR
#include <boost/spirit/include/karma.hpp>
#include "../descriptors/descriptors_common.h"
#include <boost/spirit/include/phoenix_bind.hpp>

namespace rubble { namespace event_model {
  namespace karma = boost::spirit::karma;  
  namespace phoenix = boost::phoenix;
  namespace ascii = boost::spirit::ascii; 
/////////////////////////////////////////////////////////////////////////////// 
  template<typename iterator>
  struct descriptor_generator_primitives
  {
    karma::rule<iterator, void(const Oid &) >  namespace_header;
    karma::rule<iterator, void()> namespace_footer;
    karma::rule<iterator, void()> one_indent; 
    karma::rule<iterator,void()> two_indent;
    karma::rule<iterator, void(const Oid &) >  event_header;
    karma::rule<iterator, void()> event_footer;


    descriptor_generator_primitives()
    {
      using karma::lit;
      using karma::int_;
      using karma::stream;
      using karma::_r1;
      using karma::eol;
      using ascii::space;

      #define _ORDINAL_OID                      \
      phoenix::bind(&Oid::ordinal, _r1)
      #define _NAME_OID                         \
      phoenix::bind(&Oid::name,_r1)
      
      namespace_header = 
        lit("namespace") << space <<
        int_(_ORDINAL_OID) << space << lit(':') << stream(_NAME_OID) <<
        space << lit('{') << eol;
      ;
      namespace_footer = lit('}') << eol;

      one_indent = space << space;
      two_indent = one_indent << one_indent;

      event_header = 
        one_indent<< lit("event")  << space <<
        int_(_ORDINAL_OID) << space << lit(':') << stream(_NAME_OID) <<
        space << lit('{') << eol;
      ;
      event_footer = one_indent << eol << eol;
      
      #undef _ORDINAL_OID
      #undef _NAME_OID
    }
  };
///////////////////////////////////////////////////////////////////////////////
  template<typename iterator, typename ns_type>
  struct namespace_grammar
  {
  private:
    namespace_grammar();
  };
///////////////////////////////////////////////////////////////////////////////
  template<typename iterator>
  struct namespace_grammar<iterator, BasicNamespaceDescriptor > 
    : karma::grammar<iterator, void(const BasicNamespaceDescriptor &) >
  {
    descriptor_generator_primitives<iterator> primitives;
    
    karma::rule<iterator, void(const BasicNamespaceDescriptor &) > base_rule;
 
    
 
    namespace_grammar() : namespace_grammar::base_type(base_rule)
    {
       
    }
  };

} }

#endif
