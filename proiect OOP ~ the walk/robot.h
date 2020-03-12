#ifndef robot_H
#define robot_H
#include <iostream>
using namespace std;
class robot{
    int x,y;   //pozitia robotului
    int viata;
public:
    robot();
    void setPozitie(int, int);
    void getPozitie(int&, int&);
    int getViata();
    void setViata(int);
    virtual bool accesibilitateViata()=0;
    virtual bool accesibilitateDiagonala()=0;
    virtual void actiune()=0;
};
robot::robot(){
    x=0;
    y=0;
    viata=5;
}
int robot::getViata(){
    return viata;
}
void robot::setViata(int i){
    viata=i;
}
void robot::setPozitie(int i, int j){
    x=i;
    y=j;
}
void robot::getPozitie(int &i,int &j){
    i=x;
    j=y;
}
#endif // robot.h

