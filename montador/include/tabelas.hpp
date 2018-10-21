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
        int value;
        bool isZero;
        bool isConst;
        bool isExtern;
        bool isVector;
        int vectorSize;
    };

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
    void AddElementSymbolTable(std::string symbol, int addr, std::string section = "TEXT", bool isVariable = false, int value = 0, bool isZero = false, bool isConst = false, bool isExtern = false, bool isVector = false, int vectorSize = 1);
    int GetSymbolAddr(std::string symbol);
    int GetSymbolValue(std::string symbol);
    bool IsSymbolValueZero(std::string symbol);
    bool IsSymbolConst(std::string symbol);
    bool IsSymbolExtern(std::string symbol);
    bool IsSymbolInSymbolTable(std::string symbol);
    std::string SymbolSection(std::string symbol);

    void AddElementDefinitionTable(std::string symbol, int addr);
    int GetDefinitionAddr(std::string symbol);
    std::string GetDefinitionSymbolName(int pos) { return _defs[pos].symbol; }
    bool IsSymbolInDefinitionTable(std::string symbol);
    int GetDefinitionTableSize() { return _defs.size(); }

    void AddElementUseTable(std::string symbol, int addr);
    int GetUseAddr(int pos) { return _use[pos].addr; }
    int GetUseTableSize() { return _use.size(); }
    std::string GetUseSymbolName(int pos) { return _use[pos].symbol; }

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
            std::cout << "IsExtern: " << _symbols[i].isExtern << std::endl;
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