#include "moteurs.h"
#include "codeurs.h"
#include "arduino.h"
#include "asserPerso.h"
#define VITESSE 1
#define POSITION 2

void AsserpersoInit(int tolerance, int vitesse, int distance, int type){
 
  int cptD = 0;
  int cptG = 0;
  volatile static int temp;
  bool valid = true;
  

  avancer_bis(vitesse,vitesse);
  delay(150);
    
  do{
      cptD = getCodeur(1);
      cptG = getCodeur(0);
      int temp = cptD - cptG;
      Serial.print(temp);
      if (temp < 0)     { avancer_bis (vitesse/2, vitesse); }
      else if (temp > 0){ avancer_bis (vitesse, vitesse/2); }
      Serial.println (vitesse);
      delay(50);
  }while(valid == true);
}
