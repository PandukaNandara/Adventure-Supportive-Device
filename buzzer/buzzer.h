/*
 * buzzer.h
 *
 * Created: 2019-07-25 8:22:41 PM
 *  Author: pandu
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#ifndef BUZZER_PIN
#define BUZZER_PIN PIND5
#endif



void buzzer_init(void);
void buzzer_alarm(int ms);

#endif /* BUZZER_H_ */