/*Association des moteurs avec leur id codeur*/ 
#define MOTEUR_DROIT 1 
#define MOTEUR_GAUCHE 0 
 
/* init des 2 codeurs de chaque moteur 
(raz des valeurs codeurs, interruptions)*/ 
void codeursInit(); 

void moteur_1_Sb();
void moteur_2_Sb();

void PrintSens();

void toTest();
 
/*remet Ã  0 les valeurs des 2 codeurs*/ 
void codeursReset(); 
 
/*retourne la valeur des codeurs (768 ticks / tours ) 
codeurId doit etre 0 ou 1, sinon, renvoie 0*/ 
long getCodeur(unsigned int codeurId ); 
