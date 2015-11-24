#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <signal.h>

#define tamsenha 5

static volatile int controle;

void espera(void);
void sinal(void);

int main(void)
{
    key_t chave;
    int mcid;
    char *mc = NULL, *pm1 = NULL, *pm2 = NULL, senha[tamsenha];
    int pid_11b;
    if((chave = ftok("senha.c", (int)rand()%256)) == -1)
    {
        printf("11a - Erro na geracao da chave.\n");
        exit(1);
    }
    if((mcid = shmget(chave, 2*(tamsenha)*sizeof(char), 0700 | IPC_CREAT)) == -1)
    {
        printf("11a - Erro no alocamento de memoria.\n");
        exit(1);
    }
    if((char *)(mc = (char *)shmat(mcid, (void *)0, 0))==(char *)(-1))
    {
        printf("11a - Erro na criacao do ponteiro para memoria.\n");
        exit(1);
    }
    return 0;
}

void espera(void)
{
    printf("11a - entrando em estado de espera.\n");
    while(controle)
        sleep(1);
}

void sinal(void)
{
    controle = 0;
    signal(SIGUSR1, (void *) sinal);
}
