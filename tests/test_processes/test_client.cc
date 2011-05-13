#include "event_model/DescriptorsCommon.h"
#include "event_model/DescriptorKarmaGenerators.h"
#include <RCF/TcpEndpoint.hpp>
#include <interface/RelayToServerInterface.h>
#include <string>

using namespace event_model;

int main()
{
    std::string test;
    std::back_insert_iterator<std::string> bii(test);

    RcfClient<I_NamespaceRepository> MarshNsRepoClient(RCF::TcpEndpoint(50001));
    RelayNamespaceDescriptor mnd
        = MarshNsRepoClient.GetRelayNamespaceDescriptor("baboons");
    
    std::cout << "namespace received.\n" 
              << "the max event ordinal is " << mnd.event_container_size()
              << " and there are " << mnd.event_container_occupied_size() << " ordinals in use\n";

    bool res;
    BaseEventDescriptorGenerator edg;  

    for(int i=0; i < mnd.event_container_size(); ++i)
    {
      const RelayNamespaceDescriptor::EventDescriptor * ed;

      ed = mnd.EventAt(i);
      if(ed != NULL)
      {
        res = boost::spirit::karma::generate(bii,edg(phoenix::ref(*ed)));
      }
    }
    std::cout << test;
}
