#include "event_processing/event_gen_site.h"

#define RBL_TYPE_HEADER_SPIRIT_PARSING
#include "event_model/types.h"
#undef RBL_TYPE_HEADER_SPIRIT_PARSING

#include "event_processing/event_gen_site.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

#include <string>
#include <boost/algorithm/string/case_conv.hpp>
#include <ostream>


namespace rubble { namespace event_processing {

  // FieldToken ///////////////////////////////////////////////////////////////
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  namespace phoenix = boost::phoenix;

  template<typename Iterator>
  struct type_signature_grammar
    : qi::grammar< Iterator, void(FieldToken &)>
  {
    qi::rule<Iterator, void(FieldToken &) > base_rule;
    qi::rule< Iterator,   std::string()> valid_char_str;
    rubble::event_model::RblTypes RblTypes;
     
    type_signature_grammar() : type_signature_grammar::base_type(base_rule)
    {
      using qi::lit;
      using qi::_r1;
      using qi::_1;    
      using qi::eps;
      using qi::char_;
 
      valid_char_str = + (char_("a","z") | char_("A","Z") | char_('_')); 

      base_rule = 
        RblTypes [phoenix::bind(&FieldToken::set_type_ordinal,_r1,_1)] >>
        ( ( eps(phoenix::bind(&FieldToken::is_event,_r1)) >
              lit('(') >>
              valid_char_str  
                [phoenix::bind(&FieldToken::set_ref_event_token, _r1, _1)] >> 
              lit(')')
            ) 
          | eps ) >>
        -(lit("[]")[phoenix::bind(&FieldToken::set_is_variant,_r1, phoenix::val(true))])
      ;
    }
  };

  FieldToken::FieldToken( const char * ft_name,const char * signature, 
                          const EventToken & event_token, 
                          const EventGenSite & gen_site)
    : m_ft_name(ft_name),
      m_event_token(event_token),
      m_signature(signature),
      m_gen_site(gen_site),
      m_is_variant(false),
      m_event_reference(0)
  {
    std::string signature_str(signature);
    typedef std::string::const_iterator c_it;
   
    c_it beg = signature_str.begin(); 
    c_it end = signature_str.end();
    
    type_signature_grammar<c_it> grammar;
    bool res = qi::parse(beg,end,grammar(phoenix::ref(*this)));

    if(res == false)
      throw "signature parsing error";
  }
  //-------------------------------------------------------------------------//  
  // EventToken ///////////////////////////////////////////////////////////////
  FieldToken EventToken::fieldToken(const char * name, const char * signature)
  {
    std::string key_type(name);
    boost::to_lower(key_type);

    std::pair<std::string, FieldToken> insertee
      (key_type, FieldToken(name, signature, *this, m_gen_site));

    std::pair<FieldToken::field_token_map_t::iterator, bool> ret
      = field_token_map.insert(insertee);

    if(ret.second)
      return (ret.first->second);
    else
    {
      std::ostringstream os;
      os << "Field: \"" << name << "\" allready exists in this event";
      throw GenSiteException(os.str());
    }
  }
  //-------------------------------------------------------------------------//

  // EventGenSite /////////////////////////////////////////////////////////////
  EventToken & EventGenSite::eventToken(const char * name)
  {
    std::string key_type(name);
    boost::to_lower(key_type);
  
    std::pair<std::string, EventToken> insertee(key_type, EventToken(name,*this));

    std::pair<EventToken::event_token_map_t::iterator, bool> ret
      = event_token_map.insert(insertee);
    
    if(ret.second)
      return (ret.first->second);
    else
    {
      std::ostringstream os;
      os << "Field: \"" << name << "\" allready exists in this event";
      throw GenSiteException(os.str());
    }
  }
  
  const EventToken & EventGenSite::getEventToken(std::string name) const
  {
    boost::to_lower(name);   
 
    EventToken::event_token_map_t::const_iterator it = 
      event_token_map.find(name);
    
    if(it == event_token_map.end())
    {
      std::ostringstream os;
      os << "Event: \"" << name << "\" Does not exist in this Gen Site";
      
      throw GenSiteException(os.str());
    }
    else
      return it->second;
  }
  //-------------------------------------------------------------------------//

} }

