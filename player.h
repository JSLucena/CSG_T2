#ifndef player_hpp
#define player_hpp

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

class Player
{

    float speed;
    int direction;
    float shotAngle;
    int shotPower;
    ModeloMatricial Sprite;
    Poligono Hitbox;
    float posX,posY;
public:
    Player(float startPosX, float startPosY);

    void setspeed(float f){
        speed = f;
    };
    float getspeed(){ return speed; };

    void setDirection(int i){
        direction = i;
    };
    int getDirection(){ return direction; };

    void setShotAngle(float f){
        shotAngle = f;
    };
    float getShotAngle(){ return shotAngle;};

    void setShotPower(int i){
        shotPower = i;
    };
    int getShotPower() {return shotPower;};


    void drawSprite();
    Poligono getHitbox(){ return Hitbox; };

    void updateHitbox();


};
Player::Player(float startPosX, float startPosY)
{
    Sprite.leModelo("./sprites/player");
    speed = 0;
    direction = 0;
    shotAngle = 0;
    shotPower = 0;
    posX = startPosX;
    posY = startPosY;

}
void Player::updateHitbox()
{
    Hitbox = Poligono();
    Hitbox.insereVertice(Ponto(posX,posY));
    Hitbox.insereVertice(Ponto(posX+Sprite.width,posY));
    Hitbox.insereVertice(Ponto(posX+Sprite.width,posY-Sprite.height));
    Hitbox.insereVertice(Ponto(posX,posY-Sprite.height));
    Hitbox.imprime();
}





#endif
