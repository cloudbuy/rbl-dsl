#ifndef _EM_ROW_AND_TABLE_H
#define _EM_ROW_AND_TABLE_H
#include "Types.h"
#include "Descriptors.h"
#include <parser/detail/row_parser.h>
#include <boost/scoped_ptr.hpp>

namespace event_model
{
    class Row
    {
    public:
        Row(const table_descriptor & row_descriptor);
        bool operator >>(std::string & str) const; 
        bool operator <<(const std::string & str);
        const value_variant_vector & get_row_vector() const;
   
        void reset_row(); 

        const row_parser_error_descriptor * 
            get_row_parser_error_descriptor() const 
        { 
            if(rpg_scptr_ != NULL)
                return &rpg_scptr_->rped_; 
            else
                return NULL;
        }
    private:
        value_variant_vector row_data_;
        const table_descriptor & td_;

        boost::scoped_ptr<row_parse_grammar> rpg_scptr_;
        void reset_parser();
    };
   
     
    
    class Table
    {
    public:
        Table(const table_descriptor & row_descriptor);

        bool push(const Row & row);
        bool pop(Row & row) const;
        typedef std::vector<value_variant_vector> row_vector;
    private:
        row_vector rows_;
        const table_descriptor & td_;
        
    };
};
#endif 
