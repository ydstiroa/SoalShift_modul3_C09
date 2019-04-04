#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
 
pthread_t tid1, tid2;
int WakeUp_Status,Spirit_Status;
int nomor,no2;
char aStatus[1000];
int WakeUp_Status = 0;
int Spirit_Status = 100;
int flg=0;
int i=0;
int j=0;
int flg_agmal=0;
int flg_iraj=0;

void* Agmal(void *arg)
{
    while(1){
        if(flg_agmal==1){
            sleep(10);
            flg_agmal=0;
            i=0;
        }
    }
    // if(strcmp(copy_of_line,"All_Status")==0){
    //     printf("%c",copy_of_line);    
    // }
}

void* Iraj(void *arg)
{
    while(1){
        if(flg_iraj==1){
            sleep(10);
            flg_iraj=0;
            j=0;
        }
    }
    // Spirit_Status = 100;
    // if(no2!=0){
    //     printf("%d\n",WakeUp_Status);
    //     printf("%d\n",Spirit_Status);
    // }
}
 
int main(void)
{
    char msg[50];
    char a;
    int tmp_agmal,tmp_iraj;

    pthread_create(&(tid1), NULL, Agmal, NULL);
    pthread_create(&(tid2), NULL, Iraj, NULL);

    while(1){
        if(flg==1){
            break;
        }
        printf("Agmal WakeUp_Status = %d\n",WakeUp_Status);
        printf("Iraj Spirit_Status = %d\n",Spirit_Status);
        scanf("%[^\n]s",msg);
        scanf("%c",&a);
        tmp_agmal = strcmp(msg,"Agmal Ayo Bangun");
        tmp_iraj = strcmp(msg,"Iraj Ayo Tidur");

        if(tmp_agmal == 0){
            if(flg_agmal==0){
                WakeUp_Status += 15;
                i++;
            }else{
                printf("Agmal Ayo Bangun disabled 10 s \n");
            }
        }
        if(tmp_iraj == 0){
            if(flg_iraj==0){
                Spirit_Status -= 20;
                j++;
            }else{
                printf("Fitur Iraj Ayo Tidur disabled 10 s \n");
            }
        }
        if(j==3){
            flg_agmal=1;
        }
        if(i==3){
            flg_iraj=1;
        }
        if(WakeUp_Status >= 100){
            printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
        }
        if(Spirit_Status <= 0){
            printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
        }
    }
    
    // aStatus="jaaa";
    // char *copy_of_line = strdup(aStatus);

    // if(copy_of_line){
    //     pthread_create(&(tid1), NULL, Agmal, copy_of_line);
    //     pthread_create(&(tid2), NULL, Iraj, copy_of_line);
 
    //     pthread_join(tid1, NULL);
    //     pthread_join(tid2, NULL);

    // }
    return 0;
}