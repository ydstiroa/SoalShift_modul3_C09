# SoalShift_Modul3_C09

## Oleh
   * Najatul Muslim Dinatra	05111740000079
   * Yudhistiro Adi Nugroho	05111740000165

### No 1
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:

   ./faktorial 5 3 4
    
   3! = 6
   
   4! = 24
   
   5! = 120

Jawab :

Gunakan struct yang berisi satu value int yaitu high untuk menyimpan setiap angka dari inputan untuk di proses factorialnya. Terima input angka dipisah dengan spasi dan berakhir jika inputan adalah enter, setiap inputan disimpan dalam array kemudian diurutkan. Proses masing masing factorial dari setiap angka inputan secara paralel dengan thread kemudian di print.

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

Code lengkapnya : [Soal 1](/No1/soalno1.c) 

### No 2
Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
  
  a.Terdapat 2 server: server penjual dan server pembeli
  
  b. server hanya bisa terkoneksi dengan 1 client
  
  c.Server penjual dan server pembeli memiliki stok barang yang selalu sama
  
  d.Client yang terkoneksi ke server penjual hanya bisa menambah stok
  
   -Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
  
  e.Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
    
   -Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1
  
  f.Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak     berdasarkan ketersediaan stok
    
   -Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
   
   -Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
  
  g.Server penjual akan mencetak stok saat ini setiap 5 detik sekali
  
  h.Menggunakan thread, socket, shared memory
  
Jawab :

Buat 4 program, serverpembeli, serverpenjual, clientpembeli, dan client penjual. Socket pada port 8080 pada pembeli dan 8081 pada penjual. Program terus menerima input sampai program diberhentikan, gunakan shared memory untuk membagi variabel stok agar server pembeli dengan server penjual menyimpan stok dengan jumlah yang sama, kemudian pada server penjual lakukan thread untuk menampilkan jumlah stok setiap 5 detik.

Pada serverpembeli hanya menerima pesan dari clientpembeli, jika valid yaitu beli maka stok dikurangi.

	    #include <stdio.h>
	    #include <sys/socket.h>
	    #include <stdlib.h>
	    #include <netinet/in.h>
	    #include <string.h>
	    #include <unistd.h>
	    #include <sys/ipc.h>
	    #include <sys/shm.h>
	    #define PORT 8080


	int main(int argc, char const *argv[]) {
		int server_fd, new_socket, valread;
		struct sockaddr_in address;
		int opt = 1;
		int addrlen = sizeof(address);
		char buffer[1024] = {0};
		char *berhasil = "transaksi berhasil";
		char *gagal = "transaksi gagal   ";
		char *invalid = "invalid input     ";

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
	    int *stok;

	    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666); 
	    stok = shmat(shmid, NULL, 0);

	    while(1){
		valread = read( new_socket , buffer, 1024);
		if(strcmp(buffer,"beli")==0){
		    if(*stok>0){*stok-=1;
			send(new_socket , berhasil , strlen(berhasil) , 0 );
		    }
		    else{
			send(new_socket , gagal , strlen(gagal) , 0 );
		    }
		}
		else{
		    send(new_socket , invalid , strlen(invalid) , 0 );
		}
		//memset(buffer,0,1024);
		//shmdt(stok);
		//shmctl(shmid, IPC_RMID, NULL);
		//printf("%s\n",buffer );
		//send(new_socket , hello , strlen(hello) , 0 );
	    }
		return 0;
	}

Pada serverpenjual hanya menerima pesan dari client penjual jika tambah maka stok akan ditambahkan 1.Kemudian print jumlah stok setiap 5 detik.

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

Pada clientpembeli kirim pesan beli pada serverpembeli.

    #include <stdio.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #define PORT 8080

    int main(int argc, char const *argv[]) {
        struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        char beli[9];
        char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    while(1){
        scanf("%s", beli);
        //printf("%s\n", beli);
        send(sock , beli , strlen(beli) , 0 );
        //memset(beli,0,sizeof(beli));
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
        memset(buffer,0,1024);
    }    
    return 0;
	}

Pada clientpenjual kirim pesan tambah pada serverpenjual agar stok ditambah.

    #include <stdio.h>
    #include <sys/socket.h>
    #include <stdlib.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #define PORT 8081

    int main(int argc, char const *argv[]) {
        struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        char tambah[9];
        char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    while(1){
        scanf("%s", tambah);
        send(sock , tambah , strlen(tambah) , 0 );
        //memset(tambah,0,sizeof(tambah));
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
        memset(buffer,0,1024);
    }    
    return 0;
	}

Code lengkapnya : [Client Pembeli](/No2/clientpembeli.c) 

Code lengkapnya : [Client Penjual](/No2/clientpenjual.c) 

Code lengkapnya : [Server Pembeli](/No2/serverpembeli.c) 

Code lengkapnya : [Server Penjual](/No2/serverpenjual.c)

### No 3
Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:

a.Terdapat 2 karakter Agmal dan Iraj

b.Kedua karakter memiliki status yang unik
  
 -Agmal mempunyai WakeUp_Status, di awal program memiliki status 0

 -Iraj memiliki Spirit_Status, di awal program memiliki status 100

 -Terdapat 3 Fitur utama
 
   ~All Status, yaitu menampilkan status kedua sahabat
   
   Ex: Agmal WakeUp_Status = 75 
       Iraj Spirit_Status = 30

   ~“Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
   
   ~“Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point

  -Terdapat Kasus yang unik dimana:
  
   ~Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)

   ~Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)

  -Program akan berhenti jika Salah Satu :
   
   ~WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)

   ~Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)

Jawab :


