#include <event_model/DescriptorsCommon.h>
#include <event_model/DescriptorKarmaGenerators.h>
#include <RCF/Idl.hpp>
#include <RCF/TcpEndpoint.hpp>
#include <interface/RelayToServerInterface.h>

int main()
{
    std::string test;
    std::back_insert_iterator<std::string> bii(test);

    EventDescriptorGenerator<
        std::back_insert_iterator<std::string> > etgg;

    RcfClient<I_NamespaceRepository> MarshNsRepoClient(RCF::TcpEndpoint(50001));
    RelayNamespaceDescriptor mnd
        = MarshNsRepoClient.GetRelayNamespaceDescriptor("baboons");
    
    std::cout << "namespace received.\n" 
              << "the max event ordinal is " << mnd.event_container_size()
              << " and there are " << mnd.event_container_occupied_size() << " ordinals in use\n";

    for(int i=0; i < mnd.event_container_size(); ++i)
    {
      const RelayNamespaceDescriptor::EventDescriptor * ed;
      ed = mnd.EventAt(i);
      if(ed != NULL)
      {
        std::cout << ed->name() << std::endl;
      }
    }
}
