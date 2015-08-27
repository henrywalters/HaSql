#include "HaSql.h"
#include <string>
#include <fstream>
#include <iostream>

HaSql::HaSql()
{
    //ctor
}

HaSql::~HaSql()
{
    //dtor
}

bool HaSql::db_exists(std::string lookup)
{
    std::ifstream db(lookup.c_str());
    if (db.good()) { return true ; }
    else { return false ; }
}

void HaSql::db_close()
{
    if (open)
    {
        open = false;
        cols = 0;
        rows = 0;
        name = "";
        std::cout << "DATABASE CLOSED";
    }
    else{ std::cout << "DATABASE IS NOT OPEN"; }
}
void HaSql::db_open(std::string new_db)
{
    if (!db_exists(new_db))
    {
        std::cout << "NEW DATABASE CONFIG\n";
        int col;
        std::cout << "COLUMNS IN DATABASE: ";
        std:: cin >> col;
        std::cout << "DATABASE BEING CREATED...\n";

        cols = col;
        rows = 0;
        open = true;
        name = new_db;

        std::ofstream mkdb(new_db.c_str());
        for (int i = 1; i <= col; i++)
        {
            std::cout << "COL " << i << " NAME: ";
            std::string inp;
            std::cin >> inp;
            mkdb << inp << " ";
        }
        mkdb.close();
    }
    else
    {
        open = true;
        rows = db_rows(new_db);
        cols = db_cols(new_db);
        name = new_db;
    }

}
bool HaSql::db_enter_row_array(std::string entries[])
{
    if (!open || sizeof(entries) != cols) { std::cout << "DATABASE NOT OPEN/INVALID ENTRY"; return false; }
    else
    {

        std::ofstream d(name.c_str(), std::ios::app);
        d << "\n";
        for (int i = 0; i < cols; i++)
        {
            d << entries[i] << " ";
        }
        rows++;
        return true;
    }
}
void HaSql::db_enter_row()
{
    if (!open){ std::cout << "DATABASE NOT OPEN"; }
    else
    {
        std::ofstream d(name.c_str(), std::ios::app);
        d << "\n";
        std::string *columns = db_col_names();
        for (int i = 0; i < cols; i++)
        {
            std::cout << "ENTRY TO " << columns[i] << ": ";
            std::string input;
            std::cin >> input;
            d << input << " ";

        }
        rows++;
        d.close();
    }
}

std::string * HaSql::db_col_names()
{
    if (!open) { std::cout << "DATABASE IS NOT OPEN..."; return NULL; }
    else
    {
        std::ifstream db(name.c_str());
        std::string line;
        std::getline(db,line);
        std::string * n;
        n = new (std::nothrow) std::string[cols];
        int index = 0;
        int wordStart = 0;
        int wordEnd = 0;
        for (int i = 0; i < line.length(); i++)
        {
            wordEnd++;
            if (line[i] == ' ')
            {
                n[index] = line.substr(wordStart,wordEnd - wordStart - 1);
                index++;

                wordStart = wordEnd;
            }

        }
        return n;
    }
}

bool HaSql::db_entry_exists(std::string columns[], std::string entry[])
{
    bool check_at[cols];
    std::string * col_names = db_col_names();
    for (int i = 0; i < cols; i++)
    {
        check_at[i] = false;
        for (int j = 0; j < sizeof(columns)-1; j++)
        {
            if (col_names[i] == columns[j]) { check_at[i] = true; }
        }
    }

    int index = 0;

    std::string line;
    std::ifstream db(name.c_str(),std::ios::app);
    std::getline(db,line);

    bool exists = false;
    bool return_answer = false;

    while(std::getline(db,line))
    {
        std::string n[rows];
        int wordStart = 0;
        int wordEnd = 0;
        index = 0;
        for (int i = 0; i < line.length(); i++)
        {
            wordEnd++;
            if (line[i] == ' ')
                {
                    n[index] = line.substr(wordStart,wordEnd - wordStart - 1);
                    index++;

                    wordStart = wordEnd;
                }


        }

        bool all_confirmed = true;
        int col_check = 0;
        for (int i = 0; i < cols; i++)
        {

            if (check_at[i] && entry[col_check] != n[i]) { all_confirmed = false; }
            else { col_check++;}

        }
        if (all_confirmed) { return_answer = true;}

    }
    return return_answer;

}

void HaSql::db_data()
{
    if (!open)
    {
        std::cout << "DATABASE IS NOT OPEN";
    }
    else
    {
        std::cout << "DATABASE NAME: " << name;
        std::cout << "\nCOLUMNS: " << cols;
        std::cout << "\nROWS: " << rows;
    }
}

int HaSql::db_rows(std::string db)
{
    std::ifstream d(db.c_str());
    if (!db_exists(db) || !open)
    {
        return 0;
    }
    else
    {
        int ro = -1;
        std::string line;
        while (std::getline(d,line))
        {
            ro++;
        }
        return ro;
    }
}

int HaSql::db_cols(std::string db)
{
    std::ifstream d(db.c_str());
    std::string line;
    std::getline(d,line);
    int co = 0;
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ' ') { co++; }
    }
    return co;
}







