// **********************************************************************
// PUCRS/Escola Politécnica
// COMPUTAÇÃO GRÁFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <vector>


using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include "Poligono.h"

#include "Temporizador.h"


////////////////////////
#include "RGB.h"
#include "player.h"
#include "Bullet.h"
#include "enemy.h"
#include "building.h"


#define REFRESHRATE 60
#define XMAX 100
#define YMAX 100
#define buildingcount 5
#define BSPEED 2
#define MAXENEMIES 10
////////////////////////
Temporizador T;
double AccumDeltaT=0;
float BulletTimer;

// Limites lógicos da área de desenho
Ponto Min, Max;
Poligono P1, P2;


double dt;
float side = 0;
float pos = 0;
float speed = 0.0;
float bulletSpeed = 0.0;

float timeToTravel = 15.0;
float gravity = 10;

bool desenha = false;

float angulo=0.0;
int enemySpawnRange = -20;
int HP = 3;
bool stopEnemySpawn = false;
int spawnedEnemies = 0;
bool shootCD; /// cooldown do tiro do jogador
int enemyShotChance = 1; ///chance do inimigo atirar a cada ciclo



void RotacionaAoRedorDeUmPonto(float alfa, Ponto P);
bool HaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n);
///////////////////////////////////////////////////////////////////
RGB PaletteGlobal[100];
ModeloMatricial teste;
Player player = Player(-XMAX*0.4,-YMAX* 0.75);
vector<Bullet> bullets;
vector<Bullet> playerBullet;
bool shoot = false;


Enemy testEnemy;
vector<Enemy> enemies;
vector<Building> buildings;




