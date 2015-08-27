#ifndef HASQL_H
#define HASQL_H
#include <string>

class HaSql
{
    bool open = false;
    int cols,rows;
    std::string name;

    public:
        HaSql();
        virtual ~HaSql();
        void db_open(std::string);
        void db_close();
        bool db_exists(std::string);
        void db_data();
        int db_rows(std::string);
        int db_cols(std::string);
        void db_enter_row();
        bool db_enter_row_array(std::string[]);
        bool db_entry_exists(std::string[],std::string[]);
        std::string * db_col_names();
    protected:
    private:

};

#endif // HASQL_H
