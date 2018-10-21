#include <iostream>
#include <vector>

#ifndef MEMORY_H
#define MEMORY_H
class Memory
{

  public:
    int GetMemorySize()
    {
        return _memory_spaces.size();
    }

    int GetMemoryPositionValue(int pos)
    {
        return _memory_spaces[pos];
    }

    void IncludeMemorySpace(int val)
    {
        _memory_spaces.push_back(val);
    }

    void ChangeMemorySpaceValue(int memory_pos, int val)
    {
        while (_memory_spaces.size() < memory_pos)
        {
            IncludeMemorySpace(val);
        }

        _memory_spaces[memory_pos] = val;
    }

    void ShowMemory()
    {
        std::cout << "MEMORIA\n\n"
                  << std::endl;
        for (int i = 0; i < _memory_spaces.size(); i++)
        {
            std::cout << i << ":" << _memory_spaces[i] << "\n"
                      << std::endl;
        }
    }

  private:
    std::vector<int> _memory_spaces;
};

#endif