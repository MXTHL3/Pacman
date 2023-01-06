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

void keyboard(MinGL &window){
    if (window.isPressed({'z', false})){
        cout << 1;
    }
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
                window << Circle(Vec2D(X1, Y1), rad, KOrange);
            }
            if (mapMemory[row][col]==3){
                window << Circle(Vec2D(X1+(X2-X1)/2,Y1+(X2-X1)/2), (X2-X1)/2, KYellow);
            }
        }
    }
}

int main(){
    if (initMapVar==true){
       initMap();
    }
    cout << (int)(colNum)/2%2 << " " << (int)(colNum)/2-1 << " " << (int)(colNum)/2+1 << endl;
    cout << (int)(rowNum)/2%2 << " " << (int)(rowNum)/2-1 << " " << (int)(rowNum)/2+1;
    MinGL window("Pacman", Vec2D(winWidth, winHeight), Vec2D(0, 0), KBlack);
    window.initGlut();
    window.initGraphic();
    while (window.isOpen()){
        window.clearScreen();
        keyboard(window);
        show(window);
        window.finishFrame();
        window.getEventManager().clearEvents();

    }
    return 0;
}
