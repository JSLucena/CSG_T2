
#ifndef enemy_hpp
#define enemy_hpp

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

class Enemy
{

    float speed;
    int direction;
    float shotAngle;
    int shotPower;
    ModeloMatricial Sprite;
    Poligono Hitbox;
    float posX,posY;
    int type;
public:
    Enemy(float startPosX, float startPosY, int type);
    Enemy();

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

    int getPosX() {return posX;};
    int getPosY() {return posY;};
    int getType() {return type;};

    ModeloMatricial getSprite(){return Sprite;};


    void drawSprite(RGB Palette[100]);
    Poligono getHitbox(){ return Hitbox; };

    void updateHitbox();
    void moveEnemy(float speed);
    void rotateEntity();


};
Enemy::Enemy()
{
    speed = 0;
}
Enemy::Enemy(float startPosX, float startPosY, int type)
{
    string fname = "./sprites/enemy" + to_string(type) + ".txt";
    const char *filename = fname.c_str();
    Sprite.leModelo(filename);
    speed = 0;
    shotAngle = 0;
    shotPower = 0;
    posX = startPosX;
    posY = startPosY;
    this->type = type;

}
void Enemy::updateHitbox()
{
    Hitbox = Poligono();
    Hitbox.insereVertice(Ponto(posX+1,posY+1));
    Hitbox.insereVertice(Ponto(posX-Sprite.width+1,posY+1));
    Hitbox.insereVertice(Ponto(posX-Sprite.width+1,posY-Sprite.height+1));
    Hitbox.insereVertice(Ponto(posX+1,posY-Sprite.height+1));
 //   Hitbox.desenhaPoligono();
   // Hitbox.imprime();
  //  cout << endl;
}
void Enemy::drawSprite(RGB Palette[100])
{
     float offsetX = 0, offsetY = 0;
     int height = Sprite.height;
     int width = Sprite.width;
    Sprite.desenhaModelo(posX,posY,Palette);

}
void Enemy::moveEnemy(float speed)
{

    posX = posX - speed;
    glTranslatef(posX,0,0);
    direction = 0;
}
void Enemy::rotateEntity()
{
    if(shotAngle > 80)
        shotAngle = 80;
    else if(shotAngle < -80)
        shotAngle = -80;


    Ponto P = Ponto(posX - Sprite.width/2,posY - Sprite.height/2);

    glTranslatef(P.x, P.y, P.z);
    glRotatef(shotAngle, 0,0,1);
    glTranslatef(-P.x, -P.y, -P.z);
}




#endif
