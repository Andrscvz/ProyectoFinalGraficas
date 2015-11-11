//
//  main.cpp
//  game
//
//  Created by Gustavo Ferrufino on 10/21/15.
//  Copyright (c) 2015 Gustavo Ferrufino. All rights reserved.
//
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <string>
#include <sstream>

#include <fstream>

#include "glm.h"
#include "imageloader.h"
#include "Objeto.h"
using namespace std;

std::ostringstream strStream;
string fullPath = __FILE__;
static GLuint txtName[40];

float t=-1.0;
float delta=0.11;

//Movimientos del pajaro
float movePajaroX = -2;
bool pajaroXtof = true;
float movePajaroZ = 0;
bool pajaroZtof = true;
float rotatePajaro = 0;

//Objetos que van a caer
vector<Objeto *> objetosPlastico;
vector<Objeto *> objetosPapel;
vector<Objeto *> objetosAluminio;
vector<Objeto *> objetosOrganico;


//INTEGERS
int windowWidth = 800, windowHeight = 800;
int texNumber = -1;
int score = 0;


//FLOAT
float velX,velY;
const float medida = 10.0;

//BOOLS
bool gameOver = false;
bool moviendo = false;
bool menuInicial = false;
bool menuNivel = false;
bool instrucciones = false;
bool start = true;
bool playerLeft = false;
bool playerRight = false;
bool juegoIniciado = true;
bool seleccion_1 = false, seleccion_2 = false, seleccion_3 = false, seleccion_v = false;
bool btn = false;


/*** VERIFICAR ITERACION ***/
bool countOncePointAluminium = true;
bool reduceOnceLifeInAlum = true;

bool countOncePointPlastico = true;
bool reduceOnceLifeInPlastico = true;

bool countOncePointPapel = true;
bool reduceOnceLifeInPapel = true;

bool countOncePointOrganico = true;
bool reduceOnceLifeInOrganico = true;

GLMmodel model[11];

float x=1.0;
float medidaArbol = 2.5;
int nivel = 1;
int lives = 3;

string auxPath;

//STRINGS
string instruccionesTxt = "Instrucciones",
jugarTxt = "Jugar",
salirTxt = "Salir",
volverTxt = "<-", beginTxt = "Presiona Enter";


//Makes the image into a texture, and returns the id of the texture
void loadTexture(Image* image,int k){

    glBindTexture(GL_TEXTURE_2D, txtName[k]); //Tell OpenGL which texture to edit

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
                 image->pixels);               //The actual pixel data

}

void initModels(){

    //Pajaro
    auxPath = fullPath + "modelos/pajaro.obj";
    model[0] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[0]);
    glmVertexNormals(&model[0], 90.0, GL_TRUE);

    /********************Plastico****************************/
    auxPath = fullPath + "modelos/plastico.obj";
    model[1] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[1]);
    glmVertexNormals(&model[1], 90.0, GL_TRUE);

    /********************Papel****************************/
    auxPath = fullPath + "modelos/StackofPapers.obj";
    model[2] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[2]);
    glmVertexNormals(&model[2], 90.0, GL_TRUE);

    /********************Aluminio****************************/
    auxPath = fullPath + "modelos/lata.obj";
    model[3] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[3]);
    glmVertexNormals(&model[3], 90.0, GL_TRUE);

    /***********************Organico*************************/
    auxPath = fullPath + "modelos/manzana.obj";
    model[4] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[4]);
    glmVertexNormals(&model[4], 90.0, GL_TRUE);

    /***********************Botes de basura*************************/
    auxPath = fullPath + "modelos/trashPlastico.obj";
    model[5] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[5]);
    glmVertexNormals(&model[5], 90.0, GL_TRUE);

    auxPath = fullPath + "modelos/trashPapel.obj";
    model[6] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[6]);
    glmVertexNormals(&model[6], 90.0, GL_TRUE);

    auxPath = fullPath + "modelos/trashAluminio.obj";
    model[7] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[7]);
    glmVertexNormals(&model[7], 90.0, GL_TRUE);

    auxPath = fullPath + "modelos/trashOrganico.obj";
    model[8] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[8]);
    glmVertexNormals(&model[8], 90.0, GL_TRUE);

    /***********************Nubes*************************/
    auxPath = fullPath + "modelos/nubes.obj";
    model[9] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[9]);
    glmVertexNormals(&model[9], 90.0, GL_TRUE);

    /***********************Vidas*************************/
    auxPath = fullPath + "modelos/vidas.obj";
    model[10] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[10]);
    glmVertexNormals(&model[9], 90.0, GL_TRUE);

}

