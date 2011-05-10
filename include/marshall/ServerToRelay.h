#include "interface/RelayToServerInterface.h"
#include "marshall/Descriptors.h"
#include <map>

class NamespaceRepository
{
public:
    NamespaceRepository();
    const char * error() const;
    void AddNamespaceFromFile(const std::string & namespace_file);
    event_model::RelayNamespaceDescriptor GetRelayNamespaceDescriptor
        (const std::string name) const;
private:
    const char * error_;
    
    typedef std::map<
        std::string, 
        event_model::MarshallNamespaceDescriptor::t_shp> map_type_;

    map_type_ map_;
};
