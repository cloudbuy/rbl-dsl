#include "event_model/marshall_parser.h"
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace rubble { namespace event_model { 

    NamespaceFileParser::NamespaceFileParser()
      : filepath_(), canParse_(false), error_message_("none") {}

    NamespaceFileParser::NamespaceFileParser(const std::string & filename)
            : canParse_(true), error_message_("none"), filepath_(filename)
    {
        test_file_();
    }
    void NamespaceFileParser::SetFile(const std::string & s_in)
    {
        filepath_ = s_in;
        canParse_ = true; 
        test_file_();
    }
    void NamespaceFileParser::test_file_()
    {
         boost::filesystem::path p(filepath_);
        
        if(! boost::filesystem::exists(p) )
        {
            canParse_= false;
            error_message_ = "file does not exist" ;
            return;
        }

    }

    bool NamespaceFileParser::CanParse() const
    {
        return canParse_;
    }
    
    bool NamespaceFileParser::Parse()
    {
        std::ifstream input(filepath_.c_str());
        typedef std::istreambuf_iterator<char> base_iterator_type;
        base_iterator_type in_begin(input);

       typedef boost::spirit::multi_pass<base_iterator_type>
            forward_iterator_type;
        forward_iterator_type fwd_begin =
            boost::spirit::make_default_multi_pass(in_begin);
        forward_iterator_type fwd_end;

        namespace classic = boost::spirit::classic;

        typedef classic::position_iterator2<forward_iterator_type>
            pos_iterator_type;
        pos_iterator_type position_begin(fwd_begin, fwd_end, filepath_);
        pos_iterator_type position_end;
       
        typedef skipper<pos_iterator_type> skipper_type;
        skipper_type skipper;
    
        CompoundRules< pos_iterator_type,skipper_type > 
            namespace_compound_rules;
         
        using boost::phoenix::ref;
        using boost::spirit::qi::expectation_failure;
        namespace ascii = boost::spirit::ascii;
        namespace qi = boost::spirit::qi;
      
        MarshallNamespaceDescriptorBuilder mndb;        
        try
        {
            qi::phrase_parse(
                position_begin, position_end,
                namespace_compound_rules.namespace_descriptor(ref(mndb)),
                skipper
            );
        
            m_mnd_shp.reset(new MarshallNamespaceDescriptor(
                mndb.name(), mndb.ordinal(), mndb.events() ) );   
        }
        catch (const expectation_failure<pos_iterator_type>& e)
        {
            
            const classic::file_position_base<std::string>& pos =
                e.first.get_position();
            std::stringstream msg;
/*            if(_creator.HasSpecificationError())
                msg <<
                 //_creator.ErrorMsg() <<
                "\nError at : "<< pos.file << " line " << pos.line
                << " column " << pos.column << std::endl << "'"
                << e.first.get_currentline() << "'" << std::endl
                << std::setw(pos.column) << " " << "^- here";

            else*/
            msg << "parse error at file " << pos.file << " line " << pos.line
                << " column " << pos.column << std::endl << "'"
                << e.first.get_currentline() << "'" << std::endl
                << std::setw(pos.column) << " " << "^- here" <<
                " - expectation: " << e.what_ << std::endl;

            
            throw std::runtime_error(msg.str());
        }

        return true;
    }

    const char * NamespaceFileParser::error() const 
    {
        return error_message_;
    }

    const  NamespaceFileParser::t_mnd_shp 
    NamespaceFileParser::get_descriptor() const
    {
        return m_mnd_shp;
    }
} } 
