#include <iostream>
#include <stdbool.h>
#include "../include/preprocessamento.hpp"
#include "../include/processamento.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "ERRO - Numero invalido de argumentos" << std::endl;
        std::cout << "Insira: ./montador file_name" << std::endl;
        std::cout << "file_name deve ser inserido sem extensao - pre-definida: .asm" << std::endl;
        return 0;
    }

    Tables *_tables = new Tables();
    InstructionAnalizer _analizer;

    PreProcessing *pre_proc = new PreProcessing(argv[1], *_tables, _analizer);

    if (pre_proc->isFileOpen())
        pre_proc->PreProcess();
    pre_proc->CloseFiles();

    Processing *proc = new Processing(argv[1], _tables, _analizer);
    if (proc->isFileOpen())
    {
        if (proc->FirstPass())
            proc->SecondPass();
    }
    proc->CloseFiles();

    return 0;
}
