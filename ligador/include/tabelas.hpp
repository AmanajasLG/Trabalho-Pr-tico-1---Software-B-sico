#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <boost/algorithm/string.hpp>

#ifndef TABLES_H
#define TABLES_H

class Tables
{
    struct DefinitionTable
    {
        std::string symbol;
        int addr;
    };

    struct UseTable
    {
        std::string symbol;
        int addr; //soma com o valor correspondente no ligador
    };

  public:
    void AddElementDefinitionTable(std::string symbol, int addr);
    int GetDefinitionAddr(std::string symbol);
    std::string GetDefinitionSymbolName(int pos) { return _defs[pos].symbol; }
    bool IsSymbolInDefinitionTable(std::string symbol);
    int GetDefinitionTableSize() { return _defs.size(); }

    void AddElementUseTable(std::string symbol, int addr);
    int GetUseAddr(int pos) { return _use[pos].addr; }
    int GetUseTableSize() { return _use.size(); }
    std::string GetUseSymbolName(int pos) { return _use[pos].symbol; }
    int IsAddrInUseTable(int addr);

    void AddRelativeAddr(int addr)
    {
        _relatives.push_back(addr);
    }
    int GetRelativeAddr(int pos)
    {
        return _relatives[pos];
    }
    int GetRelativeSize()
    {
        return _relatives.size();
    }
    bool IsAddrInRelative(int addr);

    void SetTable(std::string table)
    {
        if (table == "USE")
        {
            tableUse = true;
            tableDefinition = false;
            relative = false;
        }
        else if (table == "DEFINITION")
        {
            tableUse = false;
            tableDefinition = true;
            relative = false;
        }
        else if (table == "RELATIVE")
        {
            tableUse = false;
            tableDefinition = false;
            relative = true;
        }
    }

    bool IsTableUse() { return tableUse; }
    bool IsTableDefinition() { return tableDefinition; }
    bool IsRelative() { return relative; }

    void ShowDefinitionTable()
    {
        std::cout << "DEFINITION TABLE \n\n"
                  << std::endl;
        for (int i = 0; i < _defs.size(); i++)
        {
            std::cout << "Symbol: " << _defs[i].symbol << std::endl;
            std::cout << "Addr: " << _defs[i].addr << std::endl;
            std::cout << "\n"
                      << std::endl;
        }
    }
    void ShowUseTable()
    {
        std::cout << "USE TABLE \n\n"
                  << std::endl;
        for (int i = 0; i < _use.size(); i++)
        {
            std::cout << "Symbol: " << _use[i].symbol << std::endl;
            std::cout << "Addr: " << _use[i].addr << std::endl;
            std::cout << "\n"
                      << std::endl;
        }
    }

  private:
    std::vector<DefinitionTable> _defs;
    std::vector<UseTable> _use;
    std::vector<int> _relatives;
    bool tableUse = false;
    bool tableDefinition = false;
    bool relative = false;
};

#endif