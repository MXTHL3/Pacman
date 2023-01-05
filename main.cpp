#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "MinGL2/include/mingl/mingl.h"
#include "MinGL2/include/mingl/shape/rectangle.h"

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
            }
            if (row%2==1 and col%2==0){
                r=rand()%3;
                if (r==1){
                    mapMemory[row][col]=1;
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
            colX1=round(winWidth/colNum*col);
            colX2=round(winHeight/rowNum*row);
            rowY1=round(winWidth/colNum*(col+1));
            rowY2=round(winHeight/rowNum*(row+1));
            if (mapMemory[col][row]==1){
                window << Rectangle(Vec2D(colX1, colX2),
                                    Vec2D(rowY1, rowY2),
                                    KBlue);
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

