namespace rubble { namespace event_processing {
  // FieldToken ///////////////////////////////////////////////////////////////
  inline FieldToken::FieldToken( const char * ft_name,const char * signature, 
                          const EventToken & event_token)
    : m_ft_name(ft_name),
      m_event_token(event_token),
      m_signature(signature),
      m_is_variant(false),
      m_is_reference(false),
      m_referenced_event(0){}
 
  inline const char * FieldToken::name() const
  {
    return m_ft_name;
  }

  inline const char * FieldToken::signature() const
  {
    return m_signature;
  }
  //-------------------------------------------------------------------------//
  
  // EventToken //////////////////////////////////////////////////////////////
  inline EventToken::EventToken(const char * et_name, const EventGenSite & egs)
    : m_et_name(et_name), m_event_gen_site(egs) {}
  
  inline const char * EventToken::name() const 
  {
    return m_et_name;
  }
  //-------------------------------------------------------------------------//
  
  // EventGenSite /////////////////////////////////////////////////////////////
  inline EventGenSite::EventGenSite(const char * ns_name)
    : m_ns_name(ns_name) {}

  inline const char * EventGenSite::name() const
  {
    return m_ns_name;
  }
  //-------------------------------------------------------------------------//
} }

