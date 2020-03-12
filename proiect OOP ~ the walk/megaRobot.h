#ifndef megaRobot_H
#define megaRobot_H
#include <iostream>
#include "robot.h"
using namespace std;
//robot 1
class megaRobot:public robot{
    int directie;
public:
    megaRobot();
    void actiune();
    bool accesibilitateViata();
    bool accesibilitateDiagonala();
};
megaRobot::megaRobot(){
    directie=1;
    cout<<"constructie megaRobot\n";
}
//functie ce decide cum se misca robotul pe harta- se apeleaza la fiecare runda a jocului
//robotul 1 merge pe liecare linie
void megaRobot::actiune(){
    int i=0,j=0;
    getPozitie(i,j);
    if((j==19 && directie==1 ) || (j==0 && directie==-1)){
        i++;
        directie=directie*(-1);
    }
    else
        j=j+directie;
    setPozitie(i,j);
}
//functii ce testeaza compatibiltatea cu itrmii
bool megaRobot::accesibilitateViata(){
    return 1;
}
bool megaRobot::accesibilitateDiagonala(){
    return 0;
}
#endif // MegaRobot.h


