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
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "timer.h"
#include "main.h"
#include "uart.h"

unsigned int valueTelemetreEGauche = 0;
unsigned int valueTelemetreGauche = 0;
unsigned int valueTelemetreMilieu = 0;
unsigned int valueTelemetreDroite = 0;
unsigned int valueTelemetreEDroite = 0;

int main(void) {

    //Initialisation oscillateur

    InitOscillator();
    InitIO();
    InitTimer1();
    //InitTimer23();
    InitTimer4();

    InitPWM();

    InitUART();
    
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
        SendMessageDirect((unsigned char*) "BonDour", 7);
//        __delay32(40000000);
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreEgauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreEdroite = 34 / volts - 5;
            
            if (robotState.distanceTelemetreEdroite < 20) 
                LED_VERTE_2 = 1;
            else 
                LED_VERTE_2 = 0;
            
            if (robotState.distanceTelemetreDroit < 20)
                LED_ROUGE_2 = 1;
            else 
                LED_ROUGE_2 = 0;
            
            if (robotState.distanceTelemetreCentre < 20) 
                LED_ORANGE_2 = 1;
            else 
                LED_ORANGE_2 = 0;
            
            if (robotState.distanceTelemetreGauche < 20) 
                LED_BLEUE_2 = 1;
            else 
                LED_BLEUE_2 = 0;
            
            if (robotState.distanceTelemetreEgauche < 20) 
                LED_BLANCHE_2 = 1;
            else 
                LED_BLANCHE_2 = 0;
            //20cm milieu --> 1643
            //30cm milieu --> 1178
            //40cm milieu --> 902
        } //fin main
    }
}


unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_RECULE:
            PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
            
            
        case STATE_TOURNE_EXTREME_GAUCHE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_EXTREME_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_EXTREME_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
/*            
        case STATE_TOURNE_SUR_PLACE_EXTREME_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_EXTREME_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;  
*/            
        case STATE_TOURNE_EXTREME_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_EXTREME_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_EXTREME_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
/*            
        case STATE_TOURNE_SUR_PLACE_EXTREME_GAUCHE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_EXTREME_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
*/        
            
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //éDtermination de la position des obstacles en fonction des ééètlmtres
    if      (robotState.distanceTelemetreEdroite < 30 &&
               robotState.distanceTelemetreDroit > 40 &&
              robotState.distanceTelemetreCentre > 30 &&
              robotState.distanceTelemetreGauche > 40 &&
             robotState.distanceTelemetreEgauche > 40) //Obstacle àdroite
        positionObstacle = OBSTACLE_A_EXTREME_DROITE;
    
    else if (robotState.distanceTelemetreEdroite > 40  &&
               robotState.distanceTelemetreDroit > 40 &&
              robotState.distanceTelemetreCentre > 30 &&
              robotState.distanceTelemetreGauche > 30 &&
             robotState.distanceTelemetreEgauche < 30) //Obstacle àgauche
        positionObstacle = OBSTACLE_A_EXTREME_GAUCHE;
 

    else if(robotState.distanceTelemetreEdroite < 35  &&
              robotState.distanceTelemetreDroit < 35 &&
             robotState.distanceTelemetreCentre > 30 &&
             robotState.distanceTelemetreGauche > 40 &&
            robotState.distanceTelemetreEgauche > 40) //Obstacle àdroite
        positionObstacle = OBSTACLE_A_DROITE;
    else if(robotState.distanceTelemetreEdroite > 35  &&
              robotState.distanceTelemetreDroit < 35 &&
             robotState.distanceTelemetreCentre > 30 &&
             robotState.distanceTelemetreGauche > 40 &&
            robotState.distanceTelemetreEgauche > 40) //Obstacle àdroite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreEdroite > 40  &&
               robotState.distanceTelemetreDroit > 40 &&
              robotState.distanceTelemetreCentre > 30 &&
              robotState.distanceTelemetreGauche < 35 &&
             robotState.distanceTelemetreEgauche < 35) //Obstacle àgauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreEdroite > 40  &&
               robotState.distanceTelemetreDroit > 40 &&
              robotState.distanceTelemetreCentre > 30 &&
              robotState.distanceTelemetreGauche < 35 &&
             robotState.distanceTelemetreEgauche > 35) //Obstacle àgauche
        positionObstacle = OBSTACLE_A_GAUCHE;
   
    else if (robotState.distanceTelemetreEdroite < 30 &&
            robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre < 30 &&
            robotState.distanceTelemetreGauche > 40 &&
            robotState.distanceTelemetreEgauche > 40)  //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE_DROITE;
    else if (robotState.distanceTelemetreEdroite > 40 &&
            robotState.distanceTelemetreDroit > 40 &&
            robotState.distanceTelemetreCentre < 30 &&
            robotState.distanceTelemetreGauche < 30 &&
            robotState.distanceTelemetreEgauche < 30)
        positionObstacle = OBSTACLE_EN_FACE_GAUCHE; 
    
    else if (robotState.distanceTelemetreEdroite > 40 &&
            robotState.distanceTelemetreDroit > 40 &&
            robotState.distanceTelemetreCentre < 30 &&
            robotState.distanceTelemetreGauche > 40 &&
            robotState.distanceTelemetreEgauche > 40)
        positionObstacle = OBSTACLE_EN_FACE_GAUCHE;
    
    else if (robotState.distanceTelemetreCentre < 25)
        positionObstacle = OBSTACLE_EN_FACE_GAUCHE;
    
    else if (robotState.distanceTelemetreEdroite > 40  &&
               robotState.distanceTelemetreDroit > 40 &&
              robotState.distanceTelemetreCentre > 25 &&
              robotState.distanceTelemetreGauche > 40 &&
             robotState.distanceTelemetreEgauche > 40) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;
    //éDtermination de lé?tat àvenir du robot
    
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE_GAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE_DROITE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_EXTREME_DROITE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_EXTREME_GAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
    
    //Si l?on n?est pas dans la transition de lé?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}
