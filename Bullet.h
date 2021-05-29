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
Ponto Curva1[3];

// **********************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;

    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}
// **********************************************************************
void TracaBezier3Pontos(Ponto curva[3])
{
    double t=0.0;
    double DeltaT = 1.0/10;
    //cout << "DeltaT: " << DeltaT << endl;
    glBegin(GL_LINE_STRIP);
    while(t<1.0)
    {
        Ponto P = CalculaBezier3(curva, t);
        glVertex2f(P.x, P.y);
        t += DeltaT;
    }
    glEnd();
}




class Bullet
{
    Poligono shape;
    float angle;
    float speedX, speedY;
    float startingSpeedX, startingSpeedY;
    float posX, posY;
    bool isAlly;
    Ponto Bezier[3];
    Ponto Rotation;
public:
    Bullet(float startX, float startY, float force, float angle, float ymax);
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
    void drawShape();
    void moveBullet(float dt);
    void applyGravity(float gravity);


};
Bullet::Bullet(float startX, float startY, float force, float angle,float ymax)
{
   // shape.LePoligono("Triangulo.txt");

    shape = Poligono();
    posX = startX;
    posY = startY;



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

    startingSpeedX = - force * sin(this->angle*PI/180);
    startingSpeedY = force * cos(this->angle*PI/180);
    speedX = - force * sin(this->angle*PI/180);
    speedY = force * cos(this->angle*PI/180);

    Bezier[0] = Ponto(startX,startY);
    Bezier[1] = Ponto(speedX,speedY);
    Bezier[2] = Ponto(speedX*1.5,-ymax*0.85);
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

      //  TracaBezier3Pontos(Bezier);
    glPopMatrix();



   // glRotatef(angle, 0,0,1);
   // glTranslatef(-posX, -posY, 0);

}
void Bullet::moveBullet(float dt)
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
/////ja volto
void Bullet::applyGravity(float gravity)
{
    //speedX = speedX - sqrt(pow(startingSpeedX,2)+ 2*gravity + (posX - (posX + speedX)));
    speedY = sqrt(pow(startingSpeedY,2)+ 2*gravity + (posY - (posY + speedY)));

    speedY = speedY - gravity;
   // speedX = speedX - gravity/16;

}
#endif
