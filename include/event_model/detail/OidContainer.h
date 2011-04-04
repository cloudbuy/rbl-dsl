#ifndef     _EM_OID_CONTAINER_H
#define     _EM_OID_CONTAINER_H
#include <string>
#include <stdexcept>
#include <string.h>
#include <vector>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/noncopyable.hpp>

#include <SF/vector.hpp>
#include <SF/string.hpp>

namespace event_model
{
namespace primitives
{

// uninstantiatable generic template
template<typename char_type, unsigned length>
class OidConstrainedString
{
private:
    OidConstrainedString();
};

// instantiatable specialization
template<unsigned length>
class OidConstrainedString<char,length> 
{
public:
    inline OidConstrainedString()
    {
        str_[0]='\0';
    };
   
    inline OidConstrainedString(const char * char_)
    {
        std::string str(char_);
        construct_(str); 
    };

    inline OidConstrainedString(const std::string & str)
    {
        construct_(str);
    };

    inline const char * c_str() const 
    {
        if( ! is_initialized() )
            throw std::invalid_argument("the string is uninitialized");
         
        return str_;
    };
    inline bool is_initialized() const
    {
        return (str_[0]=='\0') ? false : true;
    }

    inline bool 
    operator< (const OidConstrainedString<char,length> & rhs) const
    {
        return (strncmp(str_, rhs.c_str(),length) < 0) ? true : false;
    }
    inline bool 
    operator== (const OidConstrainedString<char,length> & rhs) const
    {
        return (strncmp(str_,rhs.c_str(),length) == 0) ? true : false;
    }
    inline bool 
    operator> (const OidConstrainedString<char,length> & rhs) const
    {
        return (strncmp(str_,rhs.c_str(),length) > 0) ? true : false;
    }
    
    void serialize(SF::Archive & ar)
    {
        if(ar.isWrite())
        {
            std::string s(str_);
            ar & s;        
        }
        else
        {
            std::string s;
            ar & s;
            construct_(s);
        }
    }
    
private:
    void construct_(const std::string & str)
    {
        if(str.length()+1 > length)
            throw std::length_error
                ("identifier string larger than identifier limit");
        std::string str2(str);
        boost::to_lower(str2);

        strncpy(str_, str2.c_str(), length);
    }

    char str_[length];
};

template<unsigned length>
std::ostream & operator<< (std::ostream & os, const OidConstrainedString<char,length> & oid)
{
    os << oid.c_str();
    return os; 
}

template< typename str_type, typename size_type>
class OidType
{
    const static size_type MAX = boost::integer_traits<size_type>::const_max;
    BOOST_STATIC_ASSERT(boost::integer_traits<size_type>::is_integer == true);

public:
    typedef str_type name_type;
    typedef size_type ordinal_type;    

    inline OidType()
        : name_(),ordinal_() {}
     
    explicit inline OidType( const std::string & str_in, 
                             const boost::uint32_t ordinal_in)
        : name_(str_in), ordinal_() 
    { 
        if( ordinal_in > MAX )
            throw std::out_of_range("ordinal exceeds limit");

        ordinal_ = ordinal_in;
    }
 
    void serialize(SF::Archive & ar)
    {
        ar & name_ & ordinal_;
    }
    
    inline bool operator==(const OidType<str_type,size_type> & rhs) const
    {
        if( rhs.name_ == this->name_
            && rhs.ordinal_ == this->ordinal_)
            return true;
        else
            return false; 
    }
    inline const str_type & name() const { return name_; }
    inline const size_type ordinal() const { return ordinal_; }

