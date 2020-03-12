#ifndef harta_H
#define harta_H
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "robot.h"
#include "item.h"
#include "megaRobot.h"
#include "roboBobo.h"
#include "robotBoy.h"
#include "teleportare.h"
#include "altaDiagonala.h"
#include "viata.h"
using namespace std;
//clasa de gestionare a jocului
class harta{
    char **mapa;
    robot *R;
    item *C[10];
    int Xstop,Ystop;
public:
    harta();
    void game();
    void afisare();
    bool actualizare();
    bool castigator(int ,int);
    void plasare(int, int,const char);
    void verificareItemi();
    ~harta();
};
//costructorul clasei: aloca memorie pentru matrice, plaseaza punctul de sosire, alege cu ce robot se pleaca la joc si plaseaza itemii
harta::harta(){
    mapa=new char*[20];
    for(int i=0;i<20;i++){
        mapa[i]=new char[20];
        for(int j=0;j<20;j++)
            mapa[i][j]='.';
    }
    mapa[0][0]='R';
    srand(time(NULL));
    Xstop=rand()%20;
    Ystop=rand()%20;
    mapa[Xstop][Ystop]='S';
    cout<<"robot name: ";
    char *nume;
    nume=new char[20];
    cin>>nume;
    if(strcmp(nume,"megaRobot")==0)
        R=new megaRobot;
    else
        if(strcmp(nume,"roboBobo")==0)
            R=new roboBobo;
        else
            if(strcmp(nume,"robotBoy")==0)
                R=new robotBoy;
            else{
                cout<<"nu exista un robot cu numele acesta\n";
                cout<<"incercati: megaRobot, roboBobo sau robotBoy\n";
                exit(0);
            }
    int a,i,j;
    cout<<"loading...";
    for(int k=0;k<10;k++){
        a=rand()%3;
        if(a==0){
            C[k]=new teleportare;
            C[k]->getPozitie(i,j);
            if(mapa[i][j]!='.'){
                delete C[k];
                k--;
            }
            else mapa[i][j]='T';
        }
        if(a==1){
            C[k]=new altaDiagonala;
            C[k]->getPozitie(i,j);
            if(mapa[i][j]!='.'){
                delete C[k];
                k--;
            }
            else mapa[i][j]='D';
        }
        if(a==2){
            C[k]=new viata;
            C[k]->getPozitie(i,j);
            if(mapa[i][j]!='.'){
                delete C[k];
                k--;
            }
            else mapa[i][j]='V';
        }
    }
}
//vunctia verificareItem testeaza pentru fiecare item din joc daca robotul a ajuns sa ia itemul respectiv
void harta::verificareItemi(){
    int i,j;
    R->getPozitie(i,j);
    int a,b;
    for(int k=0;k<10;k++)
        if(C[k]!=NULL){
            C[k]->getPozitie(a,b);
            if(a==i && b==j){
                mapa[a][b]='.';
                C[k]->caracteristica(*R);
                delete C[k];
                C[k]=NULL;
            }
    }
}
//functia game este functia care gestioneaza jocul
void harta::game(){
    while(actualizare()){
        verificareItemi();
        system("CLS");
        afisare();
        cout<<endl;
        R->actiune();
        cout<<"life: "<<R->getViata()<<"\n";
    }
}
// functia de afisare a matricei
void harta::afisare(){
    for(int i=0;i<20;i++){
        for(int j=0;j<20;j++)
            cout<<mapa[i][j]<<" ";
        cout<<endl;
    }
}
//functie ce reconstruieste matricea cu noile date
bool harta::actualizare(){
    for(int i=0;i<20;i++){
        for(int j=0;j<20;j++)
            if(mapa[i][j]=='R')
                mapa[i][j]='.';
    }
    int x,y;
    R->getPozitie(x,y);
    if(x==20 || y==20 || R->getViata()==0){
        cout<<"LOSE!!!\n";
        return false;
    }
    mapa[x][y]='R';
    if(castigator(x,y)){
        cout<<"WIN!!!\n";
        return false;
    }
    return true;
}
//daca robotul ajunge in stop se declara castigator
bool harta::castigator(int x,int y){
    if(Xstop==x && Ystop==y)
        return true;
    return false;
}
//functie ce primeste un caracter si il plaseaza pe mapa[i][j]
void harta::plasare(int i, int j,const char c){
    mapa[i][j]=c;
}
harta::~harta(){
    for(int i=0;i<20;i++)
        delete[]mapa[i];
    delete[]mapa;
    delete R;
    for(int i=0;i<10;i++)
        if(C[i]!=NULL)
            delete C[i];
}
#endif // harta.h

