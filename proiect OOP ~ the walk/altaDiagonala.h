#ifndef altaDiagonala_H
#define altaDiagonala_H
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "item.h"
class altaDiagonala:public item{
public:
    altaDiagonala();
    void caracteristica(robot &R);
};
altaDiagonala::altaDiagonala(){
    int i,j;
    srand(time(NULL));
    i=rand()%20;
    j=rand()%20;
    setPozitie(i,j);
}
//item accesibil pentru robot 3
void altaDiagonala::caracteristica(robot &R){
    if(R.accesibilitateDiagonala()){
        int i,j;
        R.getPozitie(i,j);
        i++;
        R.setPozitie(i,j);
    }
}
#endif // item.h



