#pragma once

#include <Arduino.h>

#define MAX_BUFFER_SIZE 128
#define MAX_ARGS        10

struct Parsed_Args
{
  bool available;
  int arg_count;
  String args[MAX_ARGS];
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
     */
    Serial_Manager(HardwareSerial* serial = &Serial, char separator = ' ');
    
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
     * @brief Memecah pesan serial terbaru berdasarkan separator dan memperbarui data internal kelas.
     * @return Referensi statis (read-only) ke struktur data Parsed_Args.
     */
    const Parsed_Args &get_args(String serial_data = "NULL");
};