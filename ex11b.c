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
    int nb, np, a, b, jogada, jogocerteza[tamsenha];

}jogoant;

void espera(void);
void sinal(void);
void troca_cor(char senha[tamsenha], int a);
void inverte_posicao(char senha[tamsenha], int a, int b);
int brancos(char senha[tamsenha]);
int pretos(char senha[tamsenha]);
void deduzindo(char senha[tamsenha], char result[tamsenha], struct jogoant m1);

int main(void)
{
    key_t chave;
    int mcid;
    char *mc = NULL, *pmp = NULL, *pmr = NULL, senha[tamsenha];
    int pid_11a, cont=0, nb, np, senhac[tamsenha];
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

void espera(void) /*Funcao que espera a resposta do programa a*/
{
    printf("11b - entrando em estado de espera.\n");
    while(controle)
        sleep(1);
    controle = 1;
}

void sinal(void) /*funcao para mexer com o sinal*/
{
    controle = 0;
    signal(SIGUSR1, (void *) sinal);
}

void troca_cor(char senha[tamsenha], int a) /*funcao para trocar a cor de uma posicao*/
{
    int aux1=0;
    char aux2;
    while(aux1)
    {
        switch(rand()%4)
        {
            case 0:
                aux2 = 'R';
                break;
            case 1:
                aux2 = 'G';
                break;
            case 2:
                aux2 = 'Y';
                break;
            case 3:
                aux2 = 'B';
                break;
        }
        if(aux2 != senha[a])
            aux1 = 1;
    }
}

void inverte_posicao(char senha[tamsenha], int a, int b) /*funcao para inverter a posicao de duas cores*/
{
    char aux = senha[a];
    senha[a] = senha[b];
    senha[b] = aux;
}

int brancos(char senha[tamsenha]) /*Funcao para contar a quantidade de brancos*/
{
    int i = 0, cont = 0;
    for(; i<tamsenha-1; i++)
        if(senha[i] == 'B')
            cont++;
    return cont;
}

int pretos(char senha[tamsenha]) /*Funcao para contar a quantidade de pretos*/
{
    int i=0, cont=0;
    for(; i<tamsenha-1; i++)
        if(senha[i] == 'B')
            cont++;
    return cont;
}

void deduzindo(char senha[tamsenha], char result[tamsenha], struct jogoant m1)
{
    int aux;
    m1.nb = brancos(m1.ant);
    m1.np = pretos(m1.ant);
    if(m1.jogada != 1)
    {
        if(m1.nb > brancos(pmr))
        {
            if(m1.np > pretos(pmr))
            {
                /*ocorrencia impossivel para este codigo*/
            }
            else if(m1.np < pretos(pmr))
            {
                if(m1.a == m1.b)
                {
                    m1.jogocerteza[m1.a] = 1;
                    while(m1.a == m1.b)
                    {
                        m1.a = rand()%4;
                        while(m1.jogocerteza[m1.a])
                            m1.a = rand()%4;
                        m1.b = m1.a;
                        troca_cor(senha, m1.a);
                    }
                    else
                    {
                        m1.b = m1.a;
                        troca_cor(senha, m1.a);
                    }
                }
                else
                {
                    while(m1.jogocerteza[m1.a])
                        m1.a = rand()%4;
                    m1.b = m1.a;
                    troca_cor(senha, m1.a);
                }
            }
            else
            {
                if(m1.a == m1.b)
                {
                    m1.jogocerteza[m1.a] = 1;
                    while(m1.a == m1.b && m1.jogocerteza[m1.a])
                        m1.a = rand()%4;
                    m1.b = m1.a;
                    troca_cor(senha, m1.a)
                }
                else
                {
                    m1.b = m1.a;
                    troca_cor(senha, m1.a);
                }
            }
        }
        else if(m1.nb < brancos(pmr))
        {
            if(m1.np > pretos(pmr))
            {
                senha = m1.ant;
                if(m1.a == m1.b)
                {
                    m1.jogocerteza[m1.a] = 1;
                    while(m1.a == m1.b && m1.jogocerteza[m1.a])
                        m1.a = rand()%4;
                    m1.b = m1.a;
                    troca_cor(senha, m1.a);
                }
                else
                {
                    /*ocorrencia impossivel para este codigo*/
                }
            }
            else if(m1.np < pretos(pmr))
            {
                senha = m1.ant;
                if(m1.a == m1.b)
                {
                    m1.jogocerteza[m1.a] = 1;
                    while(m1.a == m1.b && m1.jogocerteza[m1.a])
                        m1.a = rand()%4;
                    m1.b = m1.a;
                    troca_cor(senha, m1.a);
                }
                else
                {
                    /*Ocorrencia impossivel para o codigo atual*/
                }
            }
            else
            {
                m1.jogocerteza[m1.a] = 1;
                while(m1.a == m1.b && m1.jogocerteza[m1.a])
                    m1.a = rand()%4;
                m1.b = m1.a;
                troca_cor(senha, m1.a);
            }
        }
        else
        {
            if(m1.np > pretos(pmr))
            {
                if(m1.a == m1.b)
                {
                    while(m1.b != m1.a && m1.jogocerteza[m1.b])
                        m1.b = rand()%4;
                    inverte_posicao(senha, m1.a, m1.b)
                }
                else
                {
                    /*Ocorrencia impossivel para o codigo atual*/
                }
            }
            else if(m1.np < pretos(pmr))
            {
                if(m1.a == m1.b)
                {
                    senha = m1.ant;
                    while(m1.a == m1.b && m1.jogocerteza[m1.a])
                        m1.a = rand()%4;
                    m1.b = m1.a;
                    troca_cor(senha, m1.a);
                }
                else
                {
                    /*Ocorrencia impossivel para o codigo atual*/
                }
            }
            else
            {
                while(m1.jogocerteza[m1.a])
                    m1.a = rand()%4;
                m1.b = m1.a;
                troca_cor(senha, m1.a);
            }
        }
    }
    else
        senha = "RGBY";
    m1.ant = senha;
    m1.resultant = pmr;
}