void loadImage(string nombreImagen, int numImagen){
    Image* image;
    string ruta = fullPath + "./imagenes/" + nombreImagen;
    image = loadBMP(ruta.c_str());
    loadTexture(image,numImagen);
    delete image;
}

void despliegaTexto(string texto, float x, float y, float sizeX, float sizeY) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(sizeX, sizeY, 1.0);

    for (int k=0;k<texto.length(); k++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN,texto[k]);

    glPopMatrix();
}

void reshape (int w, int h){
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
    glTranslatef(0.0f, 1.0f, -6.0f);
}

void iniciaNivel(){
    Objeto * obj;

    objetosPlastico.clear();
    objetosOrganico.clear();
    objetosAluminio.clear();
    objetosPapel.clear();

    int random;

    if(nivel == 1 ){
        delta = 0.07;
    }else if( nivel == 2){
        delta = 0.1;
    }else if( nivel == 3){
        delta = 0.12;
    }else{
        delta = 0.15;
    }

    //Plastico
    for(int i = 0; i < nivel; i++){
        random = (rand()%100)/10.0;
        obj = new Objeto(.2,1,-1-random);
        objetosPlastico.push_back(obj);
    }

    //Papel
    for(int i = 0; i < nivel; i++){
        random = (rand()%100)/10.0;
        obj = new Objeto(.2,2,-1-random);
        objetosPapel.push_back(obj);
    }

    //Aluminio
    for(int i = 0; i < nivel; i++){
        obj = new Objeto(.15,3,-1-random);
        objetosAluminio.push_back(obj);
    }

    //Organico
    for(int i = 0; i < nivel; i++){
        random = (rand()%100)/10.0;
        obj = new Objeto(.15,4,-1-random);
        objetosOrganico.push_back(obj);
    }
}


void myTimer(int i) {
    if (i ==1){
        //Rotacion del mundo
        x=x+1;

        /*********************Objetos que caen****************************/
        int random;
        //Para que cada objeto se mueva Plastico
        reduceOnceLifeInPlastico = true;
        for(int i = 0; i < objetosPlastico.size(); i++){
            random = (rand()%100)/10.0;
            if(objetosPlastico[i]->getTranslate() > 3.5){
                if(!objetosPlastico[i]->valid && reduceOnceLifeInPlastico){
                    lives-=1;
                    reduceOnceLifeInPlastico = false;
                }
                objetosPlastico[i]->setTranslate(-1 - random);
                objetosPlastico[i]->valid = false;


            }
            if(objetosPlastico[i]->reset){
                countOncePointPlastico = true;
                objetosPlastico[i]->setTranslate(-1 - random);
                objetosPlastico[i]->reset = false;
            }

            objetosPlastico[i]->setTranslate(objetosPlastico[i]->getTranslate() + delta);
        }

        //Para que cada objeto se mueva Papel
        reduceOnceLifeInPapel = true;
        for(int i = 0; i < objetosPapel.size(); i++){
             random = (rand()%100)/10.0;
            if(objetosPapel[i]->getTranslate() > 3.5){
                if(!objetosPapel[i]->valid && reduceOnceLifeInPapel){
                    lives-=1;
                    reduceOnceLifeInPapel = false;
                }
                objetosPapel[i]->setTranslate(-1 - random);
                objetosPapel[i]->valid = false;


            }
            if(objetosPapel[i]->reset){
                countOncePointPapel = true;
                objetosPapel[i]->setTranslate(-1 - random);
                objetosPapel[i]->reset = false;
            }

            objetosPapel[i]->setTranslate(objetosPapel[i]->getTranslate() + delta);
        }

        //Para que cada objeto se mueva Aluminio
         reduceOnceLifeInAlum = true;
        for(int i = 0; i < objetosAluminio.size(); i++){
            random = (rand()%100)/10.0;
            if(objetosAluminio[i]->getTranslate() > 3.5){
                if(!objetosAluminio[i]->valid && reduceOnceLifeInAlum){
                    lives-=1;
                    reduceOnceLifeInAlum = false;
                }
                objetosAluminio[i]->setTranslate(-1 - random);
                objetosAluminio[i]->valid = false;


            }
            if(objetosAluminio[i]->reset){
                countOncePointAluminium = true;
                objetosAluminio[i]->setTranslate(-1 - random);
                objetosAluminio[i]->reset = false;
            }

            objetosAluminio[i]->setTranslate(objetosAluminio[i]->getTranslate() + delta);
        }

        //Para que cada objeto se mueva Organico
        reduceOnceLifeInOrganico = true;
        for(int i = 0; i < objetosOrganico.size(); i++){
             random = (rand()%100)/10.0;
            if(objetosOrganico[i]->getTranslate() > 3.5){
                if(!objetosOrganico[i]->valid && reduceOnceLifeInOrganico){
                    lives-=1;
                    reduceOnceLifeInOrganico = false;
                }
                objetosOrganico[i]->setTranslate(-1 - random);
                objetosOrganico[i]->valid = false;


            }
            if(objetosOrganico[i]->reset){
                countOncePointOrganico = true;
                objetosOrganico[i]->setTranslate(-1 - random);
                objetosOrganico[i]->reset = false;
            }

            objetosOrganico[i]->setTranslate(objetosOrganico[i]->getTranslate() + delta);
        }

        rotatePajaro += 0.11;

        //Pajaro moviendose en x
        if(movePajaroX < -2)
            pajaroXtof = true;

        if(movePajaroX < 2 && pajaroXtof){
            movePajaroX += 0.11;
        }else{
            pajaroXtof = false;
            movePajaroX -= 0.11;
        }


        //Pajaro moviendose en y
        if(movePajaroZ < -2)
            pajaroZtof = true;

        if(movePajaroZ < 2 && pajaroZtof){
            movePajaroZ += 0.11;
        }else{
            pajaroZtof = false;
            movePajaroZ -= 0.11;
        }

        glutPostRedisplay();
        glutTimerFunc(100,myTimer,1);
    }
}


void init(void){
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}

void initRendering(){
    GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glGenTextures(2, txtName); //Make room for our texture

    loadImage("willy.bmp",++texNumber);
}

