#include "../include/tabelas.hpp"
#include <boost/algorithm/string.hpp>

void Tables::AddElementDefinitionTable(std::string symbol, int addr)
{
    _defs.push_back(DefinitionTable{symbol, addr});
}
int Tables::GetDefinitionAddr(std::string symbol)
{
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
    for (int i = 0; i < _defs.size(); i++)
    {
        if (boost::iequals(symbol, _defs[i].symbol))
        {
            return true;
        }
    }

    return false;
}

void Tables::AddElementUseTable(std::string symbol, int addr)
{
    _use.push_back(UseTable{symbol, addr});
}

int Tables::IsAddrInUseTable(int addr)
{
    for (int i = 0; i < _use.size(); i++)
    {
        if (addr == _use[i].addr)
        {
            return i;
        }
    }
    return -1;
}

bool Tables::IsAddrInRelative(int addr)
{
    for (int i = 0; i < _relatives.size(); i++)
    {
        if (addr == _relatives[i])
        {
            return true;
        }
    }
    return false;
}