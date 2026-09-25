# Serial Manager

Pustaka C++ ringan untuk ekosistem Arduino yang menangani pembacaan komunikasi serial secara *non-blocking* dan memecah (parsing) pesan secara otomatis menjadi argumen terpisah. Dirancang dengan prinsip efisiensi RAM yang ketat untuk sistem tertanam (*embedded systems*).

## Fitur Utama
- **Non-blocking Read:** Tidak menggunakan fungsi `delay()` atau penahan komputasi apa pun. Program utama akan terus berjalan sembari mengumpulkan karakter serial di latar belakang.
- **Zero-Copy Parsing:** Menggunakan implementasi pengembalian *constant reference* (`const Parsed_Args&`) untuk mengirimkan data ke program `.ino` Anda tanpa menyalin atau menduplikasi objek di dalam RAM.
- **Pre-allocation Memory:** Buffer utama dicadangkan kapasitasnya (*reserved*) sejak awal untuk menghindari fragmentasi *heap*.
- **Otomasi Batasan Memori:** Melindungi mikrokontroler dari *crash* akibat *buffer overflow* secara proaktif dengan batasan kapasitas 128 karakter per pesan dan 10 maksimum pembagian argumen.

## Instalasi
1. Unduh repositori ini sebagai file `.zip`.
2. Buka Arduino IDE.
3. Buka menu **Sketch** -> **Include Library** -> **Add .ZIP Library...**
4. Pilih file `.zip` yang baru saja diunduh.

## Spesifikasi API Teknis
Pustaka ini membatasi kapasitas memori secara bawaan (bisa dimodifikasi di file `.h` jika diperlukan):
- `MAX_BUFFER_SIZE`: 128 karakter. Jika panjang pesan yang datang belum menemui `\n` melebihi 128 karakter, sisa pesan akan dipotong (dibuang) secara aman.
- `MAX_ARGS`: 10 potong argumen per satu kali siklus pemecahan data. 

## Contoh Penggunaan
Kirimkan pesan melalui Serial Monitor dengan format argumen yang dipisahkan koma dan diakhiri dengan tombol Enter (Newline `\n`). Contoh: `LED,ON,255`

```cpp
#include <Serial_Manager.h>

// Inisialisasi manajer pada port Serial utama dengan pemisah koma (',')
Serial_Manager manager(&Serial, ',');

void setup() {
  manager.begin(115200);
  Serial.println("Sistem siap. Kirim data dengan format: Arg1,Arg2,Arg3");
}

void loop() {
  // Ambil referensi data ke variabel lokal (harus menggunakan const dan &)
  const Parsed_Args& data = manager.get_args();

  // Cek apakah ada paket data utuh yang baru saja masuk
  if (data.available) {
    Serial.print("Pesan diterima! Jumlah argumen: ");
    Serial.println(data.arg_count);
    
    // Tampilkan setiap argumen yang dipecah
    for(int i = 0; i < data.arg_count; i++) {
      Serial.print("Argumen [");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(data.args[i]);
    }
    Serial.println("---");
  }

  // Baris di bawah ini membuktikan loop tetap berjalan (non-blocking)
  // Lakukan tugas mikrokontroler lainnya di sini...
}