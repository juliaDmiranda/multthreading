/*
cc -o pt -lpthread pt.c
*/
// #define MWIN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#ifdef MWIN
#include <windows.h>
#endif
#define ERRO 0
#define OK   1

#define TEMPO_SLEEP 10

// Variaveis globais

// declaracao dos mutexes



// controladores dos buffers

// variavel de controle do encerramento
int G_terminou;

// buffers de processamento
char G_be[6];
char G_bs[6];

void m_usleep(unsigned long pause)
{
#ifdef MWIN
   Sleep(pause);
#else
   usleep(pause*1000l);
#endif

   return;
}

void init_lock(pthread_mutex_t *lock) // inicializa as vari�veis de lock, fazer isto antes do inicio das threads
{
   pthread_mutex_init(lock,NULL);
   return;
}

void fini_lock(pthread_mutex_t *lock) // finalize as variaveis de lock, apos o pthread_kill
{
   pthread_mutex_destroy(lock);
   return;
}

int gerar_entrada()
{
    FILE *arq;
    int i;
    if ((arq = fopen("e.txt","wt"))==NULL)
    {
        printf("\nERRO: criando o arquivo de entrada (e.txt)\n");
        return(ERRO);
    }

    for (i = 1 ; i <= 1000; ++i)
    {
        fprintf(arq,"%05d\n",i);
    }

    fflush(arq);
    fclose(arq);

    return(OK);
}

void *escrita()
{

    return(NULL);
}

void *leitura()
{
   return(NULL);
}

void *processamento()
{
    return(NULL);
}

void finalizar()
{
    int nao_acabou = 1;
    while (nao_acabou)
    {
        m_usleep(50);
        pthread_mutex_lock(&G_p_fi);
        if (G_terminou == 0)
        {
            printf("\nEm finalizar... Acabou mesmo!");
            nao_acabou = 0;
        }
        pthread_mutex_unlock(&G_p_fi);
    }
    return;
}

int main(void)
{
    // declara��o das pthreads
    pthread_t entrada_thread;
    pthread_t processamento_thread;
    pthread_t saida_thread;


    // inicializacao do G_terminou
    G_terminou = 3;

    // inicializacao dos mutexes de lock

    // limpeza dos buffers

    // inicializacao dos controladores dos buffers

    // geracao do arquivo de entrada
    if (!gerar_entrada())
    {
        printf("\nVou sair");
        return(1);
    }

    // chamada das pthreads




    // Aguarda finalizar
    finalizar();

    // matar as pthreads
    pthread_kill(entrada_thread,0);
    pthread_kill(processamento_thread,0);
    pthread_kill(saida_thread,0);

    // finaliza��o dos mutexes

    return(0);
}
