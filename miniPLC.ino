
#define mySerial Serial
// https://robu.in/arduino-pin-configuration/
#define pin_d_i_1 DD2
#define pin_d_i_2 DD3
#define pin_d_i_3 DD4
#define pin_d_i_4 DD5
#define pin_d_o_1 DD7
#define pin_d_o_2 8
#define analogOut_01 DD6
#define analogOut_02 9
#define pin_a_i_1 A0
#define pin_a_i_2 A1

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

class State
{
public:
  bool d_i_1, d_i_2, d_i_3, d_i_4;
  bool d_o_1, d_o_2;
  unsigned int a_i_1, a_i_2;
  const unsigned int interval;
  unsigned int previousMillis;
  State(unsigned int intervl) : interval(intervl) {}
  void update(void);
};

void State::update(void)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    d_i_1 = digitalRead(pin_d_i_1);
    d_i_2 = digitalRead(pin_d_i_2);
    d_i_3 = digitalRead(pin_d_i_3);
    d_i_4 = digitalRead(pin_d_i_4);
    d_o_1 = digitalRead(pin_d_o_1);
    d_o_2 = digitalRead(pin_d_o_2);
    a_i_1 = analogRead(pin_a_i_1);
    a_i_2 = analogRead(pin_a_i_2);
  }
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