    inline void set_name(const name_type & name_in) 
    {
        name_ = name_in;    
    }
    inline void set_ordinal(const size_type & ordinal_in)
    {
        ordinal_  = ordinal_in;
    }
private:
    str_type name_;
    size_type ordinal_;
};

namespace
{
    namespace intrusive = boost::intrusive;
}

template<typename identifier_type, typename _entry_type>
class OidContainerEntryType: 
    public intrusive::set_base_hook<>
{
public:
    typedef _entry_type basic_entry_type;
    inline OidContainerEntryType()
        : id_(), entry_()
    {}    
    explicit inline 
    OidContainerEntryType(const identifier_type & id, const _entry_type & et)
        : id_(id), entry_(et) {}

    inline bool is_initialized() const { return id_.name().is_initialized(); }
    
    inline bool operator< 
        (const OidContainerEntryType<identifier_type,_entry_type> & rhs) const
    { return id_.name() < rhs.id_.name() ; }
   
    inline bool operator== 
        (const OidContainerEntryType<identifier_type,_entry_type> & rhs) const
    { return id_.name() == rhs.id_.name() ; }

    inline bool operator> 
        (const OidContainerEntryType<identifier_type, _entry_type> & rhs) const
    { return id_.name() > rhs.id_.name() ; }

    void serialize(SF::Archive & ar)
    {
        ar & id_ & entry_; 
    }
    inline const typename identifier_type::name_type & name() const 
    {
        return id_.name();
    } 
    inline const typename identifier_type::ordinal_type ordinal() const
    {
        return id_.ordinal();
    }
    inline const _entry_type & entry() const
    {
        return entry_;
    }
    inline const identifier_type & Id() const { return id_; }
    
    inline void set_identifier(const identifier_type & id_in) 
    { 
        id_.set_name(id_in.name());
        id_.set_ordinal(id_in.ordinal());
    }
    inline void set_entry(const basic_entry_type & entry_in)
    {
        entry_ = entry_in;
    }
private:
    identifier_type id_;
    _entry_type     entry_;
};

enum OP_RESPONSE {
        OP_NO_ERROR = 0,
        OP_ORDINAL_USED,
        OP_NAME_USED,
        OP_ALLREADY_CONTAINS_ENTRY,
        OP_ORDINAL_OVERFLOW
};

template<typename _identifier_type, typename _entry_type>
class OidContainer
{
public:
    typedef _identifier_type                            identifier_type;
    typedef _entry_type                                 basic_entry_type;
    typedef OidContainerEntryType<  identifier_type, 
                                    basic_entry_type>   entry_type;
    typedef typename _identifier_type::name_type        name_type;
    typedef typename _identifier_type::ordinal_type     ordinal_type;
    typedef std::vector<entry_type>                     vector_type;

    OidContainer() : entries_(),name_index_() { }
    OidContainer(const OidContainer & rhs)
    {
        name_index_.clear();
        entries_ = rhs.entries_;
        regen_name_index_();
    }
    OidContainer& operator=(const OidContainer & oid)
    {
        if(this != &oid)
        {
            name_index_.clear();
            entries_ = oid.entries_;
            regen_name_index_();
        }

        return *this; 
    }
    
    inline const vector_type & get_entries() const
    {
        return entries_;
    }
 
    inline const std::size_t size() const
    {
        return entries_.size();
    }

    inline const std::size_t occupied_size() const
    {
        return name_index_.size();
    }

    inline const entry_type * EntryAtordinal(boost::uint32_t ordinal) const
    {   
        try
        {
            const entry_type * e = &(entries_.at(ordinal));
            if(e->is_initialized())
                return e;
            else
                return NULL; 
        }
        catch(std::out_of_range)
        {
            return NULL; 
        }
    }

    const entry_type * EntryWithName (
        const typename identifier_type::name_type & name_in) const 
    {
        typename name_index_set::const_iterator it =
            name_index_.find(name_in, name_key_finder());
        
        if( it == name_index_.end() )
            return NULL;
        else
        {
            return &(*it);
        }
    }

    
    
