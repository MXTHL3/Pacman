#define FPS_LIMIT 30

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "MinGL2/include/mingl/mingl.h"
#include "MinGL2/include/mingl/shape/rectangle.h"
#include "MinGL2/include/mingl/shape/circle.h"
#include "MinGL2/include/mingl/shape/triangle.h"
#include "MinGL2/include/mingl/gui/text.h"
#include <thread>

using namespace std;
using namespace nsGraphics;
using namespace nsShape;
using namespace nsGui;

double winWidth=1200; //dimensions fenêtre
double winHeight=800;
int colNum=31;    //résolution cases
int rowNum=21;
int X1,Y1,X2,Y2; //coordonnées case
int r;
int rad; //rayon
int score=0;
int leftGums;
int r2;
bool exitCenter=false;
int countMainLoop=0;

vector<vector<int>> mapMemory (rowNum, vector<int>(colNum,0)); //initialisation matrice carte
vector<vector<int>> chCoords (4, vector<int>(2,0)); //coordonnées de Pacman et des trois fantômes
vector<vector<int>> pacmanMouth = {{0,0,1,0},{1,0,1,1},{0,1,1,1},{0,0,0,1}};
int pacmanDirection=1;
vector<RGBAcolor> ghostColors = {KCyan, KPink, KOrange};
int iGhost=0;
vector<int> ghostsDirections = {3,0,1};
vector<vector<int>> URDL = {{0,-1},{1,0},{0,1},{-1,0}}; //URDL -> Up Right Down Left

char paramUp='z';
char paramRight='d';
char paramDown='s';
char paramLeft='q';

bool initMapVar=true;
bool activeSuperPacGum=false;
bool gameOver=false;
bool won=false;

