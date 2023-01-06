#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "MinGL2/include/mingl/mingl.h"
#include "MinGL2/include/mingl/shape/rectangle.h"
#include "MinGL2/include/mingl/shape/circle.h"

using namespace std;
using namespace nsGraphics;
using namespace nsShape;

double winWidth=1000;
double winHeight=600;
double colNum=51;
double rowNum=31;
double colX1,colX2,rowY1,rowY2;
int r;

vector<vector <int>> mapMemory (rowNum, vector<int>(colNum,0));

bool initMapVar=true
void initMap(){
    for (unsigned int row=0; row<rowNum; ++row){
        for (unsigned int col=0; col<colNum; ++col){
            if (row%2==0 and col%2==0){
                mapMemory[row][col]=1;
            }
            if (row%2==0 and col%2==1){
                r=rand()%3;
                if (r==1){
                    mapMemory[row][col]=1;
                }
                else{
                    mapMemory[row][col]=2;
                }
            }
            if (row%2==1 and col%2==0){
                r=rand()%3;
                if (r==1){
                    mapMemory[row][col]=1;
                }   
                else{
                    mapMemory[row][col]=2;
                }
            }          
        }
    }
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
                window << Rectangle(Vec2D(X1, Y1),Vec2D(X2, Y2), KBlue);
            }
            if (mapMemory[row][col]==2){
                X1=round(double(X1+(X2-X1)*0.4));
                Y1=round(double(Y1+(Y2-Y1)*0.4));
                window << Circle(Vec2D(X1, Y1),5, KOrange);
            }
        }
    }
}

int main(){
    if (initMapVar==true){
       initMap();
    }
    MinGL window("Pacman", Vec2D(winWidth, winHeight), Vec2D(0, 0), KBlack);
    window.initGlut();
    window.initGraphic();
    while (window.isOpen()){
        window.clearScreen();
        show(window);
        window.finishFrame();
        window.getEventManager().clearEvents();

    }
    return 0;
}

