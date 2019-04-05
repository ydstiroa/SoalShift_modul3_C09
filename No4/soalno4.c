#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
 
pthread_t tid[2];
 
void* catat1(void *arg)
{
    system("ps -aux | head -11 | tail -10 > /home/najaslanardo/Documents/FolderProses1/SimpanProses1.txt");
}

void* catat2(void *arg)
{
    system("ps -aux | head -11 | tail -10 > /home/najaslanardo/Documents/FolderProses2/SimpanProses2.txt");
}

void* kompres1(void *arg)
{
    system("cd /home/najaslanardo/Documents/FolderProses1/ && zip -m KompresProses1.zip SimpanProses1.txt");
}

void* kompres2(void *arg)
{
    system("cd /home/najaslanardo/Documents/FolderProses2/ && zip -m KompresProses2.zip SimpanProses2.txt");
}

void* ekstrak1(void *arg)
{
    system("cd /home/najaslanardo/Documents/FolderProses1/ && unzip -o KompresProses1.zip");
}

void* ekstrak2(void *arg)
{
    system("cd /home/najaslanardo/Documents/FolderProses2/ && unzip -o KompresProses2.zip");
}


int main(void)
{

    pthread_create(&(tid[0]), NULL, &catat1, NULL);
    pthread_create(&(tid[1]), NULL, &catat2, NULL);
 
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
 
    pthread_create(&(tid[0]), NULL, kompres1, NULL);
    pthread_create(&(tid[1]), NULL, kompres2, NULL);
 
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    printf("\nMenunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);
    pthread_create(&(tid[0]), NULL, ekstrak1, NULL);
    pthread_create(&(tid[1]), NULL, ekstrak2, NULL);
 
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}