#ifndef RBL_EP_EVENT_GEN_SITE_H
#define RBL_EP_EVENT_GEN_SITE_H
#include <map>

namespace rubble { namespace event_processing {

  class EventGenSite;
  class EventToken;

  class FieldToken
  {
  public:
    typedef std::map<const char *, FieldToken> field_token_map_t;

    FieldToken( const char * ft_name, 
                const char * signature, const EventToken & event_token);

    const char * name() const;
    const char * signature() const;
  private:
    const char * m_ft_name;
    const char * m_signature;
  
    bool m_is_variant;
    bool m_is_reference;

    const EventToken * m_referenced_event;

    const EventToken &  m_event_token;
  };

  class EventToken
  {
  public:
    typedef std::map<const char *, EventToken> event_token_map_t;

    EventToken(const char * et_name, const EventGenSite & egs); 
    FieldToken fieldToken(const char * name, const char * signature);
    
    const char * name() const ;
  private:
    const char * m_et_name;
    const EventGenSite & m_event_gen_site;
    FieldToken::field_token_map_t field_token_map;
  };

  class EventGenSite
  {
  public:
    EventGenSite(const char * ns_name);
    EventToken & eventToken(const char * name);

    const char * name() const;
  private:
    const char * m_ns_name;
    EventToken::event_token_map_t event_token_map;
  };

} }
#include "event_gen_site-inl.h"

#endif
