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
        std::string section;
        int value;
        bool isConst;
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
    void AddElementSymbolTable(std::string symbol, int addr, std::string section = "TEXT", int value = 0, bool isConst = false);
    int GetSymbolAddr(std::string symbol);
    int GetSymbolValue(std::string symbol);
    bool IsSymbolConst(std::string symbol);
    bool IsSymbolInSymbolTable(std::string symbol);
    std::string SymbolSection(std::string symbol);

    void AddElementDefinitionTable(std::string symbol, int val);
    int GetDefinitionVal(std::string symbol);
    bool IsSymbolInDefinitionTable(std::string symbol);

    void AddElementUseTable(std::string symbol, int addr);
    int GetUseAddr(std::string symbol);
    bool IsSymbolInUseTable(std::string symbol);

  private:
    std::vector<SymbolTable> _symbols;
    std::vector<DefinitionTable> _defs;
    std::vector<UseTable> _use;
};