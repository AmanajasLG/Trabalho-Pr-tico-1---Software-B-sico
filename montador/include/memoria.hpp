#include <iostream>
#include <vector>

#ifndef MEMORY_H
/* 
*   Memoria
*   Classe guarda os valores de cada endereco
*/
#define MEMORY_H
class Memory
{

  public:
    /* Retorna o tamanho da memoria */
    int GetMemorySize()
    {
        return _memory_spaces.size();
    }

    /* Retorna o valor de uma determinada posicao */
    int GetMemoryPositionValue(int pos)
    {
        return _memory_spaces[pos];
    }

    /* Adiciona um espaco na memoria */
    void IncludeMemorySpace(int val)
    {
        _memory_spaces.push_back(val);
    }

    /* Muda um valor de um local da memoria */
    void ChangeMemorySpaceValue(int memory_pos, int val)
    {
        _memory_spaces[memory_pos] = val;
    }

  private:
    std::vector<int> _memory_spaces;
};

#endif