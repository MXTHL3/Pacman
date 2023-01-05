#include <iostream>
#include <math.h>
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


void show(MinGL &window){
    //window << Rectangle(Vec2D(30, 30), Vec2D(30, 30), KWhite);
    for (unsigned int row=0; row<rowNum; ++row){
        for (unsigned int col=0; col<colNum; ++col){
            if (col%2==0 and row%2==0){
                colX1=round(winWidth/colNum*col);
                colX2=round(winHeight/rowNum*row);
                rowY1=round(winWidth/colNum*(col+1));
                rowY2=round(winHeight/rowNum*(row+1));
                window << Rectangle(Vec2D(colX1, colX2),
                                    Vec2D(rowY1, rowY2),
                                    KBlue);
            }
        }
    }
}

int main(){
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
