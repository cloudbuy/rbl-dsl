#ifndef RBL_EM_HOME_DESCRIPTOR_GENERATOR
#define RBL_EM_HOME_DESCRIPTOR_GENERATOR
#include <boost/spirit/include/karma.hpp>
#include "../descriptors/descriptors_common.h"
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/home/phoenix/object/static_cast.hpp>
#include <boost/spirit/home/phoenix/object/const_cast.hpp>

#define RBL_TYPE_HEADER_SPIRIT_PARSING
#include "../types/types.h"
#undef RBL_TYPE_HEADER_SPIRIT_PARSING
 

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
        stream(_NAME_OID) <<
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
        one_indent << lit("event")  << space <<
        int_(_ORDINAL_OID) << lit(':') << stream(_NAME_OID) <<
        space << lit('{') << eol;
      ;
      event_footer = one_indent << lit('}') << eol;
      
      #undef _ORDINAL_OID
      #undef _NAME_OID
    };
  };
///////////////////////////////////////////////////////////////////////////////
  template <typename iterator, typename NamespaceDescriptor, typename e_type>
  struct event_generator_grammar
  {
  private:
    event_generator_grammar();
  };
///////////////////////////////////////////////////////////////////////////////
  template<typename iterator,typename NamespaceDescriptor>
  struct type_generator_grammar 
    : karma::grammar< iterator,
                      void( const Oid * ,
                            const EventTypeDescriptor *,
                            const NamespaceDescriptor &)
                      , karma::locals<unsigned int, type_ordinal_type> >
  {
    karma::rule<iterator, void( const Oid *, 
                                const EventTypeDescriptor *,
                                const NamespaceDescriptor &), 
                                karma::locals<unsigned int, type_ordinal_type> > 
                                  base_rule;

    karma::rule<iterator, void()> two_indent;

    OrdinalToTypeST type_symbols;

    type_generator_grammar() : type_generator_grammar::base_type(base_rule)
    {
      using karma::eps;
      using ascii::space;
      using karma::eol;     
      using karma::stream;
      using karma::int_;
      using karma::lit;
      using karma::_r1;
      using karma::_a;
      using karma::_r2;
      using karma::_r3;
      using karma::_b;
      using karma::_1; 

      #define _T_QUALIFIER \
      phoenix::bind(&EventTypeDescriptor::qualifier,_r2)
    
      #define _ORDINAL_OID                      \
      phoenix::bind(&Oid::ordinal, _r1)
      
      #define _NAME_OID                         \
      phoenix::bind(&Oid::name,_r1)

      #define _TYPE_ORDINAL \
      phoenix::bind(&EventTypeDescriptor::type,_r2)
    
      #define _IS_EVENT \
      phoenix::bind(&EventTypeDescriptor::is_event,_r2)

      #define _REFERENCE_EVENT_NAME \
      phoenix::bind(&NamespaceDescriptor::EventDescriptor::name,          \
      phoenix::bind(&NamespaceDescriptor::EventAt,_r3,                    \
      phoenix::bind(&EventTypeDescriptor::reference_event_ordinal,_r2)))  \

      two_indent = space << space << space << space;
      
      base_rule = two_indent <<
        eps[_a = _T_QUALIFIER] <<
        (   ( eps( _a == ENTRY_REQUIRED ) << lit("REQUIRED")  ) 
          | ( eps( _a == ENTRY_OPTIONAL ) << lit("OPTIONAL")  )
          | ( eps                                             ) 
        ) << space <<
        int_(_ORDINAL_OID) << lit(':') << stream(_NAME_OID) << space <<
        type_symbols[ _1 = _TYPE_ORDINAL] <<
        ( ( eps(_IS_EVENT) << lit('(') << stream(_REFERENCE_EVENT_NAME) << lit(')') )
          | eps ) <<
        lit(';') << eol
      ;

      #undef _REFERENCE_EVENT_NAME
      #undef _T_QUALIFIER 
      #undef _ORDINAL_OID
      #undef _NAME_OID
      #undef _TYPE_ORDINAL 
    }
  };
///////////////////////////////////////////////////////////////////////////////
  template<typename iterator,typename NamespaceDescriptor>
  struct event_generator_grammar< iterator,
                                  NamespaceDescriptor, EventDescriptorBase>
    : karma::grammar< iterator,
                      void( const EventDescriptorBase *,
                            const NamespaceDescriptor &),
                      karma::locals< unsigned int, 
                                     const EventTypeDescriptor *> > 
  {
    des_e_gen_prim<iterator> primitives; 
      
    type_generator_grammar<iterator,NamespaceDescriptor> type_grammar;
  
    karma::rule< iterator, void(  const EventDescriptorBase *,
                                  const NamespaceDescriptor &), 
                 karma::locals< unsigned int, 
                                const EventTypeDescriptor *> > base_rule;
    
    event_generator_grammar() : event_generator_grammar::base_type(base_rule)
    {
      using karma::eps;
      using karma::_r1;
      using karma::_r2;
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
      
      #define _TYPE_OID_AT(i) \
      phoenix::bind(&EventDescriptorBase::TypeOidAt,_r1,i)

      base_rule = 
        primitives.event_header(_E_OID) <<
        eps[ _a = 0 ] <<
        repeat(_E_SIZE) [
          eps[ _b = _TYPE_AT(_a) ] <<
          ( ( eps(_b) << type_grammar(_TYPE_OID_AT(_a) , _b,_r2)  ) | eps) << 
          eps[ _a++ ]
        ] << 
        primitives.event_footer
      ;

      #undef _E_SIZE 
      #undef _E_OID
      #undef _TYPE_AT
      #undef _TYPE_OID_AT
    }
  };

///////////////////////////////////////////////////////////////////////////////
  template<typename iterator, typename NamespaceDescriptor>
  struct namespace_generator_grammar
    : karma::grammar< iterator, 
                      void(const NamespaceDescriptor &), 
                      karma::locals< unsigned int, const EventDescriptorBase *> >
  {
    des_ns_gen_prim<iterator> primitives;
    event_generator_grammar<iterator, NamespaceDescriptor ,EventDescriptorBase> event_grammar;
   
    karma::rule<  iterator, 
                  void(const NamespaceDescriptor &), 
                  karma::locals< unsigned int, const EventDescriptorBase *> > base_rule;
 
    
    namespace_generator_grammar() : namespace_generator_grammar::base_type(base_rule)
    {
      using karma::_r1;
      using karma::repeat;
      using karma::eps;
      using karma::_a;
      using karma::_b;
  
      #define _ND_NAME  \
      phoenix::bind(&NamespaceDescriptor::oid, _r1)
    
      #define _ND_SIZE \
      phoenix::bind(&NamespaceDescriptor::event_container_size, _r1)
      
      #define _EVENT_AT(i) \
      phoenix::bind(&NamespaceDescriptor::EventAt,_r1,i)
     
      base_rule = 
        primitives.namespace_header(_ND_NAME) << 
          eps[ _a = 0 ] << 
          repeat(_ND_SIZE) [
            eps [ _b = _EVENT_AT(_a)] <<
            ( ( eps(_b) << event_grammar(_b,_r1) ) | eps )  <<
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
