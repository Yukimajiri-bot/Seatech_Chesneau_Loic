/* 
 * File:   PWM.h
 * Author: Table2
 *
 * Created on 30 septembre 2024, 12:03
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_GAUCHE 1
#define MOTEUR_DROIT 1

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, char Type_Moteur);
void PWMSetSpeedConsigne(float vitesseEnPourcents, char Type_Moteur);
void PWMUpdateSpeed();

#endif	/* PWM_H */

