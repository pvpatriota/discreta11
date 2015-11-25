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

void espera(void);
void sinal(void);
void troca_cor(char senha[tamsenha], int a);
void inverte_posicao(char senha[tamsenha], int a, int b);
int brancos(char senha[tamsenha]);
int pretos(char senha[tamsenha]);

int main(void)
{
    key_t chave;
    int mcid;
    char *mc = NULL, *pmp = NULL, *pmr = NULL, senha[tamsenha];
    int pid_11b, cont=0, nb, np, senhac[tamsenha];
    if((chave = ftok("senha.c", (int)rand()%256)) == -1)
    {
        printf("11b - Erro na geracao da chave.\n");
        exit(1);
    }
    if((mcid = shmget(chave, 2*(tamsenha)*sizeof(char), 0700 | IPC_CREAT)) == -1) /*da 33 a 47 ta criando a memoria dividida*/
    {
        printf("11b - Erro no alocamento de memoria.\n");
        exit(1);
    }
    if((char *)(mc = (char *)shmat(mcid, (void *)0, 0))==(char *)(-1))
    {
        printf("11b - Erro na criacao do ponteiro para memoria.\n");
        exit(1);
    }

    pmp = mc;
    pmr = mc + tamsenha; /*definindo ponteiros para a memoria dividida*/

    srand(time(NULL));

    printf("Digite o ID do processo do ex11a.x");
    scanf("%d", &pid_11a); /*ID do primeiro programa*/
    signal(SIGUSR1, (void *) sinal); /*preparando para receber o sinal*/

    return 0;
}
