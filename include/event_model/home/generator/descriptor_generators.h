#ifndef RBL_EM_HOME_DESCRIPTOR_GENERATOR
#define RBL_EM_HOME_DESCRIPTOR_GENERATOR
#include <boost/spirit/include/karma.hpp>
#include "../descriptors/descriptors_common.h"
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/home/phoenix/object/static_cast.hpp>

namespace rubble { namespace event_model {
  namespace karma = boost::spirit::karma;  
  namespace phoenix = boost::phoenix;
  namespace ascii = boost::spirit::ascii; 
/////////////////////////////////////////////////////////////////////////////// 
  template<typename iterator>
  struct des_ns_gen_prim
  {
    karma::rule<iterator, void(const Oid &) >  namespace_header;
    karma::rule<iterator, void()> namespace_footer;
    //karma::rule<iterator,void()> two_indent;
    

    des_ns_gen_prim()
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
        int_(_ORDINAL_OID) << lit(':') << stream(_NAME_OID) <<
        space << lit('{') << eol;
      ;
      namespace_footer = lit('}') << eol;

      //two_indent = one_indent << one_indent;

            
      #undef _ORDINAL_OID
      #undef _NAME_OID
    }
  };
///////////////////////////////////////////////////////////////////////////////
  template<typename iterator>
  struct des_e_gen_prim
  {
    
    
    karma::rule<iterator, void()> one_indent; 
    karma::rule<iterator, void(const Oid &) >  event_header;
    karma::rule<iterator, void()> event_footer;
    
    des_e_gen_prim()
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

      one_indent = space << space;
      
      event_header = 
        one_indent<< lit("event")  << space <<
        int_(_ORDINAL_OID) << lit(':') << stream(_NAME_OID) <<
        space << lit('{') << eol;
      ;
      event_footer = one_indent << lit('}') << eol;
      
      #undef _ORDINAL_OID
      #undef _NAME_OID
    };
  };
///////////////////////////////////////////////////////////////////////////////
  template<typename iterator, typename ns_type>
  struct namespace_generator_grammar
  {
  private:
    namespace_generator_grammar();
  };
///////////////////////////////////////////////////////////////////////////////
  template <typename iterator, typename e_type>
  struct event_generator_grammar
  {
  private:
    event_generator_grammar();
  };
///////////////////////////////////////////////////////////////////////////////
  template<typename iterator>
  struct event_generator_grammar<iterator, EventDescriptorBase>
    : karma::grammar< iterator,
                      void(const EventDescriptorBase *),
                      karma::locals< unsigned int, 
                                     const EventTypeDescriptor *> > 
  {
    des_e_gen_prim<iterator> primitives; 

    karma::rule< iterator, void(const EventDescriptorBase *), 
                 karma::locals< unsigned int, 
                                const EventTypeDescriptor *> > base_rule;
    
    event_generator_grammar() : event_generator_grammar::base_type(base_rule)
    {
      using karma::eps;
      using karma::_r1;
      using karma::int_;
      using karma::eol;
      using karma::repeat;
      using karma::_a;
      using karma::_b;

      #define _E_OID \
      phoenix::bind(&EventDescriptorBase::oid,_r1)

      #define _E_SIZE \
      phoenix::bind(&EventDescriptorBase::type_container_size,_r1)      

      #define _TYPE_AT(i) \
      phoenix::bind(&EventDescriptorBase::TypeAt,_r1,i)

      base_rule = 
        primitives.one_indent << primitives.event_header(_E_OID) <<
        eps[ _a = 0 ] <<
        repeat(_E_SIZE) [
          eps[ _b = _TYPE_AT(_a) ] <<
          ( ( eps(_b) ) | eps) << 
          eps[ _a++ ]
        ] << 
        primitives.one_indent << primitives.event_footer
      ;

      #undef _E_SIZE 
      #undef _E_OID
    }
  };

///////////////////////////////////////////////////////////////////////////////
  template<typename iterator>
  struct namespace_generator_grammar<iterator, BasicNamespaceDescriptor > 
    : karma::grammar< iterator, 
                      void(const BasicNamespaceDescriptor &), 
                      karma::locals< unsigned int, const EventDescriptorBase *> >
  {
    des_ns_gen_prim<iterator> primitives;
    event_generator_grammar<iterator, EventDescriptorBase> event_grammar;
   
    karma::rule<  iterator, 
                  void(const BasicNamespaceDescriptor &), 
                  karma::locals< unsigned int, const EventDescriptorBase *> > base_rule;
 
    
    namespace_generator_grammar() : namespace_generator_grammar::base_type(base_rule)
    {
      using karma::_r1;
      using karma::repeat;
      using karma::eps;
      using karma::_a;
      using karma::_b;
  
      #define _ND_NAME  \
      phoenix::bind(&BasicNamespaceDescriptor::oid, _r1)
    
      #define _ND_SIZE \
      phoenix::bind(&BasicNamespaceDescriptor::event_container_size, _r1)
      
      #define _EVENT_AT(i) \
      phoenix::bind(&BasicNamespaceDescriptor::EventAt,_r1,i)
     
      base_rule = 
        primitives.namespace_header(_ND_NAME) << 
          eps[ _a = 0 ] << 
          repeat(_ND_SIZE) [
            eps [ _b = _EVENT_AT(_a)] <<
            ( ( eps(_b) << event_grammar(_b) ) | eps )  <<
            eps[ _a ++ ]
          ] <<
        primitives.namespace_footer
      ; 

      #undef _ND_NAME
      #undef _ND_SIZE
      #undef _EVENT_AT
    }
  };
///////////////////////////////////////////////////////////////////////////////
} }

#endif