Pertama-tama kita harus membuat thread terlebih dahulu untuk agmal dan juga iraj dimana fungsi thread tesebut nantinya akan digunakan untuk mendisable input tertentu jika sudah melebihi 3 kali

    pthread_create(&(tid1), NULL, Agmal, NULL);
    pthread_create(&(tid2), NULL, Iraj, NULL);

     void* Agmal(void *arg)
    {
        while(1){
            if(flg_agmal==1){
                sleep(10);
                flg_agmal=0;
                j=0;
            }
        }
    }

    void* Iraj(void *arg)
    {
        while(1){
            if(flg_iraj==1){
                sleep(10);
                flg_iraj=0;
                i=0;
            }
        }
    }
    
Setelah itu memasukkan input dan mengcompare stringnya yang yang disimpan dalam sebuah variable baru dan nantinya berguna untuk kondisi lainnya

    printf("Agmal WakeUp_Status = %d\n",WakeUp_Status);
            printf("Iraj Spirit_Status = %d\n",Spirit_Status);
            scanf("%[^\n]s",msg);
            scanf("%c",&a);
            tmp_agmal = strcmp(msg,"Agmal Ayo Bangun");
            tmp_iraj = strcmp(msg,"Iraj Ayo Tidur");

Lalu cek apakah string yang di input dan sudah dimasukkan ke variable baru setelah di compare tersebut true, jika iya maka membuat spirit_status dari iraj berkurang atau membuat wakeup_status dari agmal bertambah dan tentu juga meng-increment suatu variable i dan j untuk dipakai di kondisi lainnya yaitu mendisable input tertentu selama 10s

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

Buat kondisi untuk mendisable selama 10s menggunakan variable i dan j yang sudah di deklarasi dan di increment sebelumnya

     if(j==3){
              flg_agmal=1;
              //j=0;
          }
          if(i==3){
              flg_iraj=1;
              //i=0;
          }
          
Terakhir membuat kondisi dimana jika wakeup_status > 100 dan spirit_status < 0 maka akan di print

    if(WakeUp_Status >= 100){
            printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
        }
        if(Spirit_Status <= 0){
            printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
        }
        
Code Lengkapnya : [soal3](/No3/no3.c)

### No 4
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 
Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
Ketika Mengekstrak file .zip juga harus secara bersama-sama
Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
Wajib Menggunakan Multithreading
Boleh menggunakan system

Jawab :
Gunakan 2 thread untuk menyimpan 10 ps -aux pada file pertama dan kedua, kemudian 2 thread untuk menzip file pertama pada zip pertama dan zip kedua pada file ke 2 sekalian dengan menghapus kedua file tersebut setelah di zip. Kemudian 2 thread lagi untuk mengekstrak kembali kedua file zip pada masing masing folder tadi setelah menunggu 15 detik.

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

Code lengkapnya: [Soal 4](/No4/soalno4.c)

### No 5
Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta. 

a.Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.

b.Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.

c.Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).

d.Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.

e.Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 

f.Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
Pembeli (terintegrasi dengan game)
Dapat mengecek stok makanan yang ada di toko.
Jika stok ada, pembeli dapat membeli makanan.
Penjual (terpisah)
Bisa mengecek stok makanan yang ada di toko
Penjual dapat menambah stok makanan.

   Spesifikasi program:
   
a.Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)

b.Program terdiri dari 3 scene yaitu standby, battle, dan shop.

c.Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. Contoh :

Standby Mode

Health : [health status]

Hunger : [hunger status]

Hygiene : [hygiene status]

Food left : [your food stock]

Bath will be ready in [cooldown]s

Choices

Eat

Bath

Battle

Shop

Exit

Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :

Battle Mode

Monster’s Health : [health status]

Enemy’s Health : [enemy health status]

Choices

Attack

Run

Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. Contoh :

Shop Mode

Shop food stock : [shop food stock]

Your food stock : [your food stock]

Choices

Buy

Back

Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :

Shop

Food stock : [shop food stock]

Choices

Restock

Exit

Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))

Jawab :

Pertama buat input untuk nama setelah itu membuat 4 thread(makan,minum,regen,ready) dan 1 fungsi (pick()) 

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
	
Pada thread makan hunger status akan berkurang 5 setiap 10 detik
	
	void* makan(void *arg)
	{
	    while(1){
		hunger_status -= 5;
		sleep(10);
	    }
	}

Pada thread mandi hygiene status akan berkurang 10 setiap 30 detik

	void* mandi(void *arg)
	{
	    while(1){
		hygiene_status -= 10;
		sleep(30);
	    }
	}
	
Pada thread regen health status akan bertambah 5 setiap 10 detik

	void* regen(void *arg){
	    while(1){
		health_status += 5;
		sleep(10);
	    }
	}
	
Pada thread ready digunakan untuk meng countdown bath apakah sudah siap atau belum

	void* ready(void *arg){
	    while(1){
		if(siap > 0){
		    siap--;
		    sleep(1);
		}
	    }
	}

Sedangkan fungsi pick() digunakan untuk membuat option pilihan 

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
	
Terdapat fungsi fight didalam fungsi pick yang berguna untuk battle dengan musuh

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
	
Dan juga terdapat fungsi mygetch digunakan agar memilih option tanpa enter

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
	
Dan juga terdapat fungsi belanja untuk membeli stock makanan didalam fungsi pick juga

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

	    shmdt(stock);
	    shmctl(shmid, IPC_RMID, NULL);
	}

Sedangkangkan untuk membeli stock dibutuhkan restock dengn membuat program baru dimana bertujuan untuk menambah stock pada toko

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
		
Code lengkapnya : [Soal5](/No5/no5.c)

Code lengkapnya : [Soal5b](/No5/no5b.c)
