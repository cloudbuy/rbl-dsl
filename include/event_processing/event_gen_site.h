#ifndef RBL_EP_EVENT_GEN_SITE_H
#define RBL_EP_EVENT_GEN_SITE_H
#include "event_model/types.h"
#include <map>
#include <string>

namespace rubble { namespace event_processing {

  class EventGenSite;
  class EventToken;

  class FieldToken
  {
  public:
    typedef std::map<std::string, FieldToken> field_token_map_t;

    FieldToken( const char * ft_name, const char * signature, 
                const EventToken & event_token, const EventGenSite & gen_site);

    const char * name() const;
    const char * signature() const;
    const bool is_variant() const;
    const bool is_reference() const;
    const rubble::event_model::type_ordinal_type type_ordinal() const;

    void set_is_variant(bool is_variant_);
    void set_is_reference(bool is_reference_, const EventToken & event_token);
    void set_type_ordinal(rubble::event_model::type_ordinal_type ordinal);
    
  private:
    const char * m_ft_name;
    const char * m_signature;
 
    rubble::event_model::type_ordinal_type m_type_ordinal;
    bool m_is_variant;
    bool m_is_reference;
    const EventToken * m_referenced_event;

    const EventToken &  m_event_token;
    const EventGenSite & m_gen_site;
  };

  class EventToken
  {
  public:
    typedef std::map<std::string, EventToken> event_token_map_t;

    EventToken(const char * et_name, const EventGenSite & egs); 
    FieldToken fieldToken(const char * name, const char * signature);
    bool FieldExists(const char * name) const;
    const char * name() const ;
  private:
    const char * m_et_name;
    const EventGenSite & m_gen_site;
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
