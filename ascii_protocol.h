#ifndef ASCII_PROTOCOL_H
#define ASCII_PROTOCOL_H
#include "Arduino.h"
#include "state.h"

class AsciiProtocol
{
private:
private:
  Stream &m_stream;

public:
  String inputString = ""; // a String to hold incoming data
  bool begin_message = false;
  char inChar;
  // command codes
  String ID;
  String r_d_1 = ID;
  String r_a_1 = ID;

  AsciiProtocol(Stream &, String);
  bool receive(void);
  void response(bool, State &);
};

#endif
