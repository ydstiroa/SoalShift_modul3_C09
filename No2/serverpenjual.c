#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<pthread.h>
#define PORT 8081
int *stok;
pthread_t tid1;
void* tulis(void *arg)
{
    int i = 0;
    while(1){
        printf("stok saat ini : %d\n", *stok);
        sleep(5);
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *invalid = "invalid input     ";
    char *sukses = "input ditambah    ";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    key_t key = 1234;
    

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666); 
    stok = shmat(shmid, NULL, 0);
    pthread_create(&(tid1), NULL, tulis, NULL);
    while(1){
        valread = read( new_socket , buffer, 1024);
        if(strcmp(buffer,"tambah")==0){
            *stok+=1;
            send(new_socket , sukses , strlen(sukses) , 0 );
        }
        else{
            send(new_socket , invalid , strlen(invalid) , 0 );
        }
        memset(buffer,0,1024);
        //jumlah = *stok;
        
        //shmdt(stok);
        //shmctl(shmid, IPC_RMID, NULL);
        //printf("%s\n",buffer );
        //send(new_socket , hello , strlen(hello) , 0 );
    }
    return 0;
}
