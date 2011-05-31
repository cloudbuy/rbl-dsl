#include <marshall/ServerToRelay.h>
#include <event_model/marshall_parser.h>

using namespace rubble::event_model;

NamespaceRepository::NamespaceRepository()
    : map_() {}

// TODO robust error handling 

void NamespaceRepository::AddNamespaceFromFile(const std::string & namespace_file)
{
    NamespaceFileParser nfp(namespace_file);
    
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
    MarshallNamespaceDescriptor::t_shp mnd_s = nfp.get_descriptor();
    map_[mnd_s->name()] = mnd_s;
}

rubble::event_model::RelayNamespaceDescriptor NamespaceRepository::
GetRelayNamespaceDescriptor(const std::string name) const
{
    map_type_::const_iterator it = map_.find(name);
    if(it == map_.end())
        exit(1);
    else return *(it->second); 
}
