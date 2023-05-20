#include "sam.h"    //definition des registres
#include "moteurs.h"
#include "pinMux.h"

void moteursInit()
{
  //moteur 1 (sur TC3 => moteur gauche)
  PM->APBCMASK.reg |= PM_APBCMASK_TC3;  //power manager
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN  |
                      GCLK_CLKCTRL_GEN(0) |
                      GCLK_CLKCTRL_ID_TCC2_TC3; //horloge 48MHz
                      
  //voir 'reference manual' du SAMD21, p.14 (chapitre I/O Multiplexing)
  pinMux(0,15,CONFIG_E);  //PA15 => Channel 1
  pinMux(0,18,CONFIG_E);  //PA18 => Channel 0
  //il faut Freq PWM > 20KHz => pÃ©riode < 50us
  //si diviseur/8 => 42.6us max (p.101) => c'est Ok.
  TC3->COUNT8.CTRLA.reg = 
    0x3 << 8 | //prÃ©-diviseur 8
    0x2 << 5 | //mode PWM
    0x1 << 2;  //mode 8 bits
  TC3->COUNT8.PER.reg = 255;  
  // Initialisation des registres de comparaison
  TC3->COUNT8.CC[0].reg = 0;  //PA18 => PWM_R1
  TC3->COUNT8.CC[1].reg = 0;  //PA15 => PWM_F1 => 0 (Reverse) 1 (Forward)
  // TC3->COUNT8.CTRLC.reg = 0x1;
  // REG_TC3_CTRLC = 0x1; inversion de la sortie WO[1], p636 datasheet
  //enable timer
  TC3->COUNT8.CTRLA.reg |= 2;

  //moteur 2 (sur TCC0 => moteur droit)
  PM->APBCMASK.reg |= PM_APBCMASK_TCC0;  //power manager
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN  |
                      GCLK_CLKCTRL_GEN(0) |
                      GCLK_CLKCTRL_ID_TCC0_TCC1; //horloge 48MHz
  //voir 'reference manual' du SAMD21, p.14 (chapitre I/O Multiplexing)
  pinMux(0,19,CONFIG_F);  //PA19 => Channel 3 => comparaison CC[3]
  pinMux(0,20,CONFIG_F);  //PA20 => Channel 6 => comparaison CC[2] (6%4)
  //il faut Freq PWM > 20KHz => pÃ©riode < 50us
  //si diviseur/8 => 42.6us (p.101) => c'est Ok.
  TCC0->CTRLA.reg = 0x3 << 8 ;  //prÃ©-diviseur 8
  TCC0->WAVE.reg = 2;           //mode PWM
  TCC0->PER.reg = 255;          //comptage sur 8 bits (mÃªme calcul que pour TC3)
  TCC0->CC[2].reg = 0;          //PWM sur PA20 Ã  0%
  TCC0->CC[3].reg = 0;          //PWM sur PA19 Ã  0%
  //TCC0->DRVCTRL.reg = 1 << 17 ;
  //enable timer
  TCC0->CTRLA.reg |= 2;
}

/**Fonction qui commande le moteur droit
 * \param speed vitesse entre -255 et 255.
 * 0 pour l'arrÃªt. le signe indique le sens de rotation.
 */
void setMotor(unsigned int id, int speedM)
{
  if(id > 1) return;
  
  //restriction dans l'intervalle [-255,255]
  int cmd;
  if(speedM > 255) cmd = 255;
  else if(speedM < -255) cmd = -255;
  else cmd = speedM;

  //consigne.
  int consigneF, consigneR;
  if(speedM > 0) { //avance
    consigneF = cmd;
    consigneR = 0;
  } else {
    consigneF = 0;
    consigneR = -cmd;
  }

  //commandes PWM sur les moteurs.
  if(id == GAUCHE) {
    TC3->COUNT8.CC[1].reg = consigneF ;
    TC3->COUNT8.CC[0].reg = consigneR ;
  } else {
    TCC0->CC[2].reg = consigneF ;
    TCC0->CC[3].reg = consigneR ;    
  }
/*
//***************************************************************** AJOUT 14/9
  //commandes PWM sur les moteurs.            
  if(id == DROITE) {
    TC3->COUNT8.CC[1].reg = consigneR ;
    TC3->COUNT8.CC[0].reg = consigneF ;
  } else {
    TCC0->CC[2].reg = consigneR ;
    TCC0->CC[3].reg = consigneF ;    
  } 
  */
}

void avancer(int speedM) {
  setMotor(GAUCHE,speedM);
  setMotor(DROITE,speedM);
}

void avancer_bis(int gauche, int droite) {
  setMotor(GAUCHE,gauche);
  setMotor(DROITE,droite);
}
