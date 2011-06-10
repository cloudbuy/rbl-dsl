#include "event_processing/event_gen_site.h"

namespace rubble { namespace event_processing {

    
  // EventToken /////////////////////////////////////////////////////////////// 
  FieldToken EventToken::fieldToken(const char * name, const char * signature)
  {
    std::pair<const char *, FieldToken> insertee
      (name, FieldToken(name, signature, *this));

    std::pair<FieldToken::field_token_map_t::iterator, bool> ret
      = field_token_map.insert(insertee);

    if(ret.second)
      return (ret.first->second);
    else
      ;
  }
  //-------------------------------------------------------------------------//

  // EventGenSite /////////////////////////////////////////////////////////////
  EventToken & EventGenSite::eventToken(const char * name)
  {
    std::pair<const char *, EventToken> insertee(name, EventToken(name,*this));

    std::pair<EventToken::event_token_map_t::iterator, bool> ret
      = event_token_map.insert(insertee);
    
    if(ret.second)
      return (ret.first->second);
    else
      ;
  }
  //-------------------------------------------------------------------------//

} }

