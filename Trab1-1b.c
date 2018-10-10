#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define MEM_SIZE 200

int testaRotulo(const char *);
char* retornaRotuloSemDoisPontos(char *);
int testaComentario(const char *);
int testaOpcode(char *, char **);
int testaDiretiva(char *, char **);

enum codigo{ADD=0, SUB=1, MULT=2, DIV=3, JMP=4, JMPN=5, JMPP=6, JMPZ=7, COPY=8, LOAD=9, STORE=10, INPUT=11, OUTPUT=12, STOP=13}; //de 0 a 13.

struct Opcodes {
    char mnemonico[50];
    int qtdOperandos;
    int codigo;
    int tamanho;
};
struct Diretivas {
    char mnemonico[50];
    int qtdOperandos;
    int tamanho;
};
struct Tabelas {
    char rotulo[50];
    int endereco;
};
struct Linhas {
    int posicaoInicial;
    char rotulo[50];
    char opcode[50];
    char operando1[50];
    char operando2[50];
};
struct Equiv {
    char simbolo[50];
    int valor;
    int linha;
};

int memoria[MEM_SIZE];

int main() {

//LÊ O ARQUIVO-----------------------------
FILE* fp;
fp = fopen("teste.asm", "r");
char buffer[100];
char linha[100][100];
int n = 0;
int temp;
//Coloca cada linha do arquivo em uma posição do vetor linha[]
while(fgets(buffer, 100, (FILE*) fp)) {
    strcpy(linha[n], buffer);
    //temp = strlen(linha[n]);
    //printf("%d\n", temp);
    //printf("%s\n", linha[n]);
    n++;
}
fclose(fp);
struct Linhas linhas[n];
//-----------------------------------------
//CONSTRUCAO DA TABELA DE INSTRUCOES-------
struct Opcodes instrucoes[15];
enum codigo cod;
int operandos[] =   {1,1,1,1,1,1,1,1,2,1,1,1,1,0};
int tamanho[] =     {2,2,2,2,2,2,2,2,3,2,2,2,2,1};
char* mne[] = {"ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"};
int i, j;
for (i = 1; i <= 14; i++) {
    strcpy(instrucoes[i].mnemonico, mne[i-1]);
    instrucoes[i].qtdOperandos = operandos[i-1];
    instrucoes[i].codigo = i;
    instrucoes[i].tamanho = tamanho[i-1];
    //printf("%s\t%d\t%d\t%d\n", instrucoes[i].mnemonico, instrucoes[i].qtdOperandos, instrucoes[i].codigo, instrucoes[i].tamanho);
}
//----------------------------------------
//CONSTRUCAO DA TABELA DE DIRETIVAS-------
struct Diretivas diretiva[9];
int operandosDir[] =   {1,1,1,0,1,1,0,0,0};
int tamanhoDir[] =     {0,1,1,0,0,0,0,0,0};
char* mneDir[] = {"SECTION", "SPACE", "CONST", "PUBLIC", "EQU", "IF", "EXTERN", "BEGIN", "END"};
for (i = 0; i < 9; i++) {
    strcpy(diretiva[i].mnemonico, mneDir[i]);
    diretiva[i].qtdOperandos = operandosDir[i];
    diretiva[i].tamanho = tamanhoDir[i];
    //printf("%s\t%d\t\t%d\n", diretiva[i].mnemonico, diretiva[i].qtdOperandos, diretiva[i].tamanho);
}
//----------------------------------------
//Declaração de tabelas de simbolos e de definicao
struct Tabelas tabela_simbolos[50], tabela_def[50];
for(i = 0; i <50; i++) {
    tabela_simbolos[i].endereco = -1;
    tabela_def[i].endereco = -1;
}
//----------------------------------------
//INICIA A MEMORIA COM 0's----------------
for (i = 0; i < MEM_SIZE; i++) {
    memoria[i] = 0;
}
//----------------------------------------

//-------- PRE PROCESSAMENTO -------------
char *ch;
struct Equiv equivalencia[n];
int k = 0;
int flagImprimir = 1;
// O programa deve dar DUAS saidas. Uma saida (*.pre)
// com o programa pre-processado, onde devem ser
// retirados os comentarios e avaliadas as diretivas
// EQU e IF.

fp = fopen("preProc.asm", "w+");

for (i = 0; i < n; i++) {
    ch = strtok(linha[i], " ");
    while (linha[i][strlen(linha[i])-1] == '\n') {  // retirar \n repetido
        linha[i][strlen(linha[i])-1] = '\0';
    }
    while (ch != NULL) {
        if (strcmp("EQU", ch) == 0) {
            ch = strtok(NULL, " \n");
            strcpy(equivalencia[k].simbolo, ch);
            ch = strtok(NULL, " \n");
            equivalencia[k].valor = strtol(ch, (char**)NULL, 10);
            k++;
            flagImprimir = 0;
            break;
        }
        if (strcmp("IF", ch) == 0){

        }
        fprintf(fp, "%s", ch);
        ch = strtok(NULL, " \n");
        if (ch != NULL) {
            fprintf(fp, " ", ch);
        }
    }
    if (i + 1 != n && flagImprimir == 1) {
            fprintf(fp, "\n", ch);
    }
    flagImprimir = 1;
}
fclose(fp);

//printf("%s %d\n", equivalencia[0].simbolo, equivalencia[0].valor);
//printf("%s %d\n", equivalencia[1].simbolo, equivalencia[1].valor);
//PRIMEIRA PASSAGEM:----------------------
int contador_posicao = 0;
int contador_linha = 1;
//n é o tamanho de linha[] (qtd de linhas no arquivo)
for (i = 0; i < n; i++) {                                                                       // Obtém uma linha do fonte
    ch = strtok(linha[i], " ");
    //printf("Contador Linha: %d\n", contador_linha);
    contador_linha++;
// AQUI COMECA A SEPARAR A LINHA (ch):
    while (ch != NULL) {                                                                        // Separa os elementos da linha: rótulo, operação, operandos, comentários
        //printf("Contador Posicao: %d\n", contador_posicao);
        //printf("%s\n", ch);   // TA IMPRIMINDO CADA PALAVRA SEPARADA JA AQUI
        if (testaComentario(ch) == 1) {                                                         // Ignora os comentários
            ch = strtok(NULL, " \n");
            break;
        }
        if (testaRotulo(ch) == 1) { // SE FOR RÓTULO (acaba em :), guarda o contador_posicao    // Se existe rótulo:
            //printf("rot: %s\n", retornaRotuloSemDoisPontos(ch));
            for (j = 0; j < n; j++) {                                                           // Procura rótulo na TS (Tabela de Símbolos)
                if (strcmp(retornaRotuloSemDoisPontos(ch), tabela_simbolos[j].rotulo)==0) {     // Se achou: Erro --> símbolo redefinido
                    printf("ERRO -> simbolo redefinido! TEM QUE SAIR DO PROGRAMA?\n");
                    break;
                }
            }
            strcpy(tabela_simbolos[i].rotulo, retornaRotuloSemDoisPontos(ch));
            tabela_simbolos[i].endereco = contador_posicao;                                     // Senão: Insere rótulo e contador_posição na TS
        }

        if (testaOpcode(ch,mne) > 0) {    // Achou o opcode                                     // Procura operação na tabela de instruções
            printf("%d", contador_posicao);
            contador_posicao++;                                                                 // Se achou: contador_posição = contador_posição + tamanho da instrução
            printf("-%s- ", ch);
            cod = testaOpcode(ch, mne);
            printf("(cod %d, %d operandos)\n", cod+1, instrucoes[cod+1].qtdOperandos);
            contador_posicao = contador_posicao + instrucoes[cod+1].qtdOperandos; // já achou a instrucao, entao pula a linha
            break;
        } else {                        // Não achou o opcode                                   // Senão:
                                                                                                // Procura operação na tabela de diretivas
            if (testaDiretiva(ch,mneDir) >= 0) {    // achou a diretiva, chama a subrotina adequada
                printf("%d%s\n", contador_posicao, mneDir[testaDiretiva(ch,mneDir)]);
                // ATUALIZAR O CONTADOR DE POSICAO COM O VALOR RETORNADO PELA SUBROTINA
                switch (testaDiretiva(ch,mneDir)) {
                case 0:     // SECTION

                    break;
                case 1:     // SPACE - Reservar 1 ou mais enderecos de memoria nao-inicializada para armazenamento de uma palavra. ok
                    ch = strtok(NULL, " \n");
                    if (ch != NULL) {       // ou seja, eh um vetor que esta sendo reservado
                        //printf("TESTE: -%s-\n", ch);
                        //printf("%d ", contador_posicao);
                        contador_posicao = contador_posicao + strtol(ch, (char**)NULL, 10);
                    } else {
                        contador_posicao++;
                    }
                    break;
                case 2:     // CONST ok
                    ch = strtok(NULL, " \n");
                    memoria[contador_posicao] = strtol(ch, (char**)NULL, 10);
                    contador_posicao++;
                    break;
                case 3:     // PUBLIC
                    // ?????????????????????
                    break;
                case 4:     // EQU
                    // Assumir que a diretiva EQU sera sempre definida no inicio do program ANTES da secao de TEXTO.
                    break;
                case 5:     // IF
                    break;
                case 6:     // EXTERN
                    break;
                case 7:     // BEGIN
                    break;
                case 8:     // END
                    break;
                }
            } else {        // nao achou a diretiva --> Erro, operação não identificada
                //printf("ERRO --> operacao nao identificada! TEM QUE SAIR DO PROGRAMA???\n");
            }
        }
           // TA IMPRIMINDO CADA PALAVRA SEPARADA JA AQUI
        ch = strtok(NULL, " \n");
        //printf("%s\n", ch);
    }
    //printf("FIM DA LINHA ---\n");
}
// -------------------- FIM DA PRIMEIRA PASSAGEM ------------------------

// SEGUNDA PASSAGEM -----------------------------------------------------
contador_posicao = 0;
contador_linha = 1;

int ACC = 0;

for (i = 0; i < n; i++) {
    ch = strtok(linha[i], " ");
    while (linha[i][strlen(linha[i])-1] == '\n') {  // retirar \n repetido
        linha[i][strlen(linha[i])-1] = '\0';
    }
    contador_linha++;
    while (ch != NULL) {
        if (testaComentario(ch) == 1) { // ignora comentarios
            ch = strtok(NULL, " \n");
            break;
        }
        if (testaRotulo(ch) == 1) {
            // ignora rótulo
        }

        if (testaOpcode(ch,mne) > 0) {    // Achou o opcode
            printf("%d", contador_posicao);
            contador_posicao++;
            printf("-%s- ", ch);
            cod = testaOpcode(ch, mne);
            printf("(cod %d, %d operandos)\n", cod+1, instrucoes[cod+1].qtdOperandos);
            contador_posicao = contador_posicao + instrucoes[cod+1].qtdOperandos;

//            switch (cod) {
//                case ADD:   // ACC <- ACC + MEM[OP]
//                    ACC = ACC +
//
//            }

            break;
        } else {                        // Não achou o opcode

            if (testaDiretiva(ch,mneDir) >= 0) {    // achou a diretiva, chama a subrotina adequada
                printf("%d%s\n", contador_posicao, mneDir[testaDiretiva(ch,mneDir)]);
                switch (testaDiretiva(ch,mneDir)) {
                case 0:     // SECTION

                    break;
                case 1:     // SPACE - Reservar 1 ou mais enderecos de memoria nao-inicializada para armazenamento de uma palavra. ok
                    ch = strtok(NULL, " \n");
                    if (ch != NULL) {       // ou seja, eh um vetor que esta sendo reservado
                        //printf("TESTE: -%s-\n", ch);
                        //printf("%d ", contador_posicao);
                        contador_posicao = contador_posicao + strtol(ch, (char**)NULL, 10);
                    } else {
                        contador_posicao++;
                    }
                    break;
                case 2:     // CONST ok
                    ch = strtok(NULL, " \n");
                    memoria[contador_posicao] = strtol(ch, (char**)NULL, 10);
                    contador_posicao++;
                    break;
                case 3:     // PUBLIC
                    // ?????????????????????
                    break;
                case 4:     // EQU
                    break;
                case 5:     // IF
                    break;
                case 6:     // EXTERN
                    break;
                case 7:     // BEGIN
                    break;
                case 8:     // END
                    break;
                }
            } else {        // nao achou a diretiva --> Erro, operação não identificada
                //printf("ERRO --> operacao nao identificada! TEM QUE SAIR DO PROGRAMA???\n");
            }
        }
           // TA IMPRIMINDO CADA PALAVRA SEPARADA JA AQUI
        ch = strtok(NULL, " \n");
        //printf("%s\n", ch);
    }
    //printf("FIM DA LINHA ---\n");
}





for (i = 0; i < MEM_SIZE; i++) {
    printf("%d: %d\t", i, memoria[i]);
}

}

int testaRotulo(const char *str) {
    //printf("%c\n", str[strlen(str) - 1]);
    return (str[strlen(str) - 1] == ':') ? 1 : 0;
}

char* retornaRotuloSemDoisPontos(char *str) {
    if (testaRotulo(str)){
        str[strlen(str) - 1] = '\0';
    }
    return(str);
}

int testaComentario(const char *str) {
    return (str[0] == ';') ? 1 : 0;
}

int testaOpcode(char *str, char **mnemonico) {
    int i;
    for (i = 0; i < 14; i++) {
        if(strcmp(str, mnemonico[i]) == 0) {
            //printf("%s eh opcode: %s\n", str, mnemonico[i]);
            return (i);
        }
    }
    return (-1);
}

int testaDiretiva(char *str, char **mnemonicoDir) {
    int i;
    for (i = 0; i < 9; i++) {
        if(strcmp(str, mnemonicoDir[i]) == 0) {
        //printf("%s eh diretiva: %s\n", str, mnemonicoDir[i]);
        return (i);
        }
    }
    return (-1);
}
