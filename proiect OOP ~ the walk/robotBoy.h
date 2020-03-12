#ifndef robotBoy_H
#define robotBoy_H
#include <iostream>
#include "robot.h"
using namespace std;
//robot 3
class robotBoy:public robot{
public:
    robotBoy();
    void actiune();
    bool accesibilitateViata();
    bool accesibilitateDiagonala();
};
robotBoy::robotBoy(){
    cout<<"constructie robotBoy\n";
}
//functie ce decide cum se misca robotul pe harta- se apeleaza la fiecare runda a jocului
//robotul 3 merge pe diagonala principala
void robotBoy::actiune(){
    int i=0,j=0;
    getPozitie(i,j);
    i++;
    j++;
    setPozitie(i,j);
}
//functii ce testeaza compatibiltatea cu itrmii
bool robotBoy::accesibilitateViata(){
    return 0;
}
bool robotBoy::accesibilitateDiagonala(){
    return 1;
}
#endif // robotBoy.h



