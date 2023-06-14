#ifndef STATE_H

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


#define STATE_H
#endif