#ifndef __ASSERVISSEMENT_H__
#define __ASSERVISSEMENT_H__
#include "Arduino.h"

//valeur de saturation pour la commande.
//plus la valeur est haute, plus la vitesse du robot est elevee (attention patinage).
//mais la compensation en angle est faible (risque de ne pas aller droit)
#define SATURATION_COMMANDE 200

//consigne translation en demi-ticks:
//diametre roue = 65 mm.
//768 ticks/tour
//1 tick => avance de pi.D/768 = 0.26mm
//consigne en multiple de demi-ticks = 0.13 mm
#define NB_TICKS_PAR_M 7522

//est fonction des dimensions de la roue et de la longueur de l'entre-axe
//distance entre les roues.
//valeur relevee experimentalement sur plusieurs tours:
//si consigneTh = NB_TICKS_PAR_TOUR/4 => rotation de 90 degres sens horaire
#define NB_TICKS_PAR_TOUR 2548

//Valeur de la commande minimale, en dessous de laquelle
//on arrÃªte le robot (pour eviter le bruit...).
#define COMMANDE_MIN 25

//asservissement.en translation et rotation.
//doit etre appelee de maniere periodique (au moins 10 fois par constante de temps du robot...
//toutes les 5ms, c'est bien).
void asservissement();

//definit les consignes (objectif de translation/rotation).
// les consignes utilisent les constantes 'NB_TICK_PAR_M' et 'NB_TICKS_PAR_TOUR'
//exemple: 1/4 de tour vers dans le sens des aiguilles d'une montre:
// -> addConsigneRot(NB_TICKS_PAR_TOUR/4);
//exemple: 15cm devant:
// -> addConsigneTr(0.15*NB_TICKS_PAR_M);
//cette fonction peut etre appelee sous it.
void addConsigneTr(long consigneTr);
void addConsigneRot(long consigneRot);

//initialise l'asservissement.
//ceci n'initialise pas les codeurs, ni les pwm.
void asservisssementInit();

//retourne true si le robot est a l'arret.
bool isStopt();

//arrete le robot a sa position courante
//il ne s'agit pas d'arreter l'asservissement, mais de definir 
//la position actuelle comme consigne.
void stopRobot();

#endif