void initMap(){
    //0 -> espace
    //1 -> mur
    //2 -> pac-gomme
    //3 -> super pac-gomme
    srand(time(NULL));
    for (int row=0; row<rowNum; ++row){
        for (int col=0; col<colNum/2+1; ++col){
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
                    r=rand()%4;
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
                    r=rand()%4;
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
    mapMemory[rowNum/2][0]=0;
    mapMemory[rowNum/2][0]=2;
    mapMemory[rowNum/2][colNum-1]=0;
    mapMemory[rowNum/2][colNum-1]=2;

    int centerX1=colNum/2-2-colNum/2%2;
    int centerY1=rowNum/2-2-rowNum/2%2;
    int centerX2=colNum/2+3+colNum/2%2;
    int centerY2=rowNum/2+3+rowNum/2%2;
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

    for (int y=1; y<centerY1; ++y){
        mapMemory[y][colNum/2]=2;
        mapMemory[rowNum-y-1][colNum/2]=2;
    }
    for (int x=0; x<5; ++x){
        mapMemory[rowNum/2][x]=2;
        mapMemory[rowNum/2][colNum-x-1]=2;
    }

    //super pac-gommes
    mapMemory[1][1]=3;
    mapMemory[1][colNum-2]=3;
    mapMemory[rowNum-2][1]=3;
    mapMemory[rowNum-2][colNum-2]=3;

    //pos. Pacman
    mapMemory[(int)(rowNum*0.75)][(int)(colNum)/2]=0;
    chCoords[0][0]=(int)(colNum)/2;
    chCoords[0][1]=(int)(rowNum*0.75);

    //pos. fantôme cyan
    chCoords[1][0]=(int)(colNum)/2-1;
    chCoords[1][1]=(int)(rowNum)/2;

    //pos. fantôme rose
    chCoords[2][0]=(int)(colNum)/2;
    chCoords[2][1]=(int)(rowNum)/2;

    //pos. fantôme orange
    chCoords[3][0]=(int)(colNum)/2+1;
    chCoords[3][1]=(int)(rowNum)/2;
}

void aleaGhost(){ //mouvement aléatoire des fantômes
    for (int ghost=1; ghost<(int)(chCoords.size()); ++ghost){
        r2=rand()%4;
        while (mapMemory[chCoords[ghost][1]+URDL[r2][1]][chCoords[ghost][0]+URDL[r2][0]]==1){
            r2=rand()%4;
        }
        if (r2==0){
            ghostsDirections[ghost-1]=0;
            --chCoords[ghost][1];
        }
        if (r2==1){
            ghostsDirections[ghost-1]=1;
            ++chCoords[ghost][0];
            if (chCoords[1][0]>=colNum){
                chCoords[1][0]=0;
            }
        }
        if (r2==2){
            ghostsDirections[ghost-1]=2;
            ++chCoords[ghost][1];
        }
        if (r2==3){
            ghostsDirections[ghost-1]=3;
            --chCoords[ghost][0];
            if (chCoords[1][0]<0){
                chCoords[1][0]=colNum-1;
            }
        }
    }
}

void keyboard(MinGL &window){
    if (window.isPressed({paramUp, false})){ //Pacman -> haut
        if (mapMemory[chCoords[0][1]+URDL[0][1]][chCoords[0][0]+URDL[0][0]]!=1){
            pacmanDirection=0;
            --chCoords[0][1];
        }
    }
    else if (window.isPressed({paramRight, false})){ //Pacman -> droite
        if (mapMemory[chCoords[0][1]+URDL[1][1]][chCoords[0][0]+URDL[1][0]]!=1){
            pacmanDirection=1;
            ++chCoords[0][0];
            if (chCoords[0][0]>colNum-1){
                chCoords[0][0]=0;
            }
        }
    }
    else if (window.isPressed({paramDown, false})){ //Pacman -> bas
        if (mapMemory[chCoords[0][1]+URDL[2][1]][chCoords[0][0]+URDL[2][0]]!=1){
            pacmanDirection=2;
            ++chCoords[0][1];
        }
    }
    else if (window.isPressed({paramLeft, false})){ //Pacman -> gauche
        if (mapMemory[chCoords[0][1]+URDL[3][1]][chCoords[0][0]+URDL[3][0]]!=1){
            pacmanDirection=3;
            --chCoords[0][0];
            if (chCoords[0][0]<0){
                chCoords[0][0]=colNum-1;
            }
        }
    }
    if (mapMemory[chCoords[0][1]][chCoords[0][0]]==2){
        mapMemory[chCoords[0][1]][chCoords[0][0]]=0;
        ++score;
    }
    if (mapMemory[chCoords[0][1]][chCoords[0][0]]==3){
        mapMemory[chCoords[0][1]][chCoords[0][0]]=0;
        countMainLoop=0;
        activeSuperPacGum=true;
    }
    if ((chCoords[0][1]==chCoords[1][1] and chCoords[0][0]==chCoords[1][0]) or
        (chCoords[0][1]==chCoords[2][1] and chCoords[0][0]==chCoords[2][0]) or
        (chCoords[0][1]==chCoords[3][1] and chCoords[0][0]==chCoords[3][0])){
        if (activeSuperPacGum==false){
            gameOver=true;
        }
    }
    if (chCoords[0][1]==chCoords[1][1] and chCoords[0][0]==chCoords[1][0]){
        if (activeSuperPacGum==true){
            chCoords[1][1]=rowNum/2;
            chCoords[1][0]=colNum/2;
        }
    }
    if (chCoords[0][1]==chCoords[2][1] and chCoords[0][0]==chCoords[2][0]){
        if (activeSuperPacGum==true){
            chCoords[2][1]=rowNum/2;
            chCoords[2][0]=colNum/2;
        }
    }
    if (chCoords[0][1]==chCoords[3][1] and chCoords[0][0]==chCoords[3][0]){
        if (activeSuperPacGum==true){
            chCoords[3][1]=rowNum/2;
            chCoords[3][0]=colNum/2;
        }
    }
}

void show(MinGL &window){ //affiche à partir du contenu de la matrice mapMemory
    for (int row=0; row<rowNum; ++row){
        for (int col=0; col<colNum; ++col){
            X1=round(winWidth/colNum*col);
            Y1=round(winHeight/rowNum*row);
            X2=round(winWidth/colNum*(col+1));
            Y2=round(winHeight/rowNum*(row+1));
            if (mapMemory[row][col]==1){
                window << Rectangle(Vec2D(X1, Y1), Vec2D(X2, Y2), KNavy); //mur de base
            }
            if (row==chCoords[0][1] and col==chCoords[0][0]){
                window << Circle(Vec2D(X1+(X2-X1)/2,Y1+(Y2-Y1)/2), (X2-X1)/2, KYellow);
                window << Triangle(Vec2D(X1+(X2-X1)/2,Y1+(Y2-Y1)/2),
                                   Vec2D(X1+(X2-X1)*pacmanMouth[pacmanDirection][0],Y1+(Y2-Y1)*pacmanMouth[pacmanDirection][1]),
                                   Vec2D(X1+(X2-X1)*pacmanMouth[pacmanDirection][2],Y1+(Y2-Y1)*pacmanMouth[pacmanDirection][3]),
                                   KBlack);
            }
            if (mapMemory[row][col]==2){
                rad=round(double((X2-X1)/6));
                window << Circle(Vec2D(X1+(X2-X1)*0.5, Y1+(Y2-Y1)*0.5), rad, KOrange);
            }
            if (mapMemory[row][col]==3){
                rad=round(double((X2-X1)/3));
                window << Circle(Vec2D(X1+(X2-X1)*0.5, Y1+(Y2-Y1)*0.5), rad, KRed);
            }
            if ((row==chCoords[1][1] and col==chCoords[1][0]) or
                (row==chCoords[2][1] and col==chCoords[2][0]) or
                (row==chCoords[3][1] and col==chCoords[3][0])){
                if (activeSuperPacGum==false){
                    window << Circle(Vec2D(X1+(X2-X1)/2,Y1+(X2-X1)/2), (X2-X1)/2, ghostColors[iGhost]);
                    window << Rectangle(Vec2D(X1,Y1+(Y2-Y1)/2),Vec2D(X2,Y2), ghostColors[iGhost]);
                }
                else{
                    window << Circle(Vec2D(X1+(X2-X1)/2,Y1+(X2-X1)/2), (X2-X1)/2, KNavy);
                    window << Rectangle(Vec2D(X1,Y1+(Y2-Y1)/2),Vec2D(X2,Y2), KNavy);
                }
                window << Triangle(Vec2D(X1,Y2), Vec2D(X1+(X2-X1)/6,Y2-(Y2-Y1)/4), Vec2D(X1+(X2-X1)/3,Y2), KBlack);
                window << Triangle(Vec2D(X1+(X2-X1)/3,Y2), Vec2D(X1+(X2-X1)/2,Y2-(Y2-Y1)/4), Vec2D(X2-(X2-X1)/3,Y2), KBlack);
                window << Triangle(Vec2D(X2-(X2-X1)/3,Y2), Vec2D(X2-(X2-X1)/6,Y2-(Y2-Y1)/4), Vec2D(X2,Y2), KBlack);
                window << Circle(Vec2D(X1+(X2-X1)/3,Y1+(Y2-Y1)/4), (X2-X1)/8, KWhite);
                window << Circle(Vec2D(X2-(X2-X1)/3,Y1+(Y2-Y1)/4), (X2-X1)/8, KWhite);
                if (activeSuperPacGum==false){
                    window << Circle(Vec2D(X1+(X2-X1)/3,Y1+(Y2-Y1)/4), (X2-X1)/8, KWhite);
                    window << Circle(Vec2D(X2-(X2-X1)/3,Y1+(Y2-Y1)/4), (X2-X1)/8, KWhite);
                    window << Circle(Vec2D(X1+(X2-X1)/3+URDL[ghostsDirections[iGhost]][0]*((X2-X1)/16),Y1+(Y2-Y1)/4)+URDL[ghostsDirections[iGhost]][1]*((X2-X1)/16), (X2-X1)/16, KBlack);
                    window << Circle(Vec2D(X2-(X2-X1)/3+URDL[ghostsDirections[iGhost]][0]*((X2-X1)/16),Y1+(Y2-Y1)/4)+URDL[ghostsDirections[iGhost]][1]*((X2-X1)/16), (X2-X1)/16, KBlack);
                }
                else{
                    window << Circle(Vec2D(X1+(X2-X1)/3,Y1+(Y2-Y1)/4), (X2-X1)/8, KRed);
                    window << Circle(Vec2D(X2-(X2-X1)/3,Y1+(Y2-Y1)/4), (X2-X1)/8, KRed);
                }
                ++iGhost;
                if (iGhost==(int)(ghostColors.size())){
                    iGhost=0;
                }
            }
            if (mapMemory[row][col]==1){
                window << Rectangle(Vec2D(X1, Y1-(Y2-Y1)/4), Vec2D(X2, Y2-(Y2-Y1)/4), KBlue); //haut du mur pour le relief
            }
        }
    }
    window << Text(Vec2D(0, 10), "x="+to_string(chCoords[0][0])+", y="+to_string(chCoords[0][1])+", SCORE: "+to_string(score)+", LEFT: "+to_string(leftGums), KWhite, GlutFont::BITMAP_9_BY_15);
    if (gameOver==true){
        window << Text(Vec2D(0, 30), "GAME OVER", KWhite, GlutFont::BITMAP_9_BY_15);
    }
    if (leftGums==0){
        window << Text(Vec2D(0, 30), "WON", KWhite, GlutFont::BITMAP_9_BY_15);
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
    chrono::microseconds frameTime = chrono::microseconds::zero();
    while (window.isOpen()){
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
        window.clearScreen();
        if (gameOver==false){
            if (exitCenter==false){
                for (int ghost=1; ghost<(int)(chCoords.size()); ++ghost){
                    if (mapMemory[chCoords[ghost][1]+URDL[0][1]][chCoords[ghost][0]+URDL[0][0]]){
                        if (countMainLoop%5==0){
                            chCoords[ghost][1]+=URDL[0][1];
                            show(window);
                        }
                    }
                }
                chCoords[1][0]+=URDL[1][0];
                chCoords[3][0]+=URDL[3][0];
                show(window);
                for (int ghost=1; ghost<(int)(chCoords.size()); ++ghost){
                    for (int i=0; i<2; ++i){
                        if (countMainLoop%5==0){
                            chCoords[ghost][1]+=URDL[0][1];
                        }
                        show(window);
                    }
                }
                exitCenter=true;
            }
            else{
                if (countMainLoop%5==0){
                    aleaGhost();
                }
            }
            keyboard(window);
        }
        leftGums=0;
        for (int row=0; row<rowNum; ++row){
            for (int col=0; col<colNum; ++col){
                if (mapMemory[row][col]==2){
                    ++leftGums;
                }
            }
        }
        show(window);
        window.finishFrame();
        window.getEventManager().clearEvents();
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) -
        chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));
        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
        ++countMainLoop;
        if (countMainLoop==201){
            countMainLoop=0;
            if (activeSuperPacGum==true){
                activeSuperPacGum=false;
            }
        }
    }
    return 0;
}
