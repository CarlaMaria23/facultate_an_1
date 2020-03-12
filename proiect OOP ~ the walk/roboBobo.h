#ifndef roboBobo_H
#define roboBobo_H
#include <iostream>
#include "robot.h"
using namespace std;
//robot 2
class roboBobo:public robot{
    int directie;
public:
    roboBobo();
    void actiune();
    bool accesibilitateViata();
    bool accesibilitateDiagonala();
};
roboBobo::roboBobo(){
    directie=1;
    cout<<"constructie roboBobo\n";
}
//functie ce decide cum se misca robotul pe harta- se apeleaza la fiecare runda a jocului
//robotul 2 merge pe liecare coloana
void roboBobo::actiune(){
    int i=0,j=0;
    getPozitie(i,j);
    if((i==19 && directie==1 ) || (i==0 && directie==-1)){
        j++;
        directie=directie*(-1);
    }
    else
        i=i+directie;
    setPozitie(i,j);
}
//functii ce testeaza compatibiltatea cu itrmii
bool roboBobo::accesibilitateViata(){
    return 1;
}
bool roboBobo::accesibilitateDiagonala(){
    return 0;
}
#endif // roboBobo.h