    OP_RESPONSE ContainsEither(const identifier_type & id) const
    {
        bool ordinal_in_vector=false;
       
        if(occupied_size() == 0)
            return OP_NO_ERROR;
     
        if(size() < (id.ordinal()+1))
            ordinal_in_vector=false;
        else if( entries_.at( id.ordinal() ).is_initialized())
            ordinal_in_vector=true;
        
        typename name_index_set::const_iterator it =
            name_index_.find(id.name(), name_key_finder());
        
        bool entry_name_used=true;
    
        if( it == name_index_.end())
            entry_name_used=false;

        if( ordinal_in_vector != entry_name_used ) {
            if( ordinal_in_vector)
                return OP_ORDINAL_USED;
            else
                return OP_NAME_USED;
        }
        
        if(entry_name_used)
            return OP_ALLREADY_CONTAINS_ENTRY;
        
        return OP_NO_ERROR;
    }

    void serialize(SF::Archive & ar)
    {
        ar & entries_;
        
        if(!ar.isWrite())
        {
            regen_name_index_();
        }
    }

    const  basic_entry_type * operator[] (const name_type & name) const 
    {
        const entry_type * ent = EntryWithName(name);

        if( ent != NULL )
            return (& ( ent->entry() ) );
        else return NULL;
    }
    const basic_entry_type * operator[] (const ordinal_type & ordinal) const
    {
        const entry_type * ent = EntryAtordinal(ordinal);
        if( ent != NULL) {
            return (& ( ent->entry() ) );
        }
        else return NULL;
    }

protected:
    typedef intrusive::set< entry_type > name_index_set;    
    
    struct name_key_finder // functor
    {
        bool operator()(    const typename identifier_type::name_type & name, 
                            const entry_type & entry) const
        {
            return ( name < entry.name() );
        }
        bool operator()(    const entry_type & entry,
                            const typename identifier_type::name_type & name) const
        {
           return ( entry.name() < name);
        }
    };

    
    
    void resize_if_needed_(boost::uint32_t ordinal)
    {
        std::size_t old_size = entries_.size();
        if( old_size < ordinal + 1) {
            name_index_.clear();
            entries_.resize(ordinal+1 );
            if(old_size != 0) 
                regen_name_index_();
        }
    }
    void regen_name_index_()
    {
        // the intrusive container needs to be clear()'d before or vector
        // resizing will fail an assertion. therefore this isn't handled
        // here.
        typedef typename std::vector<entry_type>::iterator
            it_type;

        it_type beg = entries_.begin();
        it_type end = entries_.end();

                    
        for(it_type it=beg;it != end; it++)
        {
            if(it->is_initialized())
                name_index_.insert(*it);        
        }        
    }

    std::vector<entry_type> entries_;
    name_index_set name_index_;
};

template <typename Base, typename Derived>
class SlicingContainer : public Base
{
public:
    typedef Base    base_type;
    typedef Derived derived_type;
    SlicingContainer()
        : Base() {}
    explicit SlicingContainer(const derived_type & d)
    {
        Base::name_index_.clear();
        Base::entries_.clear();
        std::size_t size=d.size();

        Base::entries_.resize(size);
        for(int i = 0; i < size; ++i)
        {
            typename Base::entry_type & bi       = Base::entries_.at(i);
            const typename Derived::entry_type * di  = d.EntryAtordinal(i);
            
            if(di != NULL) {
                bi.set_identifier(di->Id());//bi.ordinal 
                const typename Base::entry_type::basic_entry_type bet = di->entry();
                bi.set_entry(bet);
            }
        }
        Base::regen_name_index_();
    }
    void serialize(SF::Archive & ar)
    {
        Base::serialize(ar);
    }
};

template<typename TContainer>
class ContainerBuilder : public TContainer
{
public:
    OP_RESPONSE SetEntry(const typename TContainer::entry_type & entry)
    {
        resize_if_needed_(entry.ordinal() );

        OP_RESPONSE ret = ContainsEither(entry.Id());
        if (ret != OP_NO_ERROR)
            return ret;
        
        TContainer::entries_.at(entry.ordinal()) = entry;
        TContainer::name_index_.insert( TContainer::entries_.at( entry.ordinal() )); 
        
        return OP_NO_ERROR;
    } 
};


}
}

#endif
