#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

class Tables
{
    struct SymbolTable
    {
        std::string symbol;
        int addr;
    };

    struct DefinitionTable
    {
        std::string symbol;
        int val;
    };

    struct UseTable
    {
        std::string symbol;
        int addr;
    };

  public:
    void addElementSymbolTable(std::string symbol, int addr);
    int getSymbolAddr(std::string symbol);
    bool isSymbolInSymbolTable(std::string symbol);

    void addElementDefinitionTable(std::string symbol, int val);
    int getDefinitionVal(std::string symbol);
    bool isSymbolInDefinitionTable(std::string symbol);

    void addElementUseTable(std::string symbol, int addr);
    int getUseAddr(std::string symbol);
    bool isSymbolInUseTable(std::string symbol);

  private:
    std::vector<SymbolTable> _symbols;
    std::vector<DefinitionTable> _defs;
    std::vector<UseTable> _use;
};