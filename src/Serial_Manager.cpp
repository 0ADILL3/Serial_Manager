#include "Serial_Manager.h"

Serial_Manager::Serial_Manager(HardwareSerial* serial, char separator, const uint8_t reserve_sizes[], uint8_t reserve_len)
{
  serial_ = serial;
  separator_ = separator;
  buffer_.reserve(SERIAL_MANAGER_MAX_BUFFER_SIZE); 
  
  for (uint8_t i = 0; i < SERIAL_MANAGER_MAX_ARGS; i++)
  {
    uint8_t reserve_size = 15;
    if (reserve_sizes != nullptr && i < reserve_len) {reserve_size = reserve_sizes[i];}
    parsed_data_.args[i].reserve(reserve_size); 
  }
}

void Serial_Manager::begin(unsigned long baudrate)
{
  serial_->begin(baudrate);
}

String Serial_Manager::raw_serial()
{
  while (serial_->available())
  {
    char c = serial_->read();
    if (c == '\n')
    {
      String msg = buffer_;
      msg.trim();
      buffer_ = "";
      return msg;
    } 
    else if (c != '\r' && buffer_.length() < SERIAL_MANAGER_MAX_BUFFER_SIZE)
    {
      buffer_ += c;
    }
  }
  return "";
}

const Parsed_Args &Serial_Manager::get_args()
{
  String data = raw_serial();
  return get_args(data);
}

const Parsed_Args &Serial_Manager::get_args(const String &serial_data)
{
  parsed_data_.available = false;
  parsed_data_.arg_count = 0;
  
  if (serial_data.length() == 0) return parsed_data_;

  parsed_data_.available = true;
  int start_index = 0;
  int separator_index = serial_data.indexOf(separator_);

  while (separator_index != -1 && parsed_data_.arg_count < SERIAL_MANAGER_MAX_ARGS - 1) 
  {
    parsed_data_.args[parsed_data_.arg_count] = serial_data.substring(start_index, separator_index);
    parsed_data_.arg_count++;
    
    start_index = separator_index + 1;
    separator_index = serial_data.indexOf(separator_, start_index);
  }

  if (start_index < serial_data.length() && parsed_data_.arg_count < SERIAL_MANAGER_MAX_ARGS) 
  {
    parsed_data_.args[parsed_data_.arg_count] = serial_data.substring(start_index);
    parsed_data_.arg_count++;
  }

  return parsed_data_;
}