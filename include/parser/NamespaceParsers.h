#ifndef _EM_NAMESPACE_PARSERS
#define _EM_NAMESPACE_PARSERS
#include "marshall/Descriptors.h"
#include <boost/shared_ptr.hpp>
#include <string>

namespace event_model
{
namespace parser
{


class NamespaceFileParser
{
public:
    typedef boost::shared_ptr<MarshallNamespaceDescriptor> t_mnd_shp;

    NamespaceFileParser() : 
        filepath_(), canParse_(false), error_message_("none") {}
    NamespaceFileParser(const std::string & filename);
    void SetFile(const std::string & s_in);
    bool CanParse() const;
    bool Parse();
    const char * error() const;
    const  t_mnd_shp get_descriptor() const ;
private:
    void test_file_();
    std::string filepath_;
    bool canParse_;
    const char * error_message_;
    t_mnd_shp m_mnd_shp;
};

}
}
#endif 
