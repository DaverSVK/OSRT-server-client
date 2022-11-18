#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<pthread.h>
#include <time.h>
#include<signal.h>

#define PORT 9502
timer_t cas_69;
int i=0;
void spustiCasovac();
timer_t vytvorCasovac();
void prebehol_cas();
char *join();
int *mattvor();
int *nfilter();
int *rfilter();
int *filter();
void *pripojenie_klienta(void*);
pthread_mutex_t mutexsem = PTHREAD_MUTEX_INITIALIZER;


int main(){

    pthread_t t1, t2;
    pthread_mutex_init(&mutexsem, NULL);
    int sock_server, ret, sock_klient, x;
    struct sockaddr_in server;
    struct sockaddr_in klient;
    socklen_t addr_size;

    
    char buffer[1024];
    //vytvorenie socketu pre server
    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_server < 0){
        printf("Server Socket [-].\n");
        exit(1);
    }
    printf("Server Socket [+].\n");

    memset(&server, '\0', sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY

    ret = bind(sock_server, (struct sockaddr*)&server, sizeof(server));
    if(ret < 0){
        printf("Binding [-].\n");
        exit(1);
    }
    printf("Binding [+] %d\n", PORT);

    if(listen(sock_server, 10) == 0){
        printf("Listening [+]\n");
    }else{
        printf("Listening [-].\n");
    }
    

    
        sock_klient = accept(sock_server, (struct sockaddr*)&klient, &addr_size);
        if(sock_klient < 0){
            printf("Pripojenie [-]");
            exit(1);
        }
        printf("Pripojenie od %s:%d\n ", inet_ntoa(klient.sin_addr), ntohs(klient.sin_port));

        if(pthread_create(&t1, NULL, pripojenie_klienta, (void*) &sock_klient)<0) {
            printf("Vytvorenie threadu [-]");
            exit(1);
        }
        sock_klient = accept(sock_server, (struct sockaddr*)&klient, &addr_size);
        if(sock_klient < 0){
            printf("Pripojenie [-]");
            exit(1);
        }
        printf("Pripojenie od %s:%d\n", inet_ntoa(klient.sin_addr), ntohs(klient.sin_port));

        if(pthread_create(&t2, NULL, pripojenie_klienta, (void*) &sock_klient)<0) {
            printf("Vytvorenie threadu [-]");
            exit(1);
        }
        pthread_join( t1 , NULL);
        pthread_join( t2, NULL);

        sleep(2);
        //casovac
        signal(SIGUSR1,prebehol_cas);
        timer_t cas_69=vytvorCasovac(SIGUSR1);
        spustiCasovac(cas_69);
        getchar();

    close(sock_klient);
    close(sock_server);

    return 0;
}

void *pripojenie_klienta(void *socket_desc)
{
   
    int *p;
    p=mattvor();
    //mutex semafor
    pthread_mutex_lock(&mutexsem);
    i ++;
    pthread_mutex_unlock(&mutexsem);
    int sock = *(int*)socket_desc, vel=0;
    int read_size, a=i;
    char *message ,matica[2000], client_message[2000];
    char par[10]="parne", npar[10]="neparne", hist[10]="history",v1[10]="1",v2[10]="2",v3[10]="3",v4[10]="4",v5[10]="5",v6[10]="6",v7[10]="7",v8[10]="8",v9[10]="9"; 
    join(matica, sizeof matica, p, 100);
    //prva sprava
    message = "Vitaj na serery s filtrom matic, zvol si jednu s moznosti:\n\t\t  parne, neparne, 1-9(delitelne)), history\n";
    send(sock , message , strlen(message),0);
    printf("\nServer: \t%s\n", message);
    //sleep(1);
    //printf("%s",matica); len kontrola
    send(sock , matica, 2000,0);
    printf("\nServer: \t%s\n", matica);
    
    //send(sock , ch , strlen(message),0);
    bzero(matica, sizeof(matica));
    //Receive a message from client
    recv(sock , client_message , 2000 , 0);
    printf("Client%d: \t%s\n",a, client_message);\
    if (strcmp(client_message,par)==0){ 
        message="Zvolil si parne\n";
        printf("\t\n");
        p=filter(p,&vel);
        join(matica, sizeof matica, p, vel);
        
    }
       
    if (strcmp(client_message,npar)==0){
        message="Zvolil si neparne\n";
        printf("\t\n");
        p=nfilter(p,&vel);
        join(matica, sizeof matica, p, vel);
    }
    if (strcmp(client_message,v1)==0||strcmp(client_message,v2)==0||strcmp(client_message,v3)==0||strcmp(client_message,v4)==0||strcmp(client_message,v5)==0||strcmp(client_message,v6)==0||strcmp(client_message,v7)==0||strcmp(client_message,v8)==0||strcmp(client_message,v9)==0){
        message="Zvolil si vlasne cislo\n";
        int volit = atoi(client_message);
        p=rfilter(p,&vel,volit);
        join(matica, sizeof matica, p, vel);
    }
    if (strcmp(client_message,hist)==0){
        message="Naposledy pouzite:\n";
    
    }

    //vypis moznosti a vysledku
    send(sock , message , strlen(message),0);
    printf("Server: \t%s\n", message);

    send(sock , matica, 2000,0);
    printf("Server: \t%s\n", matica);

    message="\tMatica be here\n";
    send(sock , message , strlen(message),0);
    printf("Server: \t%s\n", message);
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    free(p);
    pthread_mutex_lock(&mutexsem);
    i --;
    pthread_mutex_unlock(&mutexsem);
    pthread_exit(NULL);
}

