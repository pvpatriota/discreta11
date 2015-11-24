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
void gera_senha(char senha[tamsenha]);
int brancos(char senha[tamsenha], char tenta[tamsenha]);
int pretos(char senha[tamsenha], char tenta[tamsenha]);

int main(void)
{
    key_t chave;
    int mcid;
    char *mc = NULL, *pmp = NULL, *pmr = NULL, senha[tamsenha];
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

    pmp = mc;
    pmr = mc + tamsenha;

    srand(time(NULL));   
    gera_senha(senha);



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

void gera_senha(char senha[tamsenha])
{
    int i=0;
    for(; i<tamsenha-1; i++)
    {
        switch(rand()%4)
        {
            case 0:
                senha[i] = 'R';
                break;
            case 1:
                senha[i] = 'G';
                break;
            case 2:
                senha[i] = 'Y';
                break;
            case 3:
                senha[i] = 'B';
                break;
        }
    }
    senha[tamsenha-1] = '\0';
}

int brancos(char senha[tamsenha], char tenta[tamsenha])
{
    int i = 0, cont = 0;
    for(; i<tamsenha-1; i++)
        if(senha[i] == tenta[i])
            cont++;
    return cont;
}

int pretos(char senha[tamsenha], char tenta[tamsenha])
{
    int i=0, j=0, cont=0, aux[tamsenha]={0};
    for(; i<tamsenha-1; i++)
        for(; j<tamsenha-1; j++)
            if(senha[i] == tenta[j] && !aux[j])
            {
                cont++;
                aux[j] = 1;
            }
    return cont;
}

