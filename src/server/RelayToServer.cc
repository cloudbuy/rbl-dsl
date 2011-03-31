#include <server/RelayToServer.h>
#include <parser/NamespaceParsers.h>

using namespace event_model;

NamespaceRepository::NamespaceRepository()
    : map_() {}

// TODO robust error handling 

void NamespaceRepository::AddNamespaceFromFile(const std::string & namespace_file)
{
    parser::NamespaceFileParser nfp(namespace_file);
    
    if(nfp.CanParse())
    {
        try
        {
            nfp.Parse();
        }
        catch(std::runtime_error & re)
        {
            std::cout << re.what();
            exit(1); 
        }
    }
    else
    {
        error_ = nfp.error();
        exit(1);
    }
    map_[namespace_file] = nfp.get_descriptor();
}

event_model::RelayNamespaceDescriptor NamespaceRepository::
GetRelayNamespaceDescriptor(const std::string name) const
{
    map_type_::const_iterator it = map_.find(name);
    if(it == map_.end())
        exit(1);
    else return *(it->second); 
}
