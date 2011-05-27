#ifndef _EM_NAMESPACE_PARSERS
#define _EM_NAMESPACE_PARSERS
#include "../descriptors/marshall_descriptor_builders.h"
#include <boost/shared_ptr.hpp>
#include <string>

namespace rubble { namespace  event_model { 


class NamespaceFileParser
{
public:
    typedef boost::shared_ptr<MarshallNamespaceDescriptor> t_mnd_shp;

    NamespaceFileParser();
        
    NamespaceFileParser(const std::string & filename);

    bool              CanParse()       const;
    const char *      error()          const;
    const t_mnd_shp   get_descriptor() const;

    bool Parse();
    void SetFile(const std::string & s_in);
private:
    void test_file_();
    std::string filepath_;
    bool canParse_;
    const char * error_message_;
    
    t_mnd_shp m_mnd_shp;
};

} } 
#endif 
