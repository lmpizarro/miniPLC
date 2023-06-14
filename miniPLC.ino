#include "state.h"
#include "def.h"
#include "ascii_protocol.h"

void setupPins()
{
  pinMode(pin_d_i_1, INPUT);
  pinMode(pin_d_i_2, INPUT);
  pinMode(pin_d_i_3, INPUT);
  pinMode(pin_d_i_4, INPUT);
  pinMode(pin_d_o_1, OUTPUT);
  pinMode(pin_d_o_2, OUTPUT);
}


const long interval = 10;        // interval at which to fetch state
String ID = "1";
AsciiProtocol cmds(mySerial, "1");
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
