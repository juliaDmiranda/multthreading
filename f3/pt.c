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
pthread_mutex_t G_p_fi;

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
    memset(buffer, 0, BUFFER_TAM);
}

void inverte_string(char* str_src) {
    char tmp[BUFFER_TAM];
    int buffer_tam = BUFFER_TAM - 1;

    for (int i = 0; i < buffer_tam; i++) {
        tmp[i] = str_src[buffer_tam - 1 - i];  // Copia os caracteres na ordem inversa
    }

    tmp[buffer_tam] = '\0';  // Adiciona o caractere nulo ao final da string invertida

    strcpy(str_src, tmp);
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

    for (i = 1 ; i <= GERA_TAM; ++i)
    {
        fprintf(arq,"%05d\n",i);
    }

    fflush(arq);
    fclose(arq);

    return(OK);
}

void *escrita()
{
    FILE *arq;
    int continua = 1;
    
    //abrir o arquivo de saida 
    if((arq=fopen("s.txt", "wt"))==NULL){
        printf("\n Erro: criando o arquivo de saída (s.txt)\n");
        pthread_exit(NULL);
    }

    while(continua){

        pthread_mutex_lock(& buffer_s_mutex);

        // escrita no arquivo de saída
        if(!buffer_s_esta_vazio){
            //escreve no buffer
            fprintf(arq, "%s\n",buffer_s);

            fflush(arq);
            
            // limpa buffer
            limpa_buffer(buffer_s);

            //marcar o buffer de saida como vazio 
            buffer_s_esta_vazio = 1;
            pthread_mutex_unlock(& buffer_s_mutex);
        }
        else{
            // condição de término: buffer s vazio
            //conferir se terminou é 1
            
            pthread_mutex_lock(&terminou_mutex);

            if(terminou == 1){
                terminou--;
                continua = 0;

                pthread_mutex_unlock(& buffer_s_mutex);

                pthread_mutex_unlock(&terminou_mutex);

                break;

            }

            pthread_mutex_unlock(&terminou_mutex);
        }
        pthread_mutex_unlock(& buffer_s_mutex);

        msleep();
    }

    fclose(arq);

    //for(;;);
    
    return(NULL);
}

void *leitura()
{
    FILE *arq1;
    char* fim_de_arquivo;
    int continua = 1;

    if ((arq1 = fopen("e.txt","rt"))==NULL)
    {
        printf("\nERRO: criando o arquivo de entrada (e.txt)\n");
        return(ERRO);
    }

    while(continua) {
        //bloqueia buffer de entrada
        pthread_mutex_lock(&buffer_e_mutex);

        if(buffer_e_esta_vazio){
            if(fgets(buffer_e, BUFFER_TAM, arq1) == NULL){
                continua = 0;
                terminou --;
                printf("\nLeitura terminou: %d\n\n", terminou);
            }
            
                pthread_mutex_unlock(&buffer_e_mutex);
                break;

            }
            
            //informa que o buffer_e está cheio
            buffer_e_esta_vazio = 0;
            pthread_mutex_unlock(&buffer_e_mutex);
        }
        //desbloqueia mutex
        pthread_mutex_unlock(&buffer_e_mutex);
        
        msleep();
    }
    fclose(arq1);

    //for(;;);
   return(NULL);   
}

void *processamento(){
    char buffer_tmp[BUFFER_TAM];

    int continua = 1;

    while(continua){
        // bloqueia buffer de entrada
        pthread_mutex_lock(&buffer_e_mutex);

        if(!buffer_e_esta_vazio){

            limpa_buffer(buffer_tmp);

            // lê do buffer de entrada um novo registro
            strcpy(buffer_tmp, buffer_e);
            
            // limpa o buffer de entrada
            limpa_buffer(buffer_e);

            buffer_e_esta_vazio = 1;
            
            pthread_mutex_unlock(&buffer_e_mutex);


            inverte_string(buffer_tmp);
            

            while(1){
                pthread_mutex_lock(&buffer_s_mutex);
                if(buffer_s_esta_vazio){
                    strcpy(buffer_s, buffer_tmp);
                    buffer_s_esta_vazio = 0;
                     pthread_mutex_unlock(&buffer_s_mutex);

                    break;
                }
                pthread_mutex_unlock(&buffer_s_mutex);
            }
            
        }else{
            pthread_mutex_unlock(&buffer_e_mutex);

            pthread_mutex_lock(&terminou_mutex);
            if(terminou == 2){
                terminou --;
                continua = 0;

            }
            pthread_mutex_unlock(&terminou_mutex);
        }

        msleep(); // onde por performance *?**
    }


    //for(;;);

    return(NULL);
}
void finalizar()
{
    int nao_acabou = 1;
    while (nao_acabou)
    {
        msleep();
        pthread_mutex_lock(&terminou_mutex);
        if (terminou == 0)
        {
            printf("\nEm finalizar... Acabou mesmo!");
            nao_acabou = 0;
        }
        pthread_mutex_unlock(&terminou_mutex);
    }
    //return;
}


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


    // inicializacao do terminou
    terminou = 3;

    // inicializacao dos mutexes de lock
    init_lock(&buffer_e_mutex);
    init_lock(&buffer_s_mutex);
    init_lock(&terminou_mutex);
    
    // limpeza dos buffers
    limpa_buffer(buffer_e);
    limpa_buffer(buffer_s);

    // inicializacao dos controladores dos buffers
    buffer_e_esta_vazio = 1;
    buffer_s_esta_vazio = 1;

    // geracao do arquivo de entrada
    if (!gerar_entrada())
    {
        printf("\nVou sair");
        return(1);
    }

    // chamada das pthreads
    chama_threads(&entrada_thread, escrita);
    chama_threads(&processamento_thread, processamento);
    chama_threads(&saida_thread, leitura);



    // Aguarda finalizar
    finalizar();

    // matar as pthreads
    pthread_join(entrada_thread, NULL);
    pthread_join(processamento_thread, NULL);
    pthread_join(saida_thread, NULL);

    // finaliza��o dos mutexes
    fini_lock(&buffer_e_mutex);
    fini_lock(&buffer_s_mutex);
    fini_lock(&terminou_mutex);
    return(0);
}
