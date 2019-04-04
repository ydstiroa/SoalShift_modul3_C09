#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <termios.h>
#include<sys/ipc.h>
#include <sys/shm.h>
 
pthread_t tid1, tid2, tid3, tid4;
int hunger_status = 200;
int hygiene_status = 100;
int health_status = 300;
int enemy_health = 100;
int food_stock = 100;
int temp;
char choose;

int mygetch(void)
{
    struct termios oldt,
    newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

void belanja(){
    
    key_t key = 1234;
    int *stock;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);

    while(1){
        printf("Shop Mode\n");
        printf("Shop food stock : %d\n",*stock);
        printf("Your food stock : %d\n",food_stock);
        printf("1. Buy\n");
        printf("2. Back\n");

        choose=mygetch();

        if(choose=='1'){
            if(*stock > 0){
                food_stock += 10;
                *stock -= 10;
            }else{
                food_stock +=0;
                *stock -= 0;
            }
        }
        if(choose=='2'){
            break;
        }system("clear");
    }
    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
}

void fight(){
    while(1){
        printf("Battle Mode\n");
        printf("Monster's Health : %d\n",health_status);
        printf("Enemy Health : %d\n",enemy_health);
        printf("Choices\n");
        printf("1. Attack\n");
        printf("2. Run\n");

        choose=mygetch();

        if(choose=='1'){
            health_status -= 20;
            enemy_health -= 20;
        }
        if(choose=='2'){
            break;
        }system("clear");
    }
}

void pick(){
        while(1){
            printf("Standby Mode\n");
            printf("Health      : %d\n",health_status);
            printf("Hunger      : %d\n",hunger_status);
            printf("Hygiene     : %d\n",hygiene_status);
            printf("Food left   : %d\n",food_stock);
            //printf("Bath will be ready in %d",);

            printf("Choices \n");
            printf("1. Eat \n");
            printf("2. Bath \n");
            printf("3. Battle \n");
            printf("4. Shop \n");
            printf("5. Exit \n");

            choose= mygetch();

            if(choose=='1'){
                hunger_status += 15; 
                food_stock -=15; 
                //printf("%d\n",hunger_status);
                sleep(1);
            }
            if(choose=='2'){
                hygiene_status += 30;
                //printf("%d",hygiene_status);
                sleep(20);
            }
            if(choose=='3'){
                system("clear");
                fight();
            }
            if(choose=='4'){
                system("clear");
                belanja();
            }
            if(choose=='5'){
                break;
            }
            if(hunger_status==0){
                    printf("Monstermu Mati Kelaparan\n");
                    break;
            }
            if(hygiene_status==0){
                    printf("Monstermu Mati\n");
                    break;
            }
            system("clear");
        }
} 

void* makan(void *arg)
{
    while(1){
        hunger_status -= 5;
        sleep(10);
    }
    // status = 0;
    // printf("Masukan nomor : ");
    // scanf("%d", &nomor);
    // status = 1;
    // return NULL;
}

void* mandi(void *arg)
{
    while(1){
        hygiene_status -= 10;
        sleep(30);
    }
    // while(status != 1)
    // {
    // }
    // printf("Nomor %d\n", nomor);
}

void* regen(void *arg){
    while(1){
        health_status += 5;
        sleep(10);
    }
}
 
int main(void)
{
    char msg;
    char name[1000];

    printf("Enter Name : ");
    scanf("%s",name);

    printf("Your Mosnter Name is : ");
    printf("%s\n",name);

    pthread_create(&(tid1), NULL, makan, NULL);
    pthread_create(&(tid2), NULL, mandi, NULL);
    pthread_create(&(tid3), NULL, regen, NULL);
    //pthread_create(&(tid4), NULL, stok, NULL);
    pick();
}