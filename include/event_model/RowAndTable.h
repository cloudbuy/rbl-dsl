#ifndef _EM_ROW_AND_TABLE_H
#define _EM_ROW_AND_TABLE_H
#include "Types.h"
#include "Descriptors.h"
#include <parser/detail/row_parser.h>

namespace event_model
{
    
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
        row_parse_grammar rpg_;
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
