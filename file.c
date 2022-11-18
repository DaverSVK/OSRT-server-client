#include <stdio.h>
#include<pthread.h>
#include <semaphore.h>

sem_t bin_sem;
void *kluc();
void *sifra();

//main
int main() {

//semafor
    int resume;
    resume = sem_init(&bin_sem, 0, 0);
//sem idu thready namiesto funkcii
    pthread_t t1, t2;

    pthread_create(&t1,NULL,&sifra,NULL);
    pthread_create(&t2,NULL,&kluc,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&bin_sem);
    return 0;
}
//funkcia na zasifrovanie
void *sifra()
{

    FILE *sub,*sub2;
    int c; 
    sub = fopen("sprava.txt", "r");
    if (sub == NULL)
    {
        printf("neotvorim.\n");
    }
    sub2 = fopen("sifra.txt", "w");
    if (sub2 == NULL)
    {
        printf("nevytvorim .\n");
    }
    while((c=fgetc(sub))!=EOF)
    {
        if ((c<='z'&& c>='a')||(c<='Z'&& c>='A')){
            if(c<='z'&& c>='a'){
                c+=7;
                if (c>'z')
                    c-=('z'-'a'+1);}
            else{
                c+=7;
                if (c>'Z')
                    c-=('Z'-'A'+1);
                }
        }
        fprintf(sub2,"%c",c);
    }
    sem_post(&bin_sem);
    fclose(sub);
    fclose(sub2);

}
//funkcia na rozsifrovanie
void *kluc() {
    sem_wait(&bin_sem);
    
    FILE *sub,*sub2,*sub3;
    int c,x;
    sub = fopen("sprava.txt", "r");
    if (sub == NULL)
    {
        printf("neotvorim original.\n");
    }
    sub2 = fopen("sifra.txt", "r");
    if (sub2 == NULL)
    {
        printf("neotvorim sifru.\n");
    }
    sub3 = fopen("bingo.txt", "w");
    if (sub3 == NULL)
    {

        printf("nevytvorim .\n");
    }
    while((c=fgetc(sub2))!=EOF)
    {
        if ((c<='z'&& c>='a')||(c<='Z'&& c>='A')){
            if(c<='z'&& c>='a'){
                c-=7;
                if (c<'a')
                    c+=('z'-'a'+1);}
            else{
                c-=7;
                if (c<'A')
                    c+=('Z'-'A'+1);
            }
        }
        x=fgetc(sub);
        fprintf(sub3,"%c",c);
        if (c==x)
            printf("zhoduje sa\n");
        else
            printf("nezhoduje sa\n");
    }

    fclose(sub);
    fclose(sub2);
    fclose(sub3);
}