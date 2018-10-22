Alunos: 
    Luíza Amanajás 16/0056659
    Rafael Raupp Bocorny 14/0159690


O trabalho foi desenvolvido em C++ e testado no UNIX LinuxMint 18.3 utilizando o ambiente de desenvolvimento VisualCode Studio.

Para compilar o programa é necessario g++ e ter a biblioteca boost instalada.

A biblioteca boost foi usada apenas para separação de palavras pela função boost::split e para verificar se duas palavras são iguais independente de sua case pela função boost::iequals.
Caso boost não esteja instalada no computador, ela pode ser instalda por: 
    sudo apt-get install libboost-all-dev 

Montador:
    Para compílar o montador basta entrar na pasta pelo terminal e colocar:
        g++ -o montador src/montador.cpp src/analisador_de_instrucao.cpp src/preprocessamento.cpp src/tabelas.cpp src/processamento.cpp src/testes.cpp -std=c++11 -L/usr/local/lib/ -lboost_filesystem 

    Será gerado o arquivo montador e para rodar basta colocar ./montador e o arquivo desejado. O arquivo precisa necessariamente estar dentro da pasta montador ou inserir o endereco junto com o nome.
    EX:
        - Arquivo dentro da pasta:
            ./montador teste
        -Arquivo fora da pasta:
            ./montador ../test

    OBS: O arquivo .obj é salvo diretamente na pasta /ligador

Ligador
    Para compilar o ligador basta entrar na pasta pelo terminal e colocar: 
        g++ -o ligador src/ligador.cpp src/memoria.cpp src/tabelas.cpp src/junta_arquivos.cpp -std=c++11 -L/usr/local/lib/ -lboost_filesystem

    Será gerado o arquivo ligador e para rodar basta colocar ./ligador e o(s) arquivo(s) desejado(s). O(s) arquivo(s) precisa(m) necessariamente estar dentro da pasta montador ou inserir o endereco junto com o nome.
    EX:
    - Arquivo dentro da pasta:
        ./ligador teste
    -Arquivo fora da pasta:
        ./ligador ../test
