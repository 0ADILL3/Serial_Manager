#include <Serial_Manager.h>

Serial_Manager serial(&Serial, ' ');

void setup() {
  serial.begin(115200);
}

void loop() {
  // Ambil referensi datanya ke variabel lokal hanya-baca
  const Parsed_Args &incomingData = serial.get_args();

  if (incomingData.available) {
    Serial.print("Jumlah argumen: ");
    Serial.println(incomingData.arg_count);
    
    for(int i = 0; i < incomingData.arg_count; i++) {
      Serial.println(incomingData.args[i]);
    }
  }
}