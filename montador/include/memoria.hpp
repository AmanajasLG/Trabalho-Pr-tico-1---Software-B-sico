#include <iostream>
#include <vector>

class Memory
{

  public:
    Memory()
    {
        for (int i = 0; i < 20; i++)
        {
            _memory_spaces.push_back(0);
        }
    }

    int returnMemorySize()
    {
        return _memory_spaces.size();
    }

    void includeMemorySpace()
    {
        _memory_spaces.push_back(0);
    }

    void changeMemorySpaceValue(int memory_pos, int val)
    {
        while (_memory_spaces.size() < memory_pos)
        {
            includeMemorySpace();
        }

        _memory_spaces[memory_pos] = val;
    }

  private:
    std::vector<int> _memory_spaces;
};