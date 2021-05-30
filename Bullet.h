#ifndef Bullet_hpp
#define Bullet_hpp

#include <iostream>
#include <iomanip>
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

# define PI          3.141592653589793238462643383279502884L

#include "Poligono.h"
#include <vector>





class Bullet
{
    Poligono shape;
    float angle;
    float speedX, speedY;
    float startingSpeedX, startingSpeedY;
    float posX, posY;
    float strtX, strtY;
    bool isAlly;
    Ponto Rotation;
public:
    Bullet(float startX, float startY, float force, float angle, float ymax, float bulletspeed);
    float getAngle(){return angle;};
    void setAngle(float a){
        angle = a;
    }

    void setIsAlly(bool b) {
        isAlly = b;
    }
    bool getIsAlly(){return isAlly;};
    float getPosX(){return posX;};
    float getPosY(){return posY;};
    Poligono getShape(){return shape;};
    void drawShape();
    void moveBullet(float dt);
    void applyGravity(float gravity);


};
Bullet::Bullet(float startX, float startY, float force, float angle,float ymax, float bulletspeed)
{

    shape = Poligono();
    posX = startX;
    posY = startY;
    strtX = startX;
    strtY = startY;


    shape.insereVertice(Ponto(posX,posY));
    shape.insereVertice(Ponto(posX+2,posY+4));
    shape.insereVertice(Ponto(posX+4,posY));

    Rotation = Ponto(posX+2,posY+4);

/*
    shape.insereVertice(Ponto(0,0));
    shape.insereVertice(Ponto(2,2));
    shape.insereVertice(Ponto(4,0));
    */


    this->angle = angle;

    startingSpeedX = - force * sin(this->angle*PI/180) * bulletspeed;
    startingSpeedY = force * cos(this->angle*PI/180) * bulletspeed;
    speedX = - force * sin(this->angle*PI/180) * bulletspeed;
    speedY = force * cos(this->angle*PI/180) * bulletspeed;

}

void Bullet::drawShape()
{

    glPushMatrix();
        glTranslatef(posX,0,0);
        glLineWidth(2);
        glColor3f(1,1,0); // R, G, B  [0..1]
        glTranslatef(Rotation.x, Rotation.y, Rotation.z);
        glRotatef(angle, 0,0,1);
        glTranslatef(-Rotation.x, -Rotation.y, -Rotation.z);
        shape.desenhaPoligono();
    glPopMatrix();




}
void Bullet::moveBullet(float dt) ///para movimentarmos a bala recriamos o poligono dela, meio gambiarra mas funciona
{
    posX = posX + speedX * dt;
    posY = posY + speedY * dt;
     cout << "(" << speedX << "," << speedY << ")" << endl;
    shape = Poligono();
    shape.insereVertice(Ponto(posX,posY));
    shape.insereVertice(Ponto(posX+2,posY+2));
    shape.insereVertice(Ponto(posX+4,posY));

    Rotation = Ponto(posX+2,posY+4);

}

void Bullet::applyGravity(float gravity) /// executamos uma desaceleracao simples no eixo Y
{

    speedY = speedY - gravity;


}
#endif
