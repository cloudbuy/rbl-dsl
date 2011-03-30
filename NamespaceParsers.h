#ifndef _EM_NAMESPACE_PARSERS
#define _EM_NAMESPACE_PARSERS

class NamespaceFileParser
{
public:
    NamespaceParser(const std::string & filename);
    bool CanParse();
    bool Parse();
    NamespaceDescriptor_shptr GetNamespaceDescriptor();

private:
    const std::string _filepath;
    bool canParse_;
    const char * error_message_;
};

#endif 
