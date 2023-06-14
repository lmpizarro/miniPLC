#include "state.h"
#include "def.h"


void setupPins()
{
  pinMode(pin_d_i_1, INPUT);
  pinMode(pin_d_i_2, INPUT);
  pinMode(pin_d_i_3, INPUT);
  pinMode(pin_d_i_4, INPUT);
  pinMode(pin_d_o_1, OUTPUT);
  pinMode(pin_d_o_2, OUTPUT);
}

bool in_range(const char c)
{
  if (48 <= c <= 57 || 65 <= c <= 90 || 97 <= c <= 122 || c == 58 || c == 59)
    return true;
  return false;
}

class Commands
{
private:
private:
  Stream *m_stream;

public:
  String inputString = ""; // a String to hold incoming data
  bool begin_message = false;
  char inChar;
  // command codes
  String ID;
  String r_d_1 = ID;
  String r_a_1 = ID;

  Commands(Stream &, String);
  bool receive(void);
  void response(bool, State &);
};

Commands::Commands(Stream &port, String id) : m_stream(&port), ID(id)
{

  r_d_1 = ID + "d1";
  r_a_1 = ID + "a1";
}

bool Commands::receive(void)
{
  if (mySerial.available())
  {
    inChar = (char)mySerial.read();
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

void Commands::response(bool has_cmd, State &state)
{
  if (has_cmd)
  {

    if (inputString == r_d_1)
    {
      mySerial.println("!" + inputString + ":" + state.d_i_1);
    }
    if (inputString == r_a_1)
    {
      mySerial.println("!" + inputString + ":" + state.a_i_1);
    }
    inputString = "";
    begin_message = false;
    mySerial.flush();
  }
}

const long interval = 10;        // interval at which to fetch state
String ID = "1";
Commands cmds(mySerial, "1");
State state(interval);

void setup()
{
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  setupPins();
}

void loop()
{
  state.update();

  bool has_cmd = cmds.receive();
  cmds.response(has_cmd, state);
}
