#include "ascii_protocol.h"
#include "def.h"
#include "state.h"

bool in_range(const char c)
{
  if (48 <= c <= 57 || 65 <= c <= 90 || 97 <= c <= 122 || c == 58 || c == 59)
    return true;
  return false;
}



AsciiProtocol::AsciiProtocol(Stream &port, String id) : m_stream(port), ID(id)
{

  r_d_1 = ID + "d1";
  r_a_1 = ID + "a1";
}

bool AsciiProtocol::receive(void)
{
  if (m_stream.available())
  {
    inChar = (char)m_stream.read();
    if (!in_range(inChar))
    {
      inputString = "";
      begin_message = false;
      return false;
    }
    if (inChar == 58)
    {
      begin_message = true;
    }
    else
    {
      if (inChar != 59 && begin_message)
      {
        inputString += inChar;
      }
      else if (inChar == 59 && begin_message)
      {
        return true;
      }
    }
  }

  return false;
}

void AsciiProtocol::response(bool has_cmd, State &state)
{
  if (has_cmd)
  {

    if (inputString == r_d_1)
    {
      m_stream.println("!" + inputString + ":" + state.d_i_1);
    }
    if (inputString == r_a_1)
    {
      m_stream.println("!" + inputString + ":" + state.a_i_1);
    }
    inputString = "";
    begin_message = false;
    m_stream.flush();
  }
}
