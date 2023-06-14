#include "state.h"
#include "def.h"
#include "ascii_protocol.h"
#include "plc_func.h"

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
State state(interval);
String ID = "1";
AsciiProtocol cmds(mySerial, "1");

void setup()
{
  mySerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  setupPins();
}

void loop()
{
  state.update();

  cmds.response(state);

}
