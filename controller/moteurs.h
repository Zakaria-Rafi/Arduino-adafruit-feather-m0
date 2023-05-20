#ifndef __MOTEURS_H__
#define __MOTEURS_H__

#ifndef DROITE
  #define DROITE 1
  #define GAUCHE 0
#endif

/** initialisation des commandes pour les 2 moteurs */
void moteursInit();

/**Fonction qui commande le moteur droite ou gauche
 * \param id DROITE ou GAUCHE
 * \param speed vitesse entre -200 (recule a fond) et 200 (avance a fond).
 * 0 pour l'arret
 */
void setMotor(unsigned int id, int speedM);

void avancer(int speedM);

void avancer_bis(int gauche, int droite);

#endif
