#include "codeurs.h"
#include "moteurs.h"
#include "asservissement.h"
#include "Arduino.h"
#include "samd.h"

//indique l'etat du robot: A L'arret.
//variable non protegee. acces atomique.
static volatile bool aLarret;
//consigne en translation (en demi-ticks). acces a proteger sous it
static volatile long __consigneTr;
//consigne en rotation. acces a proteger sous it
static volatile long __consigneRot;
//ajout des commandes de translation et rotation
static volatile long commandeTr;
static volatile long commandeRot;

static float Ktr = 0.15;    //gain correcteur en translation
static float Krot = 0.5;    //gain correcteur en rotation

void asservisssementInit()
{
  aLarret = true;
  __consigneTr = 0;
  __consigneRot = 0;

  /*Initialisation de l'asservissement : interruption toutes les 5ms*/ 
  /*Compteur 16 bits TC4, frÃ©quence 48MHz, pre-diviseur 4*/ 
  PM->APBCMASK.reg |= PM_APBCMASK_TC4;          //power manager 
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN|GCLK_CLKCTRL_GEN(0)| GCLK_CLKCTRL_ID_TC4_TC5;  //horloge 48MHz                    
  TC4->COUNT16.CTRLA.reg = 0b010 << 8;  /*periode 5ms : diviseur /4 => periode de 5,4ms en mode 16 bits*/ 
  TC4-> COUNT16.CTRLBSET.reg = 1;       /*mode decomptage*/ 
  TC4-> COUNT16.COUNT.reg = 60681;            // A COMPLETER
  TC4->COUNT16.INTENSET.reg = 1;        /*validation locale de l'interruption*/   
  NVIC_EnableIRQ(TC4_IRQn); 
  TC4->COUNT16.CTRLA.reg |= 2;          /*demarrage du timer*/ 
}

/*Interruption toutes les 5ms : asservissement des roues*/ 
void TC4_Handler () 
{ 
  TC4-> COUNT16.COUNT.reg = 60681;            // A COMPLETER
  asservissement();
  TC4-> COUNT16.INTFLAG.reg = 0x1;      // A COMPLETER
} 

void asservissement()
{
  const long codeurD = getCodeur(DROITE);
  const long codeurG = getCodeur(GAUCHE);

  //les consignes peuvent etre modifiees sous it.
  //l'accÃ¨s est atomique (sur 32 bits), on garde juste une copie.
  const long mConsigneTr = __consigneTr;
  //const long mConsigneRot = __consigneRot;
  
  //ecart en translation
  commandeTr=  __consigneTr * Ktr;

  //ecart en angle
  //commandeRot=  __consigneRot * Krot;
  
  //commande moteur.
  int commandeD = commandeTr;  //-commandeRot
  int commandeG = commandeTr; //+commandeRot

  avancer_bis(commandeG,commandeD);
  
  //arret a destination (evite les vibrations).
  if(abs(commandeD) < COMMANDE_MIN) commandeD = 0;
  if(abs(commandeG) < COMMANDE_MIN) commandeG = 0;
  aLarret = (commandeD == 0 && commandeG == 0);
  


}

bool isStop()
{
  return aLarret;
}

void addConsigneTr(long consigneTr)
{
  __consigneTr += consigneTr;
}

/*void addConsigneRot(long consigneRot)
{
  __consigneRot += consigneRot;
}*/

void stopRobot()
{  
  const long codeurG = getCodeur(GAUCHE);
  const long codeurD = getCodeur(DROITE);
  __consigneTr = codeurG+codeurD;
  __consigneRot = codeurG-codeurD;
}
