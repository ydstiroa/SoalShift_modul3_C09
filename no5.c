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
int siap = 0;
char choose;
char name[1000];

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
                system("clear");
                printf("stock tidak cukup");
            }
        }
        if(choose=='2'){
            break;
        }system("clear");
    }

    // printf("Program 1 : %d\n", *value);
	// *value = 30;

    // sleep(5);
    
    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
}

void fight(){
    enemy_health=100;
    while(1){
        printf("Battle Mode\n");
        printf("%s's Health : %d\n",name,health_status);
        printf("Enemy Health : %d\n",enemy_health);
        printf("Choices\n");
        printf("1. Attack\n");
        printf("2. Run\n");

        choose=mygetch();

        if(choose=='1'){
            health_status -= 20;
            enemy_health -= 20;

            if(enemy_health == 0){
                printf("Congratulation\n");
                printf("Your Enemy is Dead\n");
                sleep(2);
                break;
            }
            // printf("%d",health_status);
            // printf("%d",enemy_health);
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
            printf("Food left   : %d\n", food_stock);
            if(siap > 0){
                printf("Bath will be ready in %d\n",siap);
            }else{
                printf("Bath is ready\n");
            }
            

            printf("Choices \n");
            printf("1. Eat \n");
            printf("2. Bath \n");
            printf("3. Battle \n");
            printf("4. Shop \n");
            printf("5. Exit \n");

            choose= mygetch();

            if(choose=='1'){
                if(food_stock > 0){  
                    //printf("%d\n",hunger_status);
                    if(food_stock-15 >=0){
                        hunger_status += 15;
                        food_stock -=15;
                        sleep(1);
                    }else{
                        hunger_status += food_stock;
                        food_stock = 0;
                    }
                }else{
                    printf("\nMakanan Tidak Tersedia!\n");
                    printf("Segera Restock Makanan\n");
                    sleep(2);
                }
            }
            if(choose=='2'){
                if(hygiene_status<100){
                    if(siap==0){
                        hygiene_status += 30;
                        if(hygiene_status > 100){
                            hygiene_status = 100;
                        }
                        siap=20;
                    }   
                }else{
                    system("clear");
                    printf("tidak bisa\n");
                    sleep(2);
                    system("clear");
                }
                //printf("%d",hygiene_status);
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
                printf("%s Mati Kelaparan\n",name);
                break;
            }
            if(hygiene_status==0){
                printf("%s Mati Tidak Terawat\n",name);
                break;
            }
            system("clear");
        }
} 

void* ready(void *arg){
    while(1){
        if(siap > 0){
            siap--;
            sleep(1);
        }
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

    printf("Enter Name : ");
    scanf("%s",name);

    printf("Your Mosnter Name is : ");
    printf("%s\n",name);
    sleep(2);

    pthread_create(&(tid1), NULL, makan, NULL);
    pthread_create(&(tid2), NULL, mandi, NULL);
    pthread_create(&(tid3), NULL, regen, NULL);
    pthread_create(&(tid4), NULL, ready, NULL);
    pick();
    
    // pthread_join(tid1, NULL);
    // pthread_join(tid2, NULL);
 
    // return 0;
}
