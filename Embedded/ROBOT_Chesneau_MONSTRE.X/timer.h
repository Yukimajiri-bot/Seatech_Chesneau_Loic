#ifndef TIMER_H
#define TIMER_H
void InitTimer23(void);
void InitTimer1(void);
void InitTimer4(void);
extern unsigned long timestamp;
void SetFreqTimer1(float freq);
void SetFreqTimer4(float freq);
#endif /* TIMER_H */