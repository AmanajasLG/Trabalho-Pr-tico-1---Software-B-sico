#include "../include/tabelas.hpp"
#include <boost/algorithm/string.hpp>

void Tables::addElementSymbolTable(std::string symbol, int addr)
{
    if (isSymbolInSymbolTable(symbol))
    {
        std::cout << "Simbolo ja esta na tabela!" << std::endl;
        return;
    }
    _symbols.push_back(SymbolTable{symbol, addr});
}
int Tables::getSymbolAddr(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].addr;
        }
    }

    return 0;
}
bool Tables::isSymbolInSymbolTable(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
            return true;
    }

    return false;
}

void Tables::addElementDefinitionTable(std::string symbol, int val)
{
    if (isSymbolInDefinitionTable(symbol))
    {
        std::cout << "Simbolo ja esta na tabela!" << std::endl;
        return;
    }
    _defs.push_back(DefinitionTable{symbol, val});
}
int Tables::getDefinitionVal(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _defs.size(); i++)
    {
        if (boost::iequals(symbol, _defs[i].symbol))
        {
            return _defs[i].val;
        }
    }

    return 0;
}
bool Tables::isSymbolInDefinitionTable(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _defs.size(); i++)
    {
        if (boost::iequals(symbol, _defs[i].symbol))
            return true;
    }

    return false;
}

void Tables::addElementUseTable(std::string symbol, int addr)
{
    if (isSymbolInUseTable(symbol))
    {
        std::cout << "Simbolo ja esta na tabela!" << std::endl;
        return;
    }
    _use.push_back(UseTable{symbol, addr});
}
int Tables::getUseAddr(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _use.size(); i++)
    {
        if (boost::iequals(symbol, _use[i].symbol))
        {
            return _use[i].addr;
        }
    }

    return 0;
}
bool Tables::isSymbolInUseTable(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _use.size(); i++)
    {
        if (boost::iequals(symbol, _use[i].symbol))
            return true;
    }

    return false;
}