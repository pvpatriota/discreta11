#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <signal.h> /*bibliotecas*/
#include <string.h>

#define tamsenha 5 /*tamanho da senha*/

static volatile int controle; /*variavel pra controle da funcao dormir*/

typedef struct jogoant /*struct para auxiliar a IA*/
{
    char ant[tamsenha];
    char resultant[tamsenha];
    int nb, np, a, b, jogocerteza[tamsenha] = {0}, jogada;
}jogoant;


