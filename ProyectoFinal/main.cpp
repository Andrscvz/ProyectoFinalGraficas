//
//  main.cpp
//  game
//
//  Created by Gustavo Ferrufino on 10/21/15.
//  Copyright (c) 2015 Gustavo Ferrufino. All rights reserved.
//
#include <iostream>
#ifdef __APPLE__

#else
#include <windows.h>
#include <GL/glut.h>
#endif
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <string>
#include <sstream>

#include <fstream>

#include "glm.h"
#include "imageloader.h"
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

//INTEGERS
int windowWidth = 800, windowHeight = 800;
int texNumber = -1;

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

GLMmodel model[8];

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
    auxPath = fullPath + "modelos/bird.obj";
    model[0] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[0]);
    glmVertexNormals(&model[0], 90.0, GL_TRUE);

    auxPath = fullPath + "modelos/plasticbottle.obj";
    model[1] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[1]);
    glmVertexNormals(&model[1], 90.0, GL_TRUE);

    auxPath = fullPath + "modelos/plasticflaske.obj";
    model[2] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[2]);
    glmVertexNormals(&model[2], 90.0, GL_TRUE);

    auxPath = fullPath + "modelos/StackofPapers.obj";
    model[3] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[3]);
    glmVertexNormals(&model[3], 90.0, GL_TRUE);

    auxPath = fullPath + "modelos/trash.obj";
    model[4] = *glmReadOBJ(&auxPath[0]);
    glmUnitize(&model[4]);
    glmVertexNormals(&model[4], 90.0, GL_TRUE);
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


void myTimer(int i) {
    if (i ==1){
        if(t > 3){
            t = -1;
        }
        t += delta;

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
    glColor3ub(1, 0, 0);
    glPushMatrix();
    glTranslatef (-2.3, 1.7, 0);
    glRotatef(1, 1.0, 0, 0);
    glPopMatrix();
    glColor3ub(0, 0, 0);
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
        glColor3ub(0, 0, 0);
        if(menuInicial)
            despliegaTexto(jugarTxt,-2,0.4,0.005,0.005);


        //Instrucciones
        glColor3ub(0, 0, 0);
        if(menuInicial)
            despliegaTexto(instruccionesTxt,-2,-1.05,0.0025,0.0025);

        //Salir
        glColor3ub(0, 0, 0);
        if(menuInicial)
            despliegaTexto(salirTxt,-2,-2.7,0.005,0.005);

    //if(menuNivel)
    //  opcionVolver();

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

    // start = false;
    // menuInicial = false;
    // menuNivel = false;
    // instrucciones = false;

    if(start){

        mostrarInicio();

    }else if(menuInicial || menuNivel) {

        mostrarMenu();

    }else if(instrucciones) {

        mostrarInstrucciones();

    } else{

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Pajaro
        glPushMatrix();
            glScalef(.7,.7,.7);
            glTranslatef (-movePajaroX, 1, movePajaroZ);
            glRotatef(0+rotatePajaro*40,0,1,0);
            glmDraw(&model[0], GLM_COLOR);
        glPopMatrix();

        //Plastic Bottle
        glPushMatrix();
            glScalef(1.5,1.5,1.5);
            glTranslatef (1, -t/1.5, 0);
            glRotatef(0+t*100,1,1,0);
            glmDraw(&model[1], GLM_COLOR);
        glPopMatrix();

        //Plastic Flaske
        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (-1, -t/.2, 0);
            glRotatef(0+t*100,1,1,0);
            glmDraw(&model[2], GLM_COLOR);
        glPopMatrix();

        //Stack of paper
        glPushMatrix();
            glScalef(.3,.3,.3);
            glTranslatef (1.5, -t/.3, 0);
            glRotatef(0+t*100,1,1,0);
            glmDraw(&model[3], GLM_COLOR);
        glPopMatrix();

        //Trash can1
        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (-4, -16, 0);
            glmDraw(&model[4], GLM_COLOR);
        glPopMatrix();

        //Trash can2
        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (-2, -16, 0);
            glmDraw(&model[4], GLM_COLOR);
        glPopMatrix();

        //Trash can3
        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (0, -16, 0);
            glmDraw(&model[4], GLM_COLOR);
        glPopMatrix();

        //Trash can4
        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (2, -16, 0);
            glmDraw(&model[4], GLM_COLOR);
        glPopMatrix();

        //Trash can5
        glPushMatrix();
            glScalef(.2,.2,.2);
            glTranslatef (4, -16, 0);
            glmDraw(&model[4], GLM_COLOR);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mouseX, int mouseY)
{
    switch (key)
    {
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

        if(menuInicial || menuNivel) {
            xIzq = windowWidth*100/800; xDer = windowWidth*300/800;
            yUp_1 = windowHeight*90/800; yDown_1 = windowHeight*207/800;
            yUp_2 = windowHeight*332/800; yDown_2 = windowHeight*448/800;
            yUp_3 = windowHeight*574/800; yDown_3 = windowHeight*689/800;
            if(x >= xIzq && x <= xDer) {
                if(y >= yUp_1 && y <= yDown_1) { // JUGAR
                    if(menuInicial) { //
                        //glutTimerFunc(50, myTimer, 1);
                        menuInicial = false;
                        menuNivel = false;
                    }
                    else if(menuNivel) {


                        menuNivel = false;
                    }
                }
                else if(y >= yUp_2 && y <= yDown_2) { //INSTRUCCIONES
                    if(menuInicial) {
                        menuInicial = false;
                        instrucciones = true;
                    }
                    else if(menuNivel) {


                        menuNivel = false;
                    }
                }
                else if(y >= yUp_3 && y <= yDown_3) { // SALIR
                    if(menuInicial) {
                        exit(0);
                    }
                    else if(menuNivel) {


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
    glutMainLoop();
    return 0;
}
