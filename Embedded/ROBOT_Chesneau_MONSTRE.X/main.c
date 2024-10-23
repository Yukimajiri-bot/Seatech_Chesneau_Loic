/* 
 * File:   main.c
 * Author: Table2
 *
 * Created on 30 septembre 2024, 09:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"

unsigned int valueTelemetreGauche = 0;
unsigned int valueTelemetreMilieu = 0;
unsigned int valueTelemetreDroite = 0;

int main(void) {

    //Initialisation oscillateur

    InitOscillator();
    InitIO();
    InitTimer1();
    InitTimer23();
    InitTimer4();
    
    InitPWM();
    PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);

    InitADC1();
    
    // Configuration des input et output (IO)
    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;
    
    LED_BLANCHE_2 = 1;
    LED_BLEUE_2 = 1;
    LED_ORANGE_2 = 1;
    LED_ROUGE_2 = 1;
    LED_VERTE_2 = 1;

    // Boucle Principale

    while (1) {
        LED_BLANCHE_2 = !LED_BLANCHE_2;
        //LED_BLEUE_2 = !LED_BLEUE_2;

        if (ADCIsConversionFinished() == 1)
        {
             ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;

                    
            if (robotState.distanceTelemetreDroit < 1178)
            {
                LED_ROUGE_2 = !LED_ROUGE_2;
            }
            if (robotState.distanceTelemetreCentre < 1178)
            {
                LED_ORANGE_2 = !LED_ORANGE_2;
            }
            if (robotState.distanceTelemetreGauche < 1178)
            {
                LED_BLEUE_2 = !LED_BLEUE_2;
            }
            //20cm milieu --> 1643
            //30cm milieu --> 1178
            //40cm milieu --> 902
        }        
    } //fin main
}
