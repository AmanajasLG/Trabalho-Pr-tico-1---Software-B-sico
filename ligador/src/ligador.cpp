#include <iostream>
#include <stdbool.h>
#include "../include/junta_arquivos.hpp"

int main(int argc, char *argv[])
{
    if (argc > 5)
    {
        std::cout << "ERRO - Numero invalido de argumentos" << std::endl;
        std::cout << "Insira: ./ligador file_name1 ... file_name4" << std::endl;
        std::cout << "Podem ser inseridos até 4 arquivos" << std::endl;
        std::cout << "file_name deve ser inserido sem extensao - pre-definida: .obj" << std::endl;
        return 0;
    }

    std::vector<std::string> files_name;

    for (int i = 1; i < argc; i++)
    {
        files_name.push_back(argv[i]);
    }

    JoinFiles jf = JoinFiles(files_name);

    if (jf.IsValidFile())
    {
        jf.Join();
        jf.CloseFiles();
    }

    return 0;
}
