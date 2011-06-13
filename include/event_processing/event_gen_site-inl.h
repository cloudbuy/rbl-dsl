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
  inline const bool FieldToken::is_event() const
  {
    if(m_type_ordinal == 
        rubble::event_model::get_type_ordinal_f<rubble::event_model::rbl_event>::pos::value)
      return true;
    else
      return false;
  }
  inline const rubble::event_model::type_ordinal_type 
  FieldToken::type_ordinal() const
  {
    return m_type_ordinal; 
  }

  inline const EventToken & FieldToken::get_ref_event_token() const
  {
    if(m_event_reference == 0)
      throw "Object is not an event";
    else
      return *m_event_reference;
  }


  inline void FieldToken::set_is_variant(bool is_variant_)
  {
    m_is_variant = is_variant_;
  }

  inline void FieldToken::set_type_ordinal
  (rubble::event_model::type_ordinal_type ordinal)
  {
    m_type_ordinal = ordinal;
  }

  inline const EventToken & 
  FieldToken::set_ref_event_token(std::string & name)
  {
    m_event_reference = &(m_gen_site.getEventToken(name));
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

