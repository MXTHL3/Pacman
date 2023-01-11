#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "mingl/mingl.h"
#include <unistd.h>
#include "mingl/shape/rectangle.h"
#include "mingl/shape/circle.h"
#include "mingl/shape/line.h"
#include "mingl/shape/triangle.h"
#include <vector>
#include <stdlib.h>

using namespace std;
using namespace nsGraphics;
using namespace nsShape;

double winWidth=1200;
double winHeight=800;
double colNum=31;
double rowNum=21;
double X1,Y1,X2,Y2;
int r;
int rad;

vector<vector <int>> mapMemory (rowNum, vector<int>(colNum,0));

bool initMapVar=true;
void initMap(){
    for (unsigned int row=0; row<rowNum; ++row){
        for (unsigned int col=0; col<colNum/2+1; ++col){
            if (row==0 or row==rowNum-1 or col==0){
                mapMemory[row][col]=1;
                mapMemory[row][colNum-col-1]=1;
            }
            if ((row>=2 and row<=rowNum-2) and (col>=2 and col<=colNum-2)){
                if (row%2==0 and col%2==0){
                    mapMemory[row][col]=1;
                    mapMemory[row][colNum-col-1]=1;
                }
            }
            if ((row>=1 and row<=rowNum-2) and (col>=1 and col<=colNum-1)){
                if (row%2==0 and col%2==1){
                    r=rand()%5;
                    if (r==1){
                        mapMemory[row][col]=1;
                        mapMemory[row][colNum-col-1]=1;
                    }
                    else{
                        mapMemory[row][col]=2;
                        mapMemory[row][colNum-col-1]=2;
                    }
                }
                if (row%2==1 and col%2==0){
                    r=rand()%5;
                    if (r==1){
                        mapMemory[row][col]=1;
                        mapMemory[row][colNum-col-1]=1;
                    }
                    else{
                        mapMemory[row][col]=2;
                        mapMemory[row][colNum-col-1]=2;
                    }
                }
                if (row%2==1 and col%2==1){
                    mapMemory[row][col]=2;
                    mapMemory[row][colNum-col-1]=2;
                }
            }
        }
    }
    mapMemory[0][colNum/2]=0;
    mapMemory[0][colNum/2]=2;
    mapMemory[rowNum-1][colNum/2]=0;
    mapMemory[rowNum-1][colNum/2]=2;
    mapMemory[rowNum/2][0]=0;
    mapMemory[rowNum/2][0]=2;
    mapMemory[rowNum/2][colNum-1]=0;
    mapMemory[rowNum/2][colNum-1]=2;

    int centerX1=(int)(colNum)/2-2-(int)(colNum)/2%2;
    int centerY1=(int)(rowNum)/2-2-(int)(rowNum)/2%2;
    int centerX2=(int)(colNum)/2+3+(int)(colNum)/2%2;
    int centerY2=(int)(rowNum)/2+3+(int)(rowNum)/2%2;
    for (int plusCenterY=0; plusCenterY<centerY2-centerY1; ++plusCenterY){
        for (int plusCenterX=0; plusCenterX<centerX2-centerX1; ++plusCenterX){
            if ((plusCenterY==0 and plusCenterX!=(centerX2-centerX1)/2) or plusCenterY==centerY2-centerY1-1 or
                plusCenterX==0 or plusCenterX==centerX2-centerX1-1){
                mapMemory[centerY1+plusCenterY][centerX1+plusCenterX]=1;
            }
            else{
                mapMemory[centerY1+plusCenterY][centerX1+plusCenterX]=0;
            }
        }
    }

    mapMemory[(int)(rowNum*0.75)][(int)(colNum)/2]=0;
    mapMemory[(int)(rowNum*0.75)][(int)(colNum)/2]=3;

    initMapVar=false;
}


void show(MinGL &window){
    //window << Rectangle(Vec2D(30, 30), Vec2D(30, 30), KWhite);
    for (unsigned int row=0; row<rowNum; ++row){
        for (unsigned int col=0; col<colNum; ++col){
            X1=round(winWidth/colNum*col);
            Y1=round(winHeight/rowNum*row);
            X2=round(winWidth/colNum*(col+1));
            Y2=round(winHeight/rowNum*(row+1));
            if (mapMemory[row][col]==1){
                window << Rectangle(Vec2D(X1, Y1), Vec2D(X2, Y2), KNavy);
                window << Rectangle(Vec2D(X1, Y1-(Y2-Y1)/4), Vec2D(X2, Y2-(Y2-Y1)/4), KBlue);
            }
            if (mapMemory[row][col]==2){
                rad=round(double((X2-X1)/6));
                X1=round(double(X1+(X2-X1)*0.5));
                Y1=round(double(Y1+(Y2-Y1)*0.5));
                window << Circle(Vec2D(X1, Y1), rad, KRed);
            }
            if (mapMemory[row][col]==3){
                //window << Circle(Vec2D(X1+(X2-X1)/2,Y1+(X2-X1)/2), (X2-X1)/2, KYellow);
            }
        }
    }
}



