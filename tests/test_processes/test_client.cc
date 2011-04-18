#include <event_model/Descriptors.h>
#include <event_model/DescriptorKarmaGenerators.h>
#include <RCF/Idl.hpp>
#include <RCF/TcpEndpoint.hpp>
#include <interface/RelayToServerInterface.h>

int main()
{
    std::string test;
    std::back_insert_iterator<std::string> bii(test);

    EventTypeContainerGenerator<
        std::back_insert_iterator<std::string> > etgg;

    RcfClient<I_NamespaceRepository> MarshNsRepoClient(RCF::TcpEndpoint(50001));
    RelayNamespaceDescriptor mnd
        = MarshNsRepoClient.GetRelayNamespaceDescriptor("baboons");
        
}
