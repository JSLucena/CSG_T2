#ifndef RGB_hpp
#define RGB_hpp

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

#include "Ponto.h"
#include <vector>

class RGB{

    public:

    int r,g,b;

    void imprime()

    {

        cout << "Cor: " << setw(4) << g << setw(4) << r << setw(4) << b;

    }

};

class ModeloMatricial{

    public:
        int height;
        int width;
        int M[50][50];

        void leModelo(const char *nome);

        void desenhaModelo(float posX, float posY, RGB Palette[100]);

};
void ModeloMatricial::leModelo(const char *nome)
{
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    unsigned int linhas, colunas;

    input >> linhas >> colunas;

    height = linhas;
    width = colunas;
    for (int i=0; i< linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            double x,y;
        // Le cada elemento da linha
        input >> M[i][j];
        if(!input)
            break;
        }

    }
    input.close();


    cout << "Modelo lido com sucesso!" << endl;

}
void ModeloMatricial::desenhaModelo(float posX, float posY, RGB Palette[100]) ///inverti algumas coisas aqui e ficou meio estranho, mas funciona
{
    float offsetX = 0, offsetY = 0;
    for(int i = 0; i < height;i++)
    {
        for(int j = 0; j < width; j++)
        {
            glColor3f(Palette[M[i][j]].r/255.0, Palette[M[i][j]].g/255.0, Palette[M[i][j]].b/255.0); // R, G, B  [0..1]
            glRectf(posX-offsetX,posY+offsetY,posX-offsetX+1,posY+offsetY+1);
            offsetX++;
        }
         offsetX=0;
         offsetY--;
    }
}




#endif