///////////////////////////////////////////////////////////////////
//###############################################################//
void leCores()
{
    ifstream input;            // ofstream arq;
    input.open("cores.txt", ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << "...";
    string S;
    //int nLinha = 0;
    unsigned int qCores;

    input >> qCores;  // arq << qtdVertices

    for (int i=0; i< qCores; i++)
    {
        // Le cada elemento da linha
        input >> PaletteGlobal[i+1].r >> PaletteGlobal[i+1].g >> PaletteGlobal[i+1].b;
        if(!input)
            break;
    }
    cout << "Cores lidas com sucesso!" << endl;

}

bool testaInterseccao(Poligono p1, Poligono p2)
{
    bool alreadyAddedp1,alreadyAddedp2, debug, isInside = false;
    Ponto auxA1,auxA2, auxB1,auxB2, newPonto;
    /// teste de interseccao
    for(int i = 0; i < p1.getNVertices(); i++)
    {
        for(int j = 0; j < p2.getNVertices(); j++)
        {
            if(j == p2.getNVertices() - 1 )
            {
                auxB1 = p2.getVertice(j);
                auxB2 = p2.getVertice(0);
            }
            else
            {
                auxB1 = p2.getVertice(j);
                auxB2 = p2.getVertice(j+1);
            }
            if (i == p1.getNVertices() - 1)
            {
                auxA1 = p1.getVertice(i);
                auxA2 = p1.getVertice(0);
            }
            else
            {
                auxA1 = p1.getVertice(i);
                auxA2 = p1.getVertice(i+1);
            }

            debug = HaInterseccao(auxA1,auxA2,auxB1,auxB2);

            if(debug == true)
            {

                return debug;


            }

        }
    }
    return false;
}

//###############################################################//

void printString(string s, int posX, int posY)
{
    glColor3f(1,1,1);
    glRasterPos2f(posX, posY); //define position on the screen
    for (int i = 0; i < s.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
  }
}
Building spawnBuilding(int range)
{
    int spawnPos = ((rand() % 15) + range);
    int type = (rand() % 3) + 1;

    return Building(spawnPos,-YMAX*0.69, type);

}
void spawnBullet()
{
    Bullet tempBullet = Bullet((player.getPosX() - player.getSprite().width/2 + 1), player.getPosY(),player.getShotPower(),player.getShotAngle(),YMAX, bulletSpeed);
    bullets.push_back(tempBullet);
}

bool BulletOutOfBounds(Bullet b)
{
    if(abs(b.getPosX()) > XMAX || abs(b.getPosY()) > YMAX*0.82)
        return true;
    return false;
}

void respawnEnemy(Enemy &e, int range)
{
    int respawnPos =(rand() % 10) + range;
    int tentativa= 0;
    for(auto en = enemies.begin(); en != enemies.end(); en++)
    {
        if (en->getPosX() < XMAX/2-10 && en->getPosX() > XMAX/2 - 25)
        {

            respawnPos+=10;
          //  en = enemies.begin();
            cout << "changed spawn pos" << endl;
        }

    }
    enemySpawnRange= respawnPos;
    e = Enemy(XMAX/2,respawnPos,e.getType());
}
Enemy spawnEnemy()
{
    int spawnPos = ((rand() % 2) + 1)*25;
    int type = (rand() % 3) + 1;

    for(auto en = enemies.begin(); en != enemies.end(); en++)
    {
        if (en->getPosX() < XMAX/2-10 && en->getPosX() > XMAX/2 - 25)
        {

            spawnPos+=10;
          //  en = enemies.begin();
            cout << "changed spawn pos" << endl;
        }
    }
    return Enemy(XMAX/2,spawnPos,type);




}
void playerHandler()
{
    glPushMatrix();
        //player.movePlayer(XMAX,REFRESHRATE);
        player.movePlayer(speed * dt);
        player.rotateEntity();
        player.drawSprite(PaletteGlobal);
        glColor3f(1,1,1);
        player.updateHitbox();


        /// Angulo e forca do disparo

        glColor3f(1,0,0);
        glLineWidth(1);
        glBegin(GL_LINES);
            glVertex3f((player.getPosX() - player.getSprite().width/2 + 1), player.getPosY(),0);
            glVertex3f(player.getPosX() - player.getSprite().width/2 + 1, player.getPosY() + player.getShotPower(),0);
        glEnd();
        RotacionaAoRedorDeUmPonto(player.getShotAngle(),Ponto(player.getPosX() - player.getSprite().width/2,player.getPosY()));


    glPopMatrix();

    if(shoot == true)
    {
        if(shootCD == false)
        {
           if(player.getShotPower() > 0 )
            {
                spawnBullet();
                bullets.back().setIsAlly(true);
                shootCD = true;
            }
        }


    }


    shoot = false;
}
void bulletHandler()
{
    cout << bullets.size() << " ";
    if(bullets.size() > 0 )
        for(auto b = bullets.begin(); b != bullets.end(); b++)
        {
            b->drawShape();
            b->moveBullet(dt);
            b->applyGravity(gravity);
          //  cout << b.getPosX() << "/" << b.getPosY() << endl;
            if(BulletOutOfBounds(*b))
            {
                bullets.erase(b);
                break;
            }

            if (b->getIsAlly()) ///testando as balas atiradas pelo jogador, que so pegam nos inimigos
            {
                for(auto e = enemies.begin(); e != enemies.end(); e++)
                {
                    Poligono P1 = b->getShape();
                    Poligono P2 = e->getHitbox();
                    if(testaInterseccao(P1,P2))
                    {
                         enemies.erase(e);
                         bullets.erase(b);
                         break;
                    }

                }
                break;
            }
            else ///testando as balas atiradas pelos inimigos, que pegam nos predios e no jogador
            {
                Poligono P1 = b->getShape();
                 if(testaInterseccao(P1,player.getHitbox()))
                    {
                         HP--;
                         bullets.erase(b);
                         break;
                    }
                for(auto bld = buildings.begin(); bld != buildings.end(); bld++)
                {
                    Poligono P1 = b->getShape();
                    Poligono P2 = bld->getHitbox();
                    if(testaInterseccao(P1,P2))
                    {
                         buildings.erase(bld);
                         bullets.erase(b);
                         break;
                    }
                }
                break;

            }

        }
}
void enemyHandler()
{
    for(auto e = enemies.begin(); e != enemies.end(); e++)
    {
        glPushMatrix();
            e->moveEnemy(speed*dt);
            e->drawSprite(PaletteGlobal);
            glColor3f(1,1,1);
            e->updateHitbox();
        glPopMatrix();
        if(abs(e->getPosX()) > XMAX/2 )
        {
            respawnEnemy(*e, enemySpawnRange);
            enemySpawnRange+=20;
            if (enemySpawnRange > YMAX*0.6)
                enemySpawnRange= -20;
        }

        int shotChance = (rand() % 500 ) + 1;
        if(shotChance <= enemyShotChance)
        {
            int randShotPower = (rand() % 5) + 1;
            int randAngle = rand() % 360;
            Bullet tempBullet = Bullet((e->getPosX() - e->getSprite().width/2 + 1), e->getPosY() - e->getSprite().height/2-1,randShotPower,randAngle,YMAX, bulletSpeed);
            bullets.push_back(tempBullet);
        }

    }

}
void buildingHandler()
{
    for(auto b = buildings.begin(); b != buildings.end(); b++)
    {
        glPushMatrix();
            b->drawSprite(PaletteGlobal);
        glPopMatrix();
    }
}
//################################################################




// **********************************************************************
//    Calcula o produto escalar entre os vetores V1 e V2
// **********************************************************************
double ProdEscalar(Ponto v1, Ponto v2)
{
    return v1.x*v2.x + v1.y*v2.y+ v1.z*v2.z;
}
// **********************************************************************
//    Calcula o produto vetorial entre os vetores V1 e V2
// **********************************************************************
void ProdVetorial (Ponto v1, Ponto v2, Ponto &vresult)
{
    vresult.x = v1.y * v2.z - (v1.z * v2.y);
    vresult.y = v1.z * v2.x - (v1.x * v2.z);
    vresult.z = v1.x * v2.y - (v1.y * v2.x);
}
/* ********************************************************************** */
/*                                                                        */
/*  Calcula a interseccao entre 2 retas (no plano "XY" Z = 0)             */
/*                                                                        */
/* k : ponto inicial da reta 1                                            */
/* l : ponto final da reta 1                                              */
/* m : ponto inicial da reta 2                                            */
/* n : ponto final da reta 2                                              */
/*                                                                        */
/* s: valor do parâmetro no ponto de interseção (sobre a reta KL)         */
/* t: valor do parâmetro no ponto de interseção (sobre a reta MN)         */
/*                                                                        */
/* ********************************************************************** */
int intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t)
{
    double det;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0)
        return 0 ; // não há intersecção

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    return 1; // há intersecção
}

