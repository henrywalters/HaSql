#include "HaSql.h"
#include <string>
#include <fstream>
#include <iostream>
#include <CMD_Maker.h>

HaSql::HaSql()
{
    //ctor
}

HaSql::~HaSql()
{
    //dtor
}

void HaSql::hasql_cmdln()
{
    CMD_Maker hasql;
    hasql.set_prefix("HaSql");
    bool running = true;
    while (running)
    {
        hasql.get_line();
        int words = hasql.word_count();
        std::string * bash;
        bash = hasql.words();
        if (words == 1 && bash[0] == "END")
        {
            std::cout << "HASQL COMMAND LINE ENDED";
            running = false;

        }
        else if (words == 2 && bash[0] == "OPEN")
        {
            db_open(bash[1]);
        }
        else if (words == 1 && bash[0] == "CLOSE")
        {
            db_close();
        }
        else if (words == 1 && bash[0] == "STATUS")
        {
            db_data();
        }
        else if (words == 2 && bash[0] == "NEW" && bash[1] == "ENTRY")
        {
            db_enter_row();
        }
        else if (bash[0] == "SELECT" && bash[2] == "FROM" && words == 4)
        {
            std::string * matches = select(bash[3],bash[1]);
            std::cout << matches[0];
        }

    }
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
        std::cout << "DATABASE CLOSED\n";
    }
    else{ std::cout << "DATABASE IS NOT OPEN\n"; }
}
void HaSql::db_open(std::string new_db)
{
    if (!db_exists(new_db))
    {
        std::cout << "NEW DATABASE CONFIG\n";
        int col;
        std::cout << "COLUMNS IN DATABASE: ";
        std:: cin >> col;


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
        std::cout << "DATABASE BEING CREATED...";
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

std::string * HaSql::select(std::string column, std::string entry)
{
    std::ifstream db(name.c_str());
    std::string line;
    getline(db,line);
    std::string * columns = db_col_names();
    std::string * output;
    output = new (std::nothrow) std::string[rows];
    int index = 0;
    while(getline(db,line))
    {
        std::string * words = db_entry_arr(line);
        for (int i = 0; i < cols; i++)
        {
            if (words[i] == entry && column == columns[i])
            {
                output[index] = line;
            }
        }
    }
    return output;
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
        std::cout << "\nROWS: " << rows << "\n";
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


int HaSql::word_count(std::string line)
{
    int x = 1;
    for (int i = 1; i < line.length(); i++)
    {
        if (line[i] == ' ' && line[i-1] != ' ' && i <line.length() - 1)
        {
            x++;
        }
    }
    return x;
}

std::string * HaSql::db_entry_arr(std::string line)
{
    std::string * out;
    HaSql ha;
    out = new (std::nothrow) std::string[ha.word_count(line)];
    int wordStart = 0;
    int wordEnd = 0;
    int index = 0;
    for (int i = 0; i < line.length(); i++)
    {
        wordEnd++;
        if (line[i] == ' ' && line[i-1] != ' ')
        {
            out[index] = line.substr(wordStart,wordEnd-wordStart-1);
            wordStart = wordEnd;
            index++;
        }
    }
    if (index != ha.word_count(line))
    {
        out[ha.word_count(line) - 1] = line.substr(wordStart, line.length() - wordStart);
    }
    return out;
}


