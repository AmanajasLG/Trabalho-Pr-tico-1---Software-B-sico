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

  private:
    std::vector<int> _memory_spaces;
};

#endif