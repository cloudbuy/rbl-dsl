#ifndef     RBL_HOME_EM_OID_CONTAINER_SERIALIZATION_H
#define     RBL_HOME_EM_OID_CONTAINER_SERIALIZATION_H
#include "oid_container.h"

#include <SF/vector.hpp>
#include <SF/string.hpp>
namespace rubble { namespace event_model {  

  template<typename CHAR_,unsigned LENGTH_> 
  void serialize( SF::Archive & ar, 
                  OidConstrainedString<CHAR_, LENGTH_> & oid_string)
  {
    if(ar.isWrite())
    {
      std::string s(oid_string.str_);
      ar & s;        
    }
    else
    {
      std::string s;
      ar & s;
      oid_string.construct_(s);
    }
  }

  template< typename f_str_type, typename f_size_type>
  void serialize(SF::Archive & ar, OidType<f_str_type, f_size_type> & oid_type)
  {
    ar & oid_type.name_ & oid_type.ordinal_;
  }

  template<typename f_identifier_type, typename f_entry_type> 
  void serialize(  SF::Archive & ar,  
              OidContainerEntryType<f_identifier_type, f_entry_type> & entry)
  {
    ar & entry.id_ & entry.entry_; 
  }

  template<typename f_identifier_type, typename f_entry_type>
  void serialize( SF::Archive & ar, 
                  OidContainer<f_identifier_type, f_entry_type> & entry)
  {
    ar & entry.entries_;
    
    if(!ar.isWrite())
    {
      entry.regen_name_index_();
    }
  }
} }
#endif 
