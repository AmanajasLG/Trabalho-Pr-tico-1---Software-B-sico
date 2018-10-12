#include "../include/tabelas.hpp"
#include <boost/algorithm/string.hpp>

void Tables::addElementSymbolTable(std::string symbol, int addr) {}
int Tables::getSymbolAddr(std::string symbol) {}
bool Tables::isSymbolInSymbolTable(std::string symbol) {}

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
    for (int i = 0; i < _defs.size(); i++)
    {
        if (boost::iequals(symbol, _defs[i].symbol))
            return true;
    }

    return false;
}

void Tables::addElementUseTable(std::string symbol, int addr) {}
int Tables::getUseAddr(std::string symbol) {}
bool Tables::isSymbolInUseTable(std::string symbol) {}