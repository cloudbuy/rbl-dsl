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
   
        void reset();
 
        const bool parsing_error() const 
            { return rpg_.has_error; } 
        const bool was_parsing_value() const 
            { return rpg_.was_parsing_value; }
        const uint16_t was_parsing_ordinal() const 
            { return rpg_.current_ordinal; }
        const uint32_t parsing_value_type() const 
            { return rpg_.current_value_type; }
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
