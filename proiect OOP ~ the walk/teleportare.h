#ifndef teleportare_H
#define teleportare_H
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "item.h"
class teleportare:public item{
    int Xsosire,Ysosire;
public:
    teleportare();
    void getSosire();
    void caracteristica(robot &R);
};
teleportare::teleportare(){
    int i,j;
    srand(time(NULL));
    i=rand()%20;
    j=rand()%20;
    setPozitie(i,j);
    i=rand()%20;
    j=rand()%20;
    Xsosire=i;
    Ysosire=j;
}
//teleportarea e accesibila tuturor robotilor
void teleportare::caracteristica(robot &R){
    R.setPozitie(Xsosire,Ysosire);
    cout<<"teleportare pe "<<Xsosire<<" "<<Ysosire<<"\n";
}
#endif // item.h