Vec2D position(400,300);
bool boucheOuverte = true;
void clavier(MinGL &window)
{
    // On vérifie si ZQSD est pressé, et met à jour la position de Pacman
    if (window.isPressed({'z', false}))
        position.setY(position.getY() - 10);
    if (window.isPressed({'s', false}))
        position.setY(position.getY() + 10);
    if (window.isPressed({'q', false}))
        position.setX(position.getX() - 10);
    if (window.isPressed({'d', false}))
        position.setX(position.getX() + 10);
}
void dessinerPacmanFerme(MinGL &window)
{
    window << Circle(position, 15, KYellow);
}
void dessinerPacmanOuvert(MinGL &window)
{
     if (window.isPressed({'z', false})){
         window << Circle (position, 15, KYellow);
         const int MOUTH_X1 = position.getX() - 15; // bouche ouverte vers la droite
         const int MOUTH_Y1 = position.getY() - 15;
         const int MOUTH_X2 = position.getX() + 15; // bouche ouverte vers la gauche
         const int MOUTH_Y2 = position.getY() - 15;
         const int MOUTH_X3 = position.getX(); // pointe de la bouche au milieu
         const int MOUTH_Y3 = position.getY();
         window << Triangle (Vec2D(MOUTH_X1, MOUTH_Y1), Vec2D(MOUTH_X2, MOUTH_Y2), Vec2D(MOUTH_X3, MOUTH_Y3), KBlack);
     }
    if (window.isPressed({'s', false})){
    window << Circle (position, 15, KYellow);
    const int MOUTH_X1 = position.getX() + 15; // bouche ouverte vers la droite
    const int MOUTH_Y1 = position.getY() + 15;
    const int MOUTH_X2 = position.getX() - 15; // bouche ouverte vers la gauche
    const int MOUTH_Y2 = position.getY() + 15;
    const int MOUTH_X3 = position.getX(); // pointe de la bouche au milieu
    const int MOUTH_Y3 = position.getY();
    window << Triangle (Vec2D(MOUTH_X1, MOUTH_Y1), Vec2D(MOUTH_X2, MOUTH_Y2), Vec2D(MOUTH_X3, MOUTH_Y3), KBlack);
    }
    if (window.isPressed({'q', false})){
        window << Circle (position, 15, KYellow);
        const int MOUTH_X1 = position.getX()-1; // bouche ouverte vers la droite
        const int MOUTH_Y1 = position.getY()+1;
        const int MOUTH_X2 = position.getX()-30; // bouche ouverte vers la gauche
        const int MOUTH_Y2 = position.getY()+30;
        const int MOUTH_X3 = position.getX(); // pointe de la bouche au milieu
        const int MOUTH_Y3 = position.getY()-35;
        window << Triangle (Vec2D(MOUTH_X1, MOUTH_Y1), Vec2D(MOUTH_X2, MOUTH_Y2), Vec2D(MOUTH_X3, MOUTH_Y3), KBlack);
    }
    if (window.isPressed({'d', false})){
        window << Circle (position, 15, KYellow);
        const int MOUTH_X1 = position.getX()+1; // bouche ouverte vers la droite
        const int MOUTH_Y1 = position.getY()-1;
        const int MOUTH_X2 = position.getX()+30; // bouche ouverte vers la gauche
        const int MOUTH_Y2 = position.getY()-30;
        const int MOUTH_X3 = position.getX(); // pointe de la bouche au milieu
        const int MOUTH_Y3 = position.getY()+35;
        window << Triangle (Vec2D(MOUTH_X1, MOUTH_Y1), Vec2D(MOUTH_X2, MOUTH_Y2), Vec2D(MOUTH_X3, MOUTH_Y3), KBlack);

    }
}
void animationPacman(MinGL &window)
{
// On inverse la valeur de la variable boucheOuverte toutes les 500 milliseconds
static int tempsPrecedent = 0;
int tempsActuel = glutGet(GLUT_ELAPSED_TIME);
if (tempsActuel - tempsPrecedent >= 500)
{
boucheOuverte = !boucheOuverte;
tempsPrecedent = tempsActuel;
}
// On dessine Pac-Man en fonction de la valeur de boucheOuverte
if (boucheOuverte)
    dessinerPacmanOuvert(window);
else
    dessinerPacmanFerme(window);
}

int main(){
if (initMapVar==true){
       initMap();
    }
    cout << (int)(colNum)/2%2 << " " << (int)(colNum)/2-1 << " " << (int)(colNum)/2+1 << endl;
    cout << (int)(rowNum)/2%2 << " " << (int)(rowNum)/2-1 << " " << (int)(rowNum)/2+1;

// Initialise le système
MinGL window("Pac-Man",Vec2D(winWidth, winHeight), nsGraphics::Vec2D(128, 128), KBlack);
window.initGlut();
window.initGraphic();
// On fait tourner la boucle tant que la fenêtre est ouverte
while (window.isOpen())
{
    // On récupère le temps actuel
    auto start_time = std::chrono::steady_clock::now();
    // On efface la fenêtre
    window.clearScreen();
    // On met à jour la position du pac man
    clavier(window);
    show(window);
    // On dessine l'animation de Pac-Man
    animationPacman(window);
    // On finit la frame en cours
    window.finishFrame();
    //
    // On attend le temps restant pour atteindre 60 FPS
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
    auto remaining_time = 1000 / 60 - elapsed_time;
    if (remaining_time > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(remaining_time));
}
return 0;
}

