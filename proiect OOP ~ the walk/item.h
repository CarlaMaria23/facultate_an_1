#ifndef item_H
#define item_H
#include <iostream>
#include "robot.h"
class robot;
class item{
    int x,y;
public:
    virtual void caracteristica(robot &R)=0; //fiecare item actioneaza diferit asupra robotului
    void getPozitie(int& ,int&);
    void setPozitie(int, int);
};
void item::getPozitie(int &i, int &j){
    i=x;
    j=y;
}
void item::setPozitie(int i, int j){
    x=i;
    y=j;
}
#endif // item.h