void opcionVolver(){
    //Boton Back
    if(seleccion_v) glColor3ub(139, 0, 139);
    else glColor3ub(0, 0, 0);
    glPushMatrix();
    glTranslatef (-2.3, 1.7, 0);
    glRotatef(1, 1.0, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f( -0.7, -0.3, -2.0 );
    glVertex3f(  0.7, -0.3, -2.0 );
    glVertex3f(  0.7,  0.3, -2.0 );
    glVertex3f( -0.7,  0.3, -2.0 );
    glEnd();
    glPopMatrix();
    glColor3ub(255, 255, 255);
    despliegaTexto(volverTxt,-2.15,0.95,0.0025,0.0025);
}

void mostrarInicio(){
    glPushMatrix();
        glColor3ub(1, 0, 0);
        despliegaTexto(beginTxt,-2,0.4,0.0025,0.0025);
    glPopMatrix();
}

void mostrarMenu(){
    glPushMatrix();

        //Iniciar juego
        if(seleccion_1) glColor3ub(139, 0, 139);
        else glColor3ub(0, 0, 0);

        glPushMatrix();
        glTranslatef (-2, 1.0, 0);
        glRotatef(2, 1.0, 0, 0);
        glBegin(GL_QUADS);
        glVertex3f( -1.5, -0.5, -2.0 );
        glVertex3f(  1.5, -0.5, -2.0 );
        glVertex3f(  1.5,  0.5, -2.0 );
        glVertex3f( -1.5,  0.5, -2.0 );
        glEnd();
        glPopMatrix();
        glColor3ub(255, 255, 255);

        if(menuInicial)
            despliegaTexto(jugarTxt,-2,0.4,0.005,0.005);
        else if(menuNivel)
            despliegaTexto("Nivel 1",-2,0.4,0.0025,0.0025);


        //Instrucciones
        if(seleccion_2) glColor3ub(139, 0, 139);
        else glColor3ub(0, 0, 0);

        glPushMatrix();
        glTranslatef (-2, -1.0, 0);
        glRotatef(2, 1.0, 0, 0);
        glBegin(GL_QUADS);
        glVertex3f( -1.5, -0.5, -2.0 );
        glVertex3f(  1.6, -0.5, -2.0 );
        glVertex3f(  1.6,  0.5, -2.0 );
        glVertex3f( -1.5,  0.5, -2.0 );
        glEnd();
        glPopMatrix();
        glColor3ub(255, 255, 255);

        if(menuInicial)
            despliegaTexto(instruccionesTxt,-2.2,-1.05,0.0025,0.0025);
        else if(menuNivel)
            despliegaTexto("Nivel 2",-2,-1.05,0.0025,0.0025);


        //Salir

        if(seleccion_3) glColor3ub(139, 0, 139);
        else glColor3ub(0, 0, 0);
        glPushMatrix();
        glTranslatef (-2, -3.0, 0);
        glRotatef(2, 1.0, 0, 0);
        glBegin(GL_QUADS);
        glVertex3f( -1.5, -0.5, -2.0 );
        glVertex3f(  1.5, -0.5, -2.0 );
        glVertex3f(  1.5,  0.5, -2.0 );
        glVertex3f( -1.5,  0.5, -2.0 );
        glEnd();
        glPopMatrix();
        glColor3ub(255, 255, 255);
        if(menuInicial)
            despliegaTexto(salirTxt,-2,-2.7,0.005,0.005);
        else if(menuNivel)
            despliegaTexto("Nivel 3",-2,-2.7,0.0025,0.0025);
    if(menuNivel)
      opcionVolver();

    glPopMatrix();
}



void mostrarInstrucciones(){
    glPushMatrix();
        opcionVolver();
    glPopMatrix();
}

void display(){

    glClearColor(0.0, 0.75, 0.75,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

        glTranslatef (0, 0.0, -.5);
        //Game Stats

        glBegin(GL_QUADS);
        //Derecha (verde)
        glColor3f(0.0, 1.0, 0.1);
        glVertex3f( medida, medida, -60 );
        glVertex3f( -medida,  -medida, -60 );
        glVertex3f( medida,  -medida,  60 );
        glVertex3f( medida, medida,  60 );

        //Izquierda (azul)
        glColor3f(0.1, 0.0, 1.0);
        glVertex3f( -medida, -medida, -60 );
        glVertex3f( medida, -medida,  60 );
        glVertex3f( -medida,  medida,  60 );
        glVertex3f( medida,  medida, -60 );

        glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, txtName[0]);



    if(start){

        mostrarInicio();

    }else if(menuInicial || menuNivel) {

        mostrarMenu();

    }else if(instrucciones) {

        mostrarInstrucciones();

    } else{

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(juegoIniciado){
             iniciaNivel();
            juegoIniciado = !juegoIniciado;
        }

        /***********Pajaro***************/
        glPushMatrix();
            glScalef(.7,.7,.7);
            glTranslatef (-movePajaroX, 1, movePajaroZ);
            glRotatef(0-rotatePajaro*40,0,1,0);
            glmDraw(&model[0], GLM_COLOR);
        glPopMatrix();
        /********************************/

        /***********Nubes***************/
        glPushMatrix();
            glScalef(1,1,1);
            glTranslatef (2, 0, -1);
            glmDraw(&model[9], GLM_COLOR);
        glPopMatrix();

        glPushMatrix();
            glScalef(2,2,2);
            glTranslatef (-.5, 1, -1);
            glmDraw(&model[9], GLM_COLOR);
        glPopMatrix();
        /********************************/

        /***********Sol***************/
        glPushMatrix();
            glColor3ub(245, 241, 10);
            glTranslatef (-2, 2, -3);
            glutSolidSphere(0.8,40,40);
        glPopMatrix();
        /********************************/

        double scale;

        /***********Plastico***************/
        for(int i = 0; i < objetosPlastico.size(); i++){
            scale = objetosPlastico[i]->getScale();
            glPushMatrix();
                glScalef(scale,scale,scale);
                glTranslatef (-3, -objetosPlastico[i]->getTranslate()/scale, 0);
                glRotatef(0+objetosPlastico[i]->getTranslate()*100,1,1,0);
                glmDraw(&model[objetosPlastico[i]->getModel()], GLM_COLOR);
            glPopMatrix();
        }
        /********************************/

        /***********Papel***************/
        for(int i = 0; i < objetosPapel.size(); i++){
            scale = objetosPapel[i]->getScale();
            glPushMatrix();
                glScalef(scale,scale,scale);
                glTranslatef (-1, -objetosPapel[i]->getTranslate()/scale, 0);
                glRotatef(0+objetosPapel[i]->getTranslate()*100,1,1,0);
                glmDraw(&model[objetosPapel[i]->getModel()], GLM_COLOR);
            glPopMatrix();
        }
        /********************************/

        /***********Aluminio***************/
        for(int i = 0; i < objetosAluminio.size(); i++){
            scale = objetosAluminio[i]->getScale();
            glPushMatrix();
                glScalef(scale,scale,scale);
                glTranslatef (1.5, -objetosAluminio[i]->getTranslate()/scale, 0);
                glRotatef(0+objetosAluminio[i]->getTranslate()*100,1,1,0);
                glmDraw(&model[objetosAluminio[i]->getModel()], GLM_COLOR);
            glPopMatrix();
        }
        /********************************/

        /***********Organico***************/
        for(int i = 0; i < objetosOrganico.size(); i++){
            scale = objetosOrganico[i]->getScale();
            glPushMatrix();
                glScalef(scale,scale,scale);
                glTranslatef (4, -objetosOrganico[i]->getTranslate()/.15, 0);
                glRotatef(0+objetosOrganico[i]->getTranslate()*100,1,1,0);
                glmDraw(&model[objetosOrganico[i]->getModel()], GLM_COLOR);
            glPopMatrix();
        }
        /********************************/

        /***********Basureros***************/
        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (-3, -16.5, -1);
            glmDraw(&model[5], GLM_COLOR);
        glPopMatrix();

        despliegaTexto("Plastico",-.75,-3.2,.00075,.00075);

        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (-1, -16.5, -1);
            glmDraw(&model[6], GLM_COLOR);
        glPopMatrix();

        despliegaTexto("Papel",-.3,-3.2,.00075,.00075);

        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (1, -16.5, -1);
            glmDraw(&model[7], GLM_COLOR);
        glPopMatrix();

        despliegaTexto("Aluminio",0,-3.2,.0007,.0007);

        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (3, -16.5, -1);
            glmDraw(&model[8], GLM_COLOR);
        glPopMatrix();

        despliegaTexto("Organico",.4,-3.2,.0007,.0007);

        /********************************/

        /***********Lineas que sigue la basura***************/
        glBegin(GL_LINES);
            glVertex3f(-.8,1,-2.0);
            glVertex3f(-.8,-4,-2.0);

            glVertex3f(-.3,1,-2.0);
            glVertex3f(-.3,-4,-2.0);

            glVertex3f(.3,1,-2.0);
            glVertex3f(.3,-4,-2.0);

            glVertex3f(.8,1,-2.0);
            glVertex3f(.8,-4,-2.0);
        glEnd();
        /********************************/


        //Para crear los troncos de los arboles
        GLUquadricObj *qobj;
        qobj = gluNewQuadric();
        GLuint startList = glGenLists(1);
        gluQuadricDrawStyle(qobj, GLU_FILL);
            gluQuadricNormals(qobj, GLU_SMOOTH);
            glNewList(startList, GL_COMPILE);
            gluCylinder(qobj,0.6,0.6,2,10,10);
        glEndList();

        /***************Mundo*****************/
        glPushMatrix();
            glTranslatef(0,-7,-8);
            glPushMatrix();

                glRotatef(x, 1.0, 0.0, 0.0);

                /****************Arboles****************/
                glPushMatrix();
                    glTranslatef(medidaArbol,medidaArbol,medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(330, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(-medidaArbol,medidaArbol,medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(330, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(-medidaArbol,-medidaArbol, medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(30, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(-medidaArbol, -medidaArbol,-medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(140, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(medidaArbol,medidaArbol,-medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(220, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(-medidaArbol,medidaArbol,-medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(220, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(medidaArbol, -medidaArbol,-medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(140, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(medidaArbol,-medidaArbol,medidaArbol);
                    glColor3ub(102, 51, 0);
                    glRotatef(30, 1.0, 0.0, 0.0);
                    glCallList(startList);
                    glPushMatrix();
                        glTranslatef(0,0,2);
                        glRotatef(350, 1.0, 0.0, 0.0);
                        glColor3ub(13, 129, 25);
                        glutSolidCone(1,1,10,10);
                    glPopMatrix();
                glPopMatrix();
                /****************Fin de arboles****************/
            glPopMatrix();

            glColor3ub(13, 145, 25);
            glutSolidSphere(5,40,40);

        glPopMatrix();
        /***************Fin de mundo*****************/


        /***********Seccion de vidas y puntaje***************/
        glPushMatrix();
            glColor3ub(199, 77, 36);
            glTranslatef (.3, -4.7, -1);

            glBegin(GL_QUADS);
                glVertex2f(1.0f, 1.0f);
                glVertex2f(2.5f, 1.0f);
                glVertex2f(2.5f, 2.0f);
                glVertex2f(1.0f, 2.0f);
            glEnd();

            glColor3ub(0, 0, 0);

            glBegin(GL_LINE_STRIP);
                glVertex2f(1.0f, 1.0f);
                glVertex2f(2.5f, 1.0f);
                glVertex2f(2.5f, 2.0f);
                glVertex2f(1.0f, 2.0f);
            glEnd();
        glPopMatrix();

        /***********Vidas***************/

        despliegaTexto("Nivel "+to_string(nivel),1.5,-2.7,.001,.001);
        despliegaTexto("Puntos: "+to_string(score),1.2,-2.95,.001,.001);
        //despliegaTexto("100/1000",1.65,-2.95,.001,.001);
        despliegaTexto("Vidas:",1.2,-3.2,.001,.001);

        if(lives == 3){
            //vida 3
            glPushMatrix();
                glTranslatef (1.75, -3.15, 0);
                glScalef(.07,.07,.07);
                glRotatef(0+rotatePajaro*100,1,1,0);
                glmDraw(&model[10], GLM_COLOR);
            glPopMatrix();
                //vida 2
                glPushMatrix();
                    glTranslatef (2, -3.15, 0);
                    glScalef(.07,.07,.07);
                    glRotatef(0+rotatePajaro*100,1,1,0);
                    glmDraw(&model[10], GLM_COLOR);
                glPopMatrix();
                 //vida 1
                glPushMatrix();
                    glTranslatef (2.25, -3.15, 0);
                    glScalef(.07,.07,.07);
                    glRotatef(0+rotatePajaro*100,1,1,0);
                    glmDraw(&model[10], GLM_COLOR);
                glPopMatrix();
                /********************************/
        }
        if(lives == 2){
                //vida 2
                glPushMatrix();
                    glTranslatef (2, -3.15, 0);
                    glScalef(.07,.07,.07);
                    glRotatef(0+rotatePajaro*100,1,1,0);
                    glmDraw(&model[10], GLM_COLOR);
                glPopMatrix();
                 //vida 1
                glPushMatrix();
                    glTranslatef (2.25, -3.15, 0);
                    glScalef(.07,.07,.07);
                    glRotatef(0+rotatePajaro*100,1,1,0);
                    glmDraw(&model[10], GLM_COLOR);
                glPopMatrix();
                /********************************/
            }
        if(lives == 1){
                 //vida 1
                glPushMatrix();
                    glTranslatef (2.25, -3.15, 0);
                    glScalef(.07,.07,.07);
                    glRotatef(0+rotatePajaro*100,1,1,0);
                    glmDraw(&model[10], GLM_COLOR);
                glPopMatrix();
                /********************************/
            }

    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mouseX, int mouseY)
{
    switch (key)
    {
        //Plastico papel alum organico
        case 'a':
             //Para que cada objeto se mueva Aluminio
            for(int i = 0; i < objetosPlastico.size(); i++){
                if(objetosPlastico[i]->getTranslate() >= 2.5 && objetosPlastico[i]->getTranslate() <= 3){
                     if(countOncePointPlastico){
                         score += 10;
                         countOncePointPlastico = false;

                     }
                   objetosPlastico[i]->reset = true;
                }

            }
            break;
        case 's':
             //Para que cada objeto se mueva Aluminio
            for(int i = 0; i < objetosPapel.size(); i++){
                if(objetosPapel[i]->getTranslate() >= 2.5 && objetosPapel[i]->getTranslate() <= 3){
                     if(countOncePointPapel){
                         score += 10;
                         countOncePointPapel = false;

                     }
                   objetosPapel[i]->reset = true;
                }

            }
            break;
        case 'k':
             //Para que cada objeto se mueva Aluminio
            for(int i = 0; i < objetosAluminio.size(); i++){
                if(objetosAluminio[i]->getTranslate() >= 2.5 && objetosAluminio[i]->getTranslate() <= 3){
                     if(countOncePointAluminium){
                         score += 10;
                         countOncePointAluminium = false;

                     }
                   objetosAluminio[i]->reset = true;
                }

            }
            break;
        case 'l':
             //Para que cada objeto se mueva Aluminio
            for(int i = 0; i < objetosOrganico.size(); i++){
                if(objetosOrganico[i]->getTranslate() >= 2.5 && objetosOrganico[i]->getTranslate() <= 3){
                     if(countOncePointOrganico){
                         score += 10;
                         countOncePointOrganico = false;

                     }
                   objetosOrganico[i]->reset = true;
                }

            }
            break;
        case 27:
            exit(0);
            break;

        case 't':
            if (!moviendo && !menuInicial && !menuNivel && !instrucciones) {
                glutTimerFunc(50, myTimer, 1);
            }
            break;

        case 13:
            if(start){
                start=false;
                menuInicial=true;
                display();
            }
            //juegoIniciado = true;   //Para reiniciar o iniciar el juego
            break;

        default:
            break;
    }
}
void myMouse(int button, int state, int x, int y)
{
    int xIzq, xDer, yUp_1, yDown_1, yUp_2, yDown_2, yUp_3, yDown_3;
    int yUp_volver = windowHeight*33/800, yDown_volver = windowHeight*103/800;
    int xIzq_volver = windowWidth*39/800, xDer_volver = windowWidth*205/800;
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        seleccion_1 = false; seleccion_2 = false; seleccion_2 = false; seleccion_v = false;
        if(menuInicial || menuNivel) {
            xIzq = windowWidth*100/800; xDer = windowWidth*300/800;
            yUp_1 = windowHeight*90/800; yDown_1 = windowHeight*207/800;
            yUp_2 = windowHeight*332/800; yDown_2 = windowHeight*448/800;
            yUp_3 = windowHeight*574/800; yDown_3 = windowHeight*689/800;
            if(x >= xIzq && x <= xDer) {
                if(y >= yUp_1 && y <= yDown_1) { // JUGAR //Nivel 1
                    if(menuInicial) { //
                        //glutTimerFunc(50, myTimer, 1);
                        menuInicial = false;
                        menuNivel = true;
                    }
                    else if(menuNivel) {
                        nivel = 1;

                        menuNivel = false;
                    }
                }
                else if(y >= yUp_2 && y <= yDown_2) { //INSTRUCCIONES //Nivel 2
                    if(menuInicial) {
                        menuInicial = false;
                        instrucciones = true;
                    }
                    else if(menuNivel) {
                        nivel = 2;

                        menuNivel = false;
                    }
                }
                else if(y >= yUp_3 && y <= yDown_3) { // SALIR //Nivel 3
                    if(menuInicial) {
                        exit(0);
                    }
                    else if(menuNivel) {

                        nivel = 3;
                        menuNivel = false;
                    }
                }
            }
            else if(x >= xIzq_volver && x <= xDer_volver && menuNivel) {
                if(y >= yUp_volver && y <= yDown_volver) {
                    menuNivel = false;
                    menuInicial = true;
                }
            }
        }
        else if(instrucciones) {
            if(x >= xIzq_volver && x <= xDer_volver && y >= yUp_volver && y <= yDown_volver) {
                instrucciones = false;
                menuInicial = true;
            }
        }
        glutPostRedisplay();
    }
}
void passive(int x, int y)
{
    int xIzq, xDer, yUp_1, yDown_1, yUp_2, yDown_2, yUp_3, yDown_3;
    int yUp_volver = windowHeight*33/800, yDown_volver = windowHeight*103/800;
    int xIzq_volver = windowWidth*39/800, xDer_volver = windowWidth*205/800;
    if(menuInicial || menuNivel) {
            xIzq = windowWidth*100/800; xDer = windowWidth*300/800;
            yUp_1 = windowHeight*90/800; yDown_1 = windowHeight*207/800;
            yUp_2 = windowHeight*332/800; yDown_2 = windowHeight*448/800;
            yUp_3 = windowHeight*574/800; yDown_3 = windowHeight*689/800;
        if(x >= xIzq && x <= xDer) {
            if(y >= yUp_1 && y <= yDown_1) {
                seleccion_1 = true;
            }
            else if(y >= yUp_2 && y <= yDown_2) {
                seleccion_2 = true;
            }
            else if(y >= yUp_3 && y <= yDown_3) {
                seleccion_3 = true;
            }
            else {
                seleccion_1 = false;
                seleccion_2 = false;
                seleccion_3 = false;
            }
        }
        else if(x >= xIzq_volver && x <= xDer_volver && menuNivel) {
            if(y >= yUp_volver && y <= yDown_volver) {
                seleccion_v = true;
            }
            else {
                seleccion_v = false;
            }
        }
        else {
            seleccion_v = false;
            seleccion_1 = false;
            seleccion_2 = false;
            seleccion_3 = false;
        }
    }
    else if(instrucciones) {
        if(x >= xIzq_volver && x <= xDer_volver && y >= yUp_volver && y <= yDown_volver) {
            seleccion_v = true;
        }
        else {
            seleccion_v = false;
        }
    }
    else {
        seleccion_v = false;
        seleccion_1 = false;
        seleccion_2 = false;
        seleccion_3 = false;
    }
    glutPostRedisplay();
}
void specialKeys (int key, int x, int y){
    if(!menuInicial && !menuNivel && !instrucciones) {
        switch (key) {

            case 'a':
            case 'A':
                playerLeft = false;
                break;

            case 'd':
            case 'D':
                playerRight = false;
                break;

            case 's':
            case 'S':
                t = 0.4;
                break;


            default:
                break;
        }
    }
}

void getParentPath(){

    for (int i = fullPath.length()-1; i>=0; i--) {
        if(fullPath[i] ==  '\\'){
            fullPath[i]= '/';
        }
    }

    for (int i = fullPath.length()-1; i>=0 && fullPath[i] != '/'; i--) {
        fullPath.erase(i,1);
    }
}

int main(int argc, char** argv)
{
    getParentPath();
    glutInit(&argc, argv);
    glutInitWindowSize (windowWidth,windowHeight);
    glutInitWindowPosition (0,0);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH);
    glutCreateWindow("Help Out Willy!");
    init();
    initModels();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(50, myTimer, 1);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(myMouse);
    glutPassiveMotionFunc(passive);
    glutMainLoop();
    return 0;
}