//filter na parne cisla
int *filter(int* p,int *vel)
{
    (*vel)=0;
    for (int x=0;x<100;x++)
    {
        if (p[x]%2==0)
            (*vel)++;
    }
    int *array=(int *)malloc((*vel)*sizeof(int));
    int count=0;
    for (int x=0;x<100;x++)
    {
        if (p[x]%2==0)
        {
            array[count++]=p[x];
        }
    }
    return array;
}
//filter na neparne cisla
int *rfilter(int* p,int *vel,int delic)
{
    (*vel)=0;
    for (int x=0;x<100;x++)
    {
        if (p[x]%delic==0)
            (*vel)++;
    }
    int *array=(int *)malloc((*vel)*sizeof(int));
    int count=0;
    for (int x=0;x<100;x++)
    {
        if (p[x]%delic==0)
        {
            array[count++]=p[x];
        }
    }
    return array;
}
//filter podla vyberu delitela
int *nfilter(int* p,int *vel)
{
    (*vel)=0;
    for (int x=0;x<100;x++)
    {
        if (p[x]%2!=0)
            (*vel)++;
    }
    int *array=(int *)malloc((*vel)*sizeof(int));
    int count=0;
    for (int x=0;x<100;x++)
    {
        if (p[x]%2!=0)
        {
            array[count++]=p[x];
        }
    }
    return array;
}

//tvorenie matice na precu filtrov
int *mattvor()
{
    int *array=(int *)malloc(100*sizeof(int));
    for (int x=0; x<100; x++)
        array[x]=1+x;//rand() % 2000;

    return array;
}

//funkcia na premenu int array do string
char *join(char *dest, int size, const int *array, int count) {
    if (size == 1) {
        dest[0] = '\0';
        return dest;
    }
    int pos = 0;
    dest[pos++] = '[';
    dest[pos] = '\0';
    for (int x = 0; pos < size && x < count; x++) {
        int len = snprintf(dest + pos, size - pos, "%d%s",
                           array[x], (x + 1 < count) ? ", " : "]");
        if (len < 0)
            return NULL;
        pos += len;
    }
    return dest;
}
timer_t vytvorCasovac(int signal)
{
  struct sigevent kam;
  kam.sigev_notify=SIGEV_SIGNAL;
  kam.sigev_signo=signal;
  
  timer_t casovac;
  timer_create(CLOCK_REALTIME, &kam, &casovac);
  return(casovac); 
}

void spustiCasovac(timer_t casovac)
{
  struct itimerspec casik;
  casik.it_value.tv_sec=5;
  casik.it_value.tv_nsec=0;
  casik.it_interval.tv_sec=5;
  casik.it_interval.tv_nsec=0;
  timer_settime(casovac,CLOCK_REALTIME,&casik,NULL);
}

void prebehol_cas(){
    printf("preslo dalsich 5s\n");
}
/*
        //clear the message buffer
        memset(client_message, 0, 2000);
    }
*/
//int mattvor(int mat[100])