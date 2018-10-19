#include <iostream>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc > 5)
    {
        std::cout << "ERRO - Numero invalido de argumentos" << std::endl;
        std::cout << "Insira: ./ligador file_name1 ... file_name4" << std::endl;
        std::cout << "Podem ser inseridos até 4 arquivos" << std::endl;
        return 0;
    }

    std::cout << argv[0] << ", " << argv[1] << std::endl;

    return 0;
}
