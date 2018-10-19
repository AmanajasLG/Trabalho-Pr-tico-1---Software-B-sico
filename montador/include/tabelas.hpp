#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

#ifndef TABLES_H
#define TABLES_H

class Tables
{
    struct SymbolTable
    {
        std::string symbol;
        int addr;
        std::string section;
        bool isVariable;
        bool isZero;
        bool isConst;
    };

    struct DefinitionTable
    {
        std::string symbol;
        int addr;
    };

    struct UseTable
    {
        std::string symbol;
        int addr;
    };

  public:
    void AddElementSymbolTable(std::string symbol, int addr, std::string section = "TEXT", bool isVariable = false, bool isZero = false, bool isConst = false);
    int GetSymbolAddr(std::string symbol);
    bool IsSymbolValueZero(std::string symbol);
    bool IsSymbolConst(std::string symbol);
    bool IsSymbolInSymbolTable(std::string symbol);
    std::string SymbolSection(std::string symbol);

    void AddElementDefinitionTable(std::string symbol, int addr);
    int GetDefinitionAddr(std::string symbol);
    bool IsSymbolInDefinitionTable(std::string symbol);

    void AddElementUseTable(std::string symbol, int addr);
    int GetUseAddr(std::string symbol);
    bool IsSymbolInUseTable(std::string symbol);

    void ShowSymbolTable()
    {
        std::cout << "SYMBOL TABLE \n\n"
                  << std::endl;
        for (int i = 0; i < _symbols.size(); i++)
        {
            std::cout << "Symbol: " << _symbols[i].symbol << std::endl;
            std::cout << "Addr: " << _symbols[i].addr << std::endl;
            std::cout << "Section: " << _symbols[i].section << std::endl;
            std::cout << "IsVariable: " << _symbols[i].isVariable << std::endl;
            std::cout << "IsConst: " << _symbols[i].isConst << std::endl;
            std::cout << "IsZero: " << _symbols[i].isZero << std::endl;
            std::cout << "\n"
                      << std::endl;
        }
    }
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
    std::vector<SymbolTable> _symbols;
    std::vector<DefinitionTable> _defs;
    std::vector<UseTable> _use;
};

#endif