#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <termios.h>
#include<stdlib.h>

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

void main()
{
        key_t key = 1234;
        int *stock;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        stock = shmat(shmid, NULL, 0);

        *stock = 1000;
        
        while(1){

            printf("Food stock : %d\n",*stock);
            printf("Choices\n");
            printf("1. Restock\n");
            printf("2. Exit\n");

            choose=mygetch();
            
            if(choose == '1'){
                *stock += 10;
            }
            if(choose == '2'){
                break;
            }system("clear");
        }
        shmdt(stock);
        shmctl(shmid, IPC_RMID, NULL);
}