// **********************************************************************
bool HaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n)
{
    int ret;
    double s,t;
    ret = intersec2d( k,  l,  m,  n, s, t);
    if (!ret) return false;
    if (s>=0.0 && s <=1.0 && t>=0.0 && t<=1.0)
        return true;
    else return false;
}


// **********************************************************************
//
// **********************************************************************
void init()
{
    Ponto MinPoly, MaxPoly, Folga;

    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 1.0f, 0.5f);


    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@///
    leCores();
    Min = Ponto (-XMAX, -YMAX);
    Max = Ponto (XMAX, YMAX);

    speed = XMAX*2/timeToTravel;
    bulletSpeed = speed*BSPEED;
    srand(time(0));
    enemies.push_back(spawnEnemy());
    gravity = bulletSpeed/5;
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@///

    P1.LePoligono("Triangulo.txt");
    P2.LePoligono("Retangulo.txt");

    int range = -XMAX/2 + 10;
    for(int i = 0; i < buildingcount; i++)
    {
        buildings.push_back(spawnBuilding(range));
        range+=16;
    }

   // testEnemy = Enemy(XMAX/2,50, 1);






    /*
    P1.obtemLimites(MinPoly, MaxPoly);

    Min = MinPoly;
    Max = MaxPoly;

    Folga = (Max-Min)* 0.1;

    cout << "Folga: "; Folga.imprime();

    Min = Min - Folga;
    Max = Max + Folga;

    cout << endl;
    cout << "Min: "; Min.imprime();
    cout << "Max: "; Max.imprime();

    //Min = ObtemMinimo(Min, MinPoly);
    //Max = ObtemMaximo(Max, MaxPoly);
    */
}

double nFrames=0;
double TempoTotal=0;
// **********************************************************************
//
// **********************************************************************
void animate()
{

    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    BulletTimer += dt;

    nFrames++;

    if (AccumDeltaT > 1.0/REFRESHRATE) // fixa a atualização da tela em 60
    {
        AccumDeltaT = 0;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
        if(enemies.size() < MAXENEMIES)
        {
            if(!stopEnemySpawn)
            {
                enemies.push_back(spawnEnemy());
                spawnedEnemies++;
            }

        }
        if(spawnedEnemies >= MAXENEMIES)
        {
            stopEnemySpawn= true;
        }
    }
    if(BulletTimer > 1.0)
    {
        if(shootCD == true)
            shootCD = false;
        BulletTimer = 0;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);  // Janela de Exibição
    // Define os limites logicos da area OpenGL dentro da Janela

    glOrtho(Min.x,Max.x, Min.y,Max.y, 0,1); //Janela de Seleção
    //glOrtho(0,10, 0,10, 0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// **********************************************************************
//
// **********************************************************************
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES); // vou desenhar LINHAS
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}
// **********************************************************************
//
// **********************************************************************
void RotacionaAoRedorDeUmPonto(float alfa, Ponto P)
{
    glTranslatef(P.x, P.y, P.z);
    glRotatef(alfa, 0,0,1);
    glTranslatef(-P.x, -P.y, -P.z);
}

// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{
    string angleUI;
    string HPUI;
	// Limpa a tela com a cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//////
	glPushMatrix();
        HPUI = "HP: ";
        for(int i = 0; i < HP; i++)
        {
            HPUI += "X";
        }
        angleUI = "Angle: " + to_string(player.getShotAngle());
        printString(angleUI,-XMAX/2-XMAX/2,YMAX*0.75);
        printString("Force: " + to_string(player.getShotPower()),-XMAX/2-XMAX/2,YMAX*0.80 );
        printString(HPUI,-XMAX/2-XMAX/2,YMAX*0.85);

    glPopMatrix();
    /*
    */
    ////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//////
    glPushMatrix();
        glColor3f(0.5,0.5,0.5);
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex3f(-XMAX,-YMAX*0.8,0);
        glVertex3f(XMAX,-YMAX*0.8,0);
        glEnd();
    glPopMatrix();


    glPushMatrix();
        buildingHandler();
    glPopMatrix();
    glPushMatrix();
        playerHandler();
    glPopMatrix();
    glPushMatrix();
        bulletHandler();
    glPopMatrix();
    glPushMatrix();
        enemyHandler();
    glPopMatrix();

	glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo número de segundos e informa quanto frames
// se passaram neste período.
// **********************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 't':
            ContaTempo(3);
            break;
        case 'w':
            player.setShotPower(player.getShotPower()+1);
            if(player.getShotPower() > 15)
                player.setShotPower(15);
            break;
        case 's':
            player.setShotPower(player.getShotPower()-1);;
            break;
        case 'a':
            player.setDirection(-1);
            break;
        case 'd':
            player.setDirection(1);
            break;
        case ' ':
            shoot = true;
        break;
		default:
			break;
	}
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // Se pressionar UP
			glutFullScreen ( ); // Vai para Full Screen
			break;
        case GLUT_KEY_RIGHT:       // Se pressionar SETA DIR
            player.setShotAngle(player.getShotAngle()-1);
            break;
        case GLUT_KEY_LEFT:       // Se pressionar SETA ESQ
            player.setShotAngle(player.getShotAngle()+1);
            break;
	    case GLUT_KEY_DOWN:     // Se pressionar UP
								// Reposiciona a janela
            glutPositionWindow (50,50);
			glutReshapeWindow ( 700, 500 );
			break;
		default:
			break;
	}
//    glutPostRedisplay();
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 1280, 1280);  // Define o SRD

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow    ( "Primeiro Programa em OpenGL" );

    // executa algumas inicializaÁıes
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // ser· chamada automaticamente quando
    // for necess·rio redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalidação da tela. A funcao "display"
    // ser· chamada automaticamente sempre que a
    // máquina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser· chamada automaticamente quando
    // o usu·rio alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser· chamada automaticamente sempre
    // o usu·rio pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser· chamada
    // automaticamente sempre o usu·rio
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
