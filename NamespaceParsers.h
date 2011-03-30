#ifndef _EM_NAMESPACE_PARSERS
#define _EM_NAMESPACE_PARSERS
#include "EventModelGrammar.h"
#include <boost/shared_ptr.hpp>

#include <string>

namespace event_model
{
namespace parser
{

typedef boost::shared_ptr<MarshallNamespaceDescriptor> 
    MarshallNamespaceDescriptor_shptr;

class NamespaceFileParser
{
public:
    NamespaceFileParser(const std::string & filename);
    bool CanParse();
    bool Parse();
private:
    const std::string filepath_;
    bool canParse_;
    const char * error_message_;
    MarshallNamespaceDescriptor_shptr mnd_;
};

}
}
#endif 
