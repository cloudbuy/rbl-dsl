namespace rubble { namespace event_processing {
  // FieldToken ///////////////////////////////////////////////////////////////
  inline const char * FieldToken::name() const
  {
    return m_ft_name;
  }

  inline const char * FieldToken::signature() const
  {
    return m_signature;
  }

  inline const bool FieldToken::is_variant() const
  {
    return m_is_variant;
  }
  inline const bool FieldToken::is_reference() const
  {
    return m_is_reference;
  }
  inline const rubble::event_model::type_ordinal_type 
  FieldToken::type_ordinal() const
  {
    return m_type_ordinal; 
  }
  inline void FieldToken::set_is_variant(bool is_variant_)
  {
    m_is_variant = is_variant_;
  }

  inline void FieldToken::set_is_reference
    (bool is_reference_, const EventToken & event_token)
  {
    m_is_reference = is_reference_;
    m_referenced_event = & event_token;
  }
  
  inline void FieldToken::set_type_ordinal
  (rubble::event_model::type_ordinal_type ordinal)
  {
    m_type_ordinal = ordinal;
  }
  //-------------------------------------------------------------------------//
  
  // EventToken //////////////////////////////////////////////////////////////
  inline EventToken::EventToken(const char * et_name, const EventGenSite & egs)
    : m_et_name(et_name), m_gen_site(egs) {}
  
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

