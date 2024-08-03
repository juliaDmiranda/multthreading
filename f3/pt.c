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

/// Variaveis globais

// declaracao dos mutexes
pthread_mutex_t buffer_e_mutex;
pthread_mutex_t buffer_s_mutex;

// controladores dos buffers que indica se estão vazios
int buffer_e_disponivel;
int buffer_s_disponivel;

// variavel de controle do encerramento
int G_terminou;

// buffers de processamento
char buffer_e[6];
char buffer_s[6];

void msleep() {struct timespec start, end;
    unsigned long elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);  // Obtém o tempo inicial

    do {
        clock_gettime(CLOCK_MONOTONIC, &end);  // Obtém o tempo atual
        elapsed = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
    } while (elapsed < TEMPO_SLEEP);  // Continua até o tempo decorrido ser o suficiente}

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

void limpa_buffer(char* buffer){
    memset(buffer, '\0', BUFFER_TAM);
}

void inverte_string(const char* str_src, char* str_dest) {
    int buffer_tam = BUFFER_TAM - 1;
    for (int i = 0; i < buffer_tam; i++) {
        str_dest[i] = str_src[buffer_tam - 1 - i];  // Copia os caracteres na ordem inversa
    }

    str_dest[buffer_tam] = '\0';  // Adiciona o caractere nulo ao final da string invertida
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
    // escrita no buffer de entrada
    if(buffer_e_disponivel){
        //escreve no buffer
    }

    return(NULL);
}

void *leitura()
{
    // escrita no arquivo de saída
    if(buffer_s_disponivel){
        //escreve no buffer
    }
   return(NULL);
}

void *processamento(){
    // bloqueia buffer de entrada
    pthread_mutex_lock(&mutex_buffer_e);

    if(esta_vazio_buffer_e){
        pthread_mutex_unlock(&mutex_buffer_e);
    }
    else{
        char buffer_tmp[BUFFER_TAM];

        limpa_buffer(buffer_tmp);

        // lê do buffer de entrada um novo registro
        strcpy(buffer_tmp, buffer_e);
        
        // limpa o buffer de entrada
        limpa_buffer(buffer_e);
        
        pthread_mutex_unlock(&mutex_buffer_e);

        pthread_mutex_lock(&mutex_buffer_s);
        
        inverte_string(buffer_tmp, buffer_s);

        esta_vazio_buffer_s = 0;

        pthread_mutex_unlock(&mutex_buffer_s);
    
    }
    msleep();

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
*/

void chama_threads(pthread_t *nome_pthread, void *(*funcao)(void *)) {
    if (pthread_create(nome_pthread, NULL, funcao, NULL)) {
        printf("\nERRO: criando thread.\n");
        exit (1);
    }
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
    /*if (!gerar_entrada())
    {
        printf("\nVou sair");
        return(1);
    }*/

    // chamada das pthreads
    chama_threads(&entrada_thread, escrita);
    chama_threads(&processamento_thread, processamento);
    chama_threads(&saida_thread, leitura);



    // Aguarda finalizar
    //finalizar();

    // matar as pthreads
    pthread_join(entrada_thread, NULL);
    pthread_join(processamento_thread, NULL);
    pthread_join(saida_thread, NULL);

    // finaliza��o dos mutexes

    return(0);
}
