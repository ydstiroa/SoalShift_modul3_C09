#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

int prime_arr[1000],n=0;
pthread_t tid[50];

struct range{
	int high;
};

void* tulis(void* args){
    struct range*extract =(struct range*)args;
    unsigned long long hasil=1;
    //printf("%d \n",extract->high);

     for(int j=1;j<=extract->high;j++){
         //printf("%d",j);
         hasil=hasil*j;
     }
     printf("%d! = %llu\n",extract->high,hasil);
}

void urut(){
    int x;
     for (int i=0;i<n-1;++i) {
        for (int j=i+1; j<n;++j){
        	if (prime_arr[i] > prime_arr[j]) {
                x =  prime_arr[i];
            	prime_arr[i] = prime_arr[j];
        		prime_arr[j] = x;
           	}    
        }     
    }
}

int main(){
    int a=0,temp;
    //scanf("%d",&t);
    char input;
    struct range rentang;
    rentang.high=n;

    // for(int i=1;i<=t; i++){
    //     scanf("%d",&n);
    //     pthread_create(&tid[i], NULL, &tulis,(void *)&rentang);
    //     rentang.high=n;
    // }   
    	
  	do {
      	scanf("%d%c", &prime_arr[a], &input);
    	a++;
	    n++;
  	} while(input != '\n');

    urut();

   	a=0;

  	while(a<n) {
 		rentang.high = prime_arr[a];
 		temp=prime_arr[a];
 		pthread_create(&tid[temp], NULL, &tulis,(void *)&rentang);
        pthread_join(tid[temp],NULL);
    	a++;
  	}
    
    return 0;
}