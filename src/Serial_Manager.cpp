#include "Serial_Manager.h"

Serial_Manager::Serial_Manager(HardwareSerial* serial, char separator)
{
  serial_ = serial;
  separator_ = separator;
  buffer_.reserve(MAX_BUFFER_SIZE); 
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
    else if (c != '\r' && buffer_.length() < MAX_BUFFER_SIZE)
    {
      buffer_ += c;
    }
  }
  return "";
}

const Parsed_Args &Serial_Manager::get_args()
{
  parsed_data_.available = false;
  parsed_data_.arg_count = 0;

  String raw = raw_serial();
  if (raw.length() == 0) return parsed_data_;

  parsed_data_.available = true;
  int start_index = 0;
  int separator_index = raw.indexOf(separator_);

  while (separator_index != -1 && parsed_data_.arg_count < MAX_ARGS - 1) 
  {
    parsed_data_.args[parsed_data_.arg_count] = raw.substring(start_index, separator_index);
    parsed_data_.arg_count++;
    
    start_index = separator_index + 1;
    separator_index = raw.indexOf(separator_, start_index);
  }

  if (start_index < raw.length() && parsed_data_.arg_count < MAX_ARGS) 
  {
    parsed_data_.args[parsed_data_.arg_count] = raw.substring(start_index);
    parsed_data_.arg_count++;
  }

  return parsed_data_;
}