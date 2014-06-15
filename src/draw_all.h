/*
 Author : Vaibhav Malpani
 Uni: vom2102
 Course: Computer Graphics COMS 4160
 Programming Assignment 1 - Pic Breaker
 Columbia University
*/
#ifndef helloWorld_draw_all_h
#define helloWorld_draw_all_h

#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>

GLfloat paddle_wd = 0.15, paddle_ht = 0.1;
GLfloat paddle1_x = 0.0, paddle1_y = 0.0;
GLfloat xTranslated = 0.0, yTranslated = 0.0, zTranslated = -4.75;
GLfloat xRotated = 33.0, yRotated = 40.0, zRotated = 30.0;

GLdouble radius=0.12;
GLuint tiles[16];

// boolean value representing 4*4 = 16 tiles on the back face of cube
bool FLAG_PATTERN[4][4]={{false,false,false,false},{false,false,false,false},{false,false,false,false},{false,false,false,false}};

// model front, top, bottom, left and right surfaces of a cube using quads
// back face is split into multiple tiles and used for texture mapping
// back face defined using display lists for efficient rendering
void cube(void){
    glPushMatrix();
    glTranslatef(0.0,0.0,-4.75);
    glBegin(GL_QUADS);                // Draw The Cube Using quads
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f( 1.0f, 1.0f,-2.0f);    // Top
    glVertex3f(-1.0f, 1.0f,-2.0f);
    glVertex3f(-1.0f, 1.0f, 2.0f);
    glVertex3f( 1.0f, 1.0f, 2.0f);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f( 1.0f,-1.0f, 2.0f);    // Bottom
    glVertex3f(-1.0f,-1.0f, 2.0f);
    glVertex3f(-1.0f,-1.0f,-2.0f);
    glVertex3f( 1.0f,-1.0f,-2.0f);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f( 1.0f, 1.0f, 2.0f);    // Front
    glVertex3f(-1.0f, 1.0f, 2.0f);
    glVertex3f(-1.0f,-1.0f, 2.0f);
    glVertex3f( 1.0f,-1.0f, 2.0f);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(-1.0f, 1.0f, 2.0f);    // Left
    glVertex3f(-1.0f, 1.0f,-2.0f);
    glVertex3f(-1.0f,-1.0f,-2.0f);
    glVertex3f(-1.0f,-1.0f, 2.0f);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f( 1.0f, 1.0f,-2.0f);    // Right
    glVertex3f( 1.0f, 1.0f, 2.0f);
    glVertex3f( 1.0f,-1.0f, 2.0f);    
    glVertex3f( 1.0f,-1.0f,-2.0f);
    glEnd();
    glPopMatrix();
}

void backPattern(void){
    glPushMatrix();
    glColor4f(1.0f,1.0f,1.0f,1.0);
    glTranslatef(0.0,0.0,-4.75);
    glBegin(GL_QUADS);
    // single image split into tiles
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (FLAG_PATTERN[i][j])     glCallList(tiles[(i*4)+j]);
        }
    }
    glEnd();
    glPopMatrix();
}

void track_ball(void){
    // Depict the current z location of sphere to assist the player
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex3f( 1.0f,-1.0f,zTranslated);
    glVertex3f(-1.0f,-1.0f,zTranslated);
    glVertex3f(-1.0f, 1.0f,zTranslated);
    glVertex3f( 1.0f, 1.0f,zTranslated);
    glEnd();
}

void paddles(void){
    glPushMatrix();
    glTranslatef(0.0,0.0,-4.75);
    glColor4ub(255,255,0,0);
    glBegin(GL_QUADS);  // Draw paddle on front face of cube
    glVertex3f( paddle_wd +paddle1_x, paddle_ht+paddle1_y, 2.0f);
    glVertex3f(-paddle_wd+paddle1_x, paddle_ht+paddle1_y, 2.0f);
    glVertex3f(-paddle_wd+paddle1_x,-paddle_ht+paddle1_y, 2.0f);
    glVertex3f( paddle_wd+paddle1_x,-paddle_ht+paddle1_y, 2.0f);
    glEnd();
    glBegin(GL_LINES);  // Draw intersecting perpendicular lines within paddle
    glVertex3f( paddle1_x-paddle_wd,paddle1_y,2.0f);
    glVertex3f( paddle1_x+paddle_wd,paddle1_y,2.0f);
    glVertex3f( paddle1_x,paddle1_y-paddle_ht,2.0f);
    glVertex3f( paddle1_x,paddle1_y+paddle_ht,2.0f);
    glEnd();
    glPopMatrix();
}

void sphere(void){
    glPushMatrix();
    // traslate the draw by z = -4.75
    glTranslatef(xTranslated,yTranslated,zTranslated);
    glColor3f(0.8, 0.2, 0.1);
    // rotation about X axis
    glRotatef(xRotated,1.0,0.0,0.0);
    // rotation about Y axis
    glRotatef(yRotated,0.0,1.0,0.0);
    // rotation about Z axis
    glRotatef(zRotated,0.0,0.0,1.0);
    // scaling transfomation
    glScalef(1.0,1.0,1.0);
    glutSolidSphere(radius,20,20);
    yRotated += 2;
    glPopMatrix();
}
#endif