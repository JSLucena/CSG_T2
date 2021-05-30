
#ifndef building_hpp
#define building_hpp

#include <iostream>
using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Poligono.h"
#include "RGB.h"
#include <vector>

class Building
{

    ModeloMatricial Sprite;
    Poligono Hitbox;
    float posX,posY;
    int type;
public:
    Building(float startPosX, float startPosY, int type);
    int getPosX() {return posX;};
    int getPosY() {return posY;};
    int getType() {return type;};

    ModeloMatricial getSprite(){return Sprite;};


    void drawSprite(RGB Palette[100]);
    Poligono getHitbox(){ return Hitbox; };

    void updateHitbox();

};
Building::Building(float startPosX, float startPosY, int type)
{
    string fname = "./sprites/house" + to_string(type) + ".txt";
    const char *filename = fname.c_str();
    Sprite.leModelo(filename);
    posX = startPosX;
    posY = startPosY;
    this->type = type;

    updateHitbox();

}
void Building::updateHitbox()
{
    Hitbox = Poligono();
    Hitbox.insereVertice(Ponto(posX+1,posY+1));
    Hitbox.insereVertice(Ponto(posX-Sprite.width+1,posY+1));
    Hitbox.insereVertice(Ponto(posX-Sprite.width+1,posY-Sprite.height+1));
    Hitbox.insereVertice(Ponto(posX+1,posY-Sprite.height+1));

   // Hitbox.imprime();
  //  cout << endl;
}
void Building::drawSprite(RGB Palette[100])
{
    float offsetX = 0, offsetY = 0;
    int height = Sprite.height;
    int width = Sprite.width;
    Sprite.desenhaModelo(posX,posY,Palette);
    Hitbox.desenhaPoligono();
}





#endif

