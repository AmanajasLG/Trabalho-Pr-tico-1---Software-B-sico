#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

#ifndef TABLES_H
/*  
*   Tabelas
*   Classe que gera as tabelas necessarias - Uso, Definicoes e Simbolos
*/
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
        int addr;
    };

  public:
    /* Funcoes para gerenciar e retirar informacoes da tabela Simbolos */
    void AddElementSymbolTable(std::string symbol, int addr, std::string section = "TEXT", bool isVariable = false, int value = 0, bool isZero = false, bool isConst = false, bool isExtern = false, bool isVector = false, int vectorSize = 1);
    int GetSymbolAddr(std::string symbol);
    int GetSymbolValue(std::string symbol);
    int GetVectorSize(std::string symbol);
    bool IsSymbolValueZero(std::string symbol);
    bool IsSymbolConst(std::string symbol);
    bool IsSymbolExtern(std::string symbol);
    bool IsSymbolVariable(std::string symbol);
    bool IsSymbolVector(std::string symbol);
    bool IsSymbolInSymbolTable(std::string symbol);
    std::string SymbolSection(std::string symbol);

    /* Funcoes para gerenciar e retirar informacoes da tabela Definicoes */
    void AddElementDefinitionTable(std::string symbol, int addr);
    int GetDefinitionAddr(std::string symbol);
    std::string GetDefinitionSymbolName(int pos) { return _defs[pos].symbol; }
    bool IsSymbolInDefinitionTable(std::string symbol);
    int GetDefinitionTableSize() { return _defs.size(); }

    /* Funcoes para gerenciar e retirar informacoes da tabela Uso */
    void AddElementUseTable(std::string symbol, int addr);
    int GetUseAddr(int pos) { return _use[pos].addr; }
    int GetUseTableSize() { return _use.size(); }
    std::string GetUseSymbolName(int pos) { return _use[pos].symbol; }

  private:
    std::vector<SymbolTable> _symbols;
    std::vector<DefinitionTable> _defs;
    std::vector<UseTable> _use;
};

#endif