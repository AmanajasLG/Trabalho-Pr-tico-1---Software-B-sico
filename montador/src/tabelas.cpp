#include "../include/tabelas.hpp"
#include <boost/algorithm/string.hpp>

void Tables::AddElementSymbolTable(std::string symbol, int addr, std::string section, bool isVariable, bool isZero, bool isConst)
{
    if (symbol.back() == ':')
        symbol.pop_back();

    if (IsSymbolInSymbolTable(symbol))
    {
        std::cout << "Simbolo ja esta na tabela!" << std::endl;
        return;
    }
    _symbols.push_back(SymbolTable{symbol, addr, section, isVariable, isZero, isConst});
}
int Tables::GetSymbolAddr(std::string symbol)
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

    return -1;
}

bool Tables::IsSymbolValueZero(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].isZero;
        }
    }
}

bool Tables::IsSymbolConst(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].isConst;
        }
    }

    return false;
}

bool Tables::IsSymbolInSymbolTable(std::string symbol)
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

std::string Tables::SymbolSection(std::string symbol)
{
    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
            return _symbols[i].section;
    }
}

void Tables::AddElementDefinitionTable(std::string symbol, int addr)
{
    if (IsSymbolInDefinitionTable(symbol))
    {
        std::cout << "Simbolo ja esta na tabela!" << std::endl;
        return;
    }
    _defs.push_back(DefinitionTable{symbol, addr});
}
int Tables::GetDefinitionAddr(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _defs.size(); i++)
    {
        if (boost::iequals(symbol, _defs[i].symbol))
        {
            return _defs[i].addr;
        }
    }

    return 0;
}
bool Tables::IsSymbolInDefinitionTable(std::string symbol)
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

void Tables::AddElementUseTable(std::string symbol, int addr)
{
    if (IsSymbolInUseTable(symbol))
    {
        std::cout << "Simbolo ja esta na tabela!" << std::endl;
        return;
    }
    _use.push_back(UseTable{symbol, addr});
}
int Tables::GetUseAddr(std::string symbol)
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

bool Tables::IsSymbolInUseTable(std::string symbol)
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