#include "codeurs.h"
#include "arduino.h"

/* ROUE 2 = ROUE DROITE */

const int capteur_A1 = 9;
const int capteur_B1 = 11;

const int capteur_A2 = A2;
const int capteur_B2 = A3;

bool Sens_moteur_1 = 0;
bool Sens_moteur_2 = 0;

static volatile long int cpt_moteur_1_Sb =  0; //roue  cÃ´tÃ© 0 switch on/off -> 1 tour = 780 tick
static volatile long int cpt_moteur_2_Sb =  0; //roue  cÃ´tÃ© 0 switch on/off -> 1 tour = 780 tick

void moteur_1_Sb(){ // 1 => recul 
  if (digitalRead(capteur_A1)){ Sens_moteur_1 = 1; cpt_moteur_1_Sb --; 
  }else        { Sens_moteur_1 = 0; cpt_moteur_1_Sb++;} 
}

void moteur_2_Sb(){  // 1 => recul
  if (!digitalRead(capteur_A2)){ Sens_moteur_2 = 1; cpt_moteur_2_Sb--;
  }else        { Sens_moteur_2 = 0; cpt_moteur_2_Sb++;} 
}

long getCodeur(unsigned int codeurId ) {
  if(codeurId == 1) { return cpt_moteur_1_Sb; }
  else              { return cpt_moteur_2_Sb; }
}

void codeursInit() {
  
  Serial.begin(9600);
  pinMode(capteur_A1, INPUT_PULLUP);
  pinMode(capteur_A2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(capteur_B1),  moteur_1_Sb, RISING); 
  attachInterrupt(digitalPinToInterrupt(capteur_B2),  moteur_2_Sb, FALLING);
}

/*void toTest(){

    Serial.print("Moteur 1 Sa :");
    Serial.print(cpt_moteur_1_Sa);
    Serial.print("  --- Moteur 1 Sb :");
    Serial.println(cpt_moteur_1_Sb);
    delay(200);
    
}*/

void PrintSens(){

    Serial.print("Sens moteur 1 :");
    Serial.print(Sens_moteur_1);
    Serial.print("   --- Position du robot :");
    Serial.print(cpt_moteur_1_Sb);
    Serial.print("    Sens moteur 2 :");
    Serial.print(Sens_moteur_2);
    Serial.print("   --- Position du robot :");
    Serial.println(cpt_moteur_2_Sb);
}
