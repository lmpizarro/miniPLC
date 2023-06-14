#include "state.h"
#include "def.h"
#include "Arduino.h"

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

