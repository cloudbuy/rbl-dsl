#ifndef _EM_NAMESPACE_PARSERS
#define _EM_NAMESPACE_PARSERS
//#include "detail/EventModelGrammar.h"
#include <parser/Descriptors.h>

#include <string>

namespace event_model
{
namespace parser
{


class NamespaceFileParser
{
public:
    NamespaceFileParser() : 
        filepath_(), canParse_(false), error_message_("none") {}
    NamespaceFileParser(const std::string & filename);
    void SetFile(const std::string & s_in);
    bool CanParse() const;
    bool Parse();
    const char * error() const;
    const  MarshallNamespaceDescriptor_shptr get_descriptor() const ;
private:
    void test_file_();
    std::string filepath_;
    bool canParse_;
    const char * error_message_;
    MarshallNamespaceDescriptor_shptr mnd_;
};

}
}
#endif 
