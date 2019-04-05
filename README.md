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



### No 4


### No 5
