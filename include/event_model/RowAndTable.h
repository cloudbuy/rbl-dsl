#ifndef _EM_ROW_AND_TABLE_H
#define _EM_ROW_AND_TABLE_H
#include "Types.h"
#include "Descriptors.h"
#include "detail/OidContainer.h"    

namespace event_model
{
    typedef boost::variant< int32_t, int64_t, std::string> value_variant;
    typedef std::vector<value_variant> value_variant_vector;
    typedef std::vector<std::string> vector_string;    
    typedef primitives::OidContainerEntryType<Oid,EventTypeContainer> table_descriptor;
    
    VALUE_TYPE RowTypeAt(   const table_descriptor & td_,
                            const uint32_t ordinal);   
    class Row
    {
    public:
        Row(const table_descriptor & row_descriptor);
        bool operator >>(std::string & str) const; 
        bool operator <<(const std::string & str);
        const value_variant_vector & get_row_vector() const;
    private:
        value_variant_vector row_data_;
        const table_descriptor & td_;
    };
    
    
    class Table
    {
    public:
        Table(const table_descriptor & row_descriptor);

        void push(const std::string & str);
        void pop(std::string & str) const ;
        bool push(const Row & row);
        bool pop(Row & row) const;
    private:
        vector_string table_data_;
        const table_descriptor & td_;
    };
};
#endif 
