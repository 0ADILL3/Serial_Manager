#include "Serial_Manager.h"

// 1. Tentukan alokasi ukuran memori (reserve) untuk masing-masing argumen.
// Sesuaikan jumlah elemen dan ukurannya dengan kebutuhan memori Anda (maks 255 karakter per elemen).
const uint8_t custom_reserves[] = {20, 10, 15}; 

// 2. Hitung panjang array secara otomatis untuk mencegah pembacaan memori di luar batas
const uint8_t reserve_len = sizeof(custom_reserves) / sizeof(custom_reserves[0]);

// 3. Inisialisasi manajer pada port Serial utama, pemisah koma (','), dan pengaturan memori khusus
Serial_Manager manager(&Serial, ',', custom_reserves, reserve_len);

void setup() {
  manager.begin(115200);
  Serial.println("Sistem siap. Kirim data dengan format: Arg1,Arg2,Arg3");
}

void loop() {
  // Ambil referensi data ke variabel lokal (harus menggunakan const dan & untuk efisiensi RAM)
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