#ifndef RBL_EM_HOME_RELAY_DESCRIPTORS_H
#define RBL_EM_HOME_RELAY_DESCRIPTORS_H
#include "descriptors_common.h"
#include "generator_descriptors.h"

//TODO centralize the SF forward declarations
namespace SF
{
    class Archive;
}

namespace rubble { namespace event_model { 
// RelayEventDescriptor ///////////////////////////////////////////////////
class RelayEventDescriptor : public GeneratorEventDescriptor
{
public:
  RelayEventDescriptor();
  RelayEventDescriptor (  const Oid & oid, const ordinal_type ordinal_, 
                          const EventTypeContainer & etc);
  friend void serialize(SF::Archive & ar,RelayEventDescriptor & red);
};
//-----------------------------------------------------------------------//
    
// RelayNamespaceDescriptor ///////////////////////////////////////////////
class RelayNamespaceDescriptor : 
  public  NamespaceDescriptorBase<RelayEventDescriptor>
{
public:
  RelayNamespaceDescriptor();
  RelayNamespaceDescriptor(   const  std::string & name_in,
                              const  ordinal_type ordinal_in,
                              const  t_edc & edc_in);
  operator GeneratorNamespaceDescriptor() const;

  friend void serialize(SF::Archive & ar, RelayNamespaceDescriptor & rnd);
};
//-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Inline Definitions                                                        //
//---------------------------------------------------------------------------//
    // RelayEventDescriptor ///////////////////////////////////////////////////
    inline RelayEventDescriptor::RelayEventDescriptor()
      : GeneratorEventDescriptor()
    {
    }
    inline RelayEventDescriptor::
    RelayEventDescriptor ( const Oid & oid,
                           const ordinal_type ordinal_,
                           const EventTypeContainer & etc)
        : GeneratorEventDescriptor(oid,ordinal_, etc) 
    {
    }
    //-----------------------------------------------------------------------//
    // RelayNamespaceDescriptor ///////////////////////////////////////////////
    inline RelayNamespaceDescriptor::RelayNamespaceDescriptor()
        : NamespaceDescriptorBase<RelayEventDescriptor>() 
    {
    }
    inline RelayNamespaceDescriptor::RelayNamespaceDescriptor
        ( const  std::string & name_in, 
          const  ordinal_type ordinal_in, 
          const t_edc & edc_in)
            : NamespaceDescriptorBase<RelayEventDescriptor>(name_in, ordinal_in,edc_in)
    {
    }
    inline RelayNamespaceDescriptor::operator GeneratorNamespaceDescriptor() const
    {
        GeneratorNamespaceDescriptor::t_edc gedc;
        m_events.SlicingPopulate(gedc);
        GeneratorNamespaceDescriptor casted(name(),ordinal(),gedc);
        return casted;    
    }
    //-----------------------------------------------------------------------//
//---------------------------------------------------------------------------//
} }
#endif
