#pragma once

#include <Arduino.h>

#ifndef SERIAL_MANAGER_MAX_BUFFER_SIZE
  #define SERIAL_MANAGER_MAX_BUFFER_SIZE 128
#endif
#ifndef SERIAL_MANAGER_MAX_ARGS
  #define SERIAL_MANAGER_MAX_ARGS        5
#endif

/**
 * @struct Parsed_Args
 * @brief Wadah struktur data hasil pemecahan pesan serial.
 * @param available Status ketersediaan pesan baru yang berhasil diproses.
 * @param arg_count Jumlah total argumen yang berhasil diekstrak dari pesan.
 * @param args[SERIAL_MANAGER_MAX_ARGS] Array penampung argumen teks berukuran statis.
 */
struct Parsed_Args
{
  /** @brief Status ketersediaan pesan baru yang berhasil diproses. */
  bool available;
  /** @brief Jumlah total argumen yang berhasil diekstrak dari pesan. */
  int arg_count;
  /** @brief Array penampung argumen teks berukuran statis. */
  String args[SERIAL_MANAGER_MAX_ARGS];
};

/**
 * @class Serial_Manager
 * @brief Manajer komunikasi serial non-blocking dengan parser memori teroptimasi.
 * 
 * Kelas ini membaca data serial ke dalam buffer secara berkelanjutan 
 * tanpa menghentikan loop utama, memotongnya berdasarkan karakter pemisah (separator), 
 * dan menyediakan akses referensi read-only ke data yang telah dipecah guna 
 * mencegah duplikasi memori RAM.
 */
class Serial_Manager
{
  private:
    HardwareSerial* serial_;
    String buffer_;
    char separator_;
    Parsed_Args parsed_data_;

  public:
    /**
     * @brief Konstruktor inisialisasi manajer serial.
     * @param serial Pointer ke antarmuka HardwareSerial (default: &Serial).
     * @param separator Karakter pemisah antar argumen pada pesan masuk (default: ' ').
     * @param reserve_sizes Array berisi ukuran reserve (maks 255) untuk masing-masing argumen (default: nullptr).
     * @param reserve_len Jumlah elemen di dalam array reserve_sizes (default: 0).
     */
    Serial_Manager(HardwareSerial* serial = &Serial, char separator = ' ', const uint8_t reserve_sizes[] = nullptr, uint8_t reserve_len = 0);
    
    /**
     * @brief Memulai jalur komunikasi serial.
     * @param baudrate Kecepatan komunikasi data (default: 115200 baud).
     */
    void begin(unsigned long baudrate = 115200);
    
    /**
     * @brief Menarik karakter dari buffer antarmuka serial secara non-blocking.
     * @return String pesan utuh jika karakter newline ('\n') terdeteksi, atau String kosong jika belum selesai.
     */
    String raw_serial();
    
    /**
     * @brief Memecah pesan serial dengan menarik data secara otomatis dari antarmuka serial.
     * @return Referensi statis (read-only) ke struktur data Parsed_Args.
     */
    const Parsed_Args& get_args();

    /**
     * @brief Memecah pesan dari variabel string eksternal (Pass-by-const-reference untuk hemat RAM).
     * @param serial_data Referensi konstan memori dari string yang akan di-parsing.
     * @return Referensi statis (read-only) ke struktur data Parsed_Args.
     */
    const Parsed_Args &get_args(const String& serial_data);
};