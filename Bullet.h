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
    float posX, posY;
    bool isAlly;
public:
    Bullet(float startX, float startY, float force, float angle);
    float getAngle(){return angle;};
    void setAngle(float a){
        angle = a;
    }

    void setIsAlly(bool b) {
        isAlly = b;
    }
    bool getIsAlly(){return isAlly;};
    void drawShape();
    void moveBullet();


};
Bullet::Bullet(float startX, float startY, float force, float angle)
{
   // shape.LePoligono("Triangulo.txt");

    shape = Poligono();
    posX = startX;
    posY = startY;

    shape.insereVertice(Ponto(posX,posY));
    shape.insereVertice(Ponto(posX+2,posY+2));
    shape.insereVertice(Ponto(posX+4,posY));


/*
    shape.insereVertice(Ponto(0,0));
    shape.insereVertice(Ponto(2,2));
    shape.insereVertice(Ponto(4,0));
    */


    angle = angle;
    speedX = force * cos(angle*PI/180);
    speedY = force * sin(angle*PI/180);
}

void Bullet::drawShape()
{
    shape.desenhaPoligono();




   // glTranslatef(posX, posY, 0);
   // glRotatef(angle, 0,0,1);
   // glTranslatef(-posX, -posY, 0);

}
void Bullet::moveBullet()
{
    posX = posX + speedX;
    posY = posY + speedY;
    glTranslatef(posX,posY,0);
}
#endif
