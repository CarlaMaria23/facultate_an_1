#ifndef viata_H
#define viata_H
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "item.h"
class viata:public item{
public:
    viata();
    void caracteristica(robot &R);
};
viata::viata(){
    int i,j;
    srand(time(NULL));
    i=rand()%20;
    j=rand()%20;
    setPozitie(i,j);
}
//doar robotu 1 si robotu 2 pot pierde viata, robotul 3 e indestructibil
void viata::caracteristica(robot &R){
    if(R.accesibilitateViata()){
        int i;
        i=R.getViata();
        i--;
        R.setViata(i);
    }
}
#endif // item.h




