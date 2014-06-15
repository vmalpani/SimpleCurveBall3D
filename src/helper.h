/*
 Author : Vaibhav Malpani
 Uni: vom2102
 Course: Computer Graphics COMS 4160
 Programming Assignment 1 - Pic Breaker
 Columbia University
 */
#ifndef helper_h
#define helper_h

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

// Lights & Materials
GLfloat camRotX, camRotY, camPosZ;
GLfloat ambient[] = {0.5, 0.5, 0.5, 0.3};
GLfloat position[] = {0.0, 0.0, -2.75, 0.0};
GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0};
GLfloat mat_specular[] = {0.6, 0.6, 0.6, 0.5};
GLfloat mat_shininess[] = {60.0};

GLfloat ambient1[] = {0.6, 0.6, 0.6, 0.5};
GLfloat position1[] = {-1.15, 0.45, 2.0, 2.0};
GLfloat mat_diffuse1[] = {0.5, 0.5, 0.5, 1.0};
GLfloat mat_specular1[] = {0.5, 0.5, 0.5, 0.5};
GLfloat mat_shininess1[] = {50.0};

// initialize lights following starter code provided
void initLights(void)
{
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess1);
}

void setupRC()
{
    // Place Camera
    camRotX = 35.0f;
    camRotY = 68.0f;
    camPosZ = 0.0f;
    
    glEnable( GL_DEPTH_TEST );
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();
}

void setCamera( void )
{
    glTranslatef(0, 0, camPosZ);
    glRotatef(camRotX, 1, 0, 0);
    glRotatef(camRotY, 0, 1, 0);
}

// helper function called to render text on openGL window
// reference: OpenGL - A Primer by E. Angel
void Sprint( GLfloat x, GLfloat y, GLfloat z, char *st, bool style)
{
    int l,i;
    l=strlen( st );
    glRasterPos3f( x, y, z);
    for( i=0; i < l; i++)
    {
        if (style)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
        else
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]); // Print a character on the screen
    }
}

// helper function to load bmp files as textures
// reference: swiftless openGL tutorial
GLuint LoadTexture( const char * filename, int width, int height )
{
    GLuint texture;
    unsigned char * data;
    FILE * file = fopen(filename, "r");
    
    if ( file == NULL ) {
        std::cout<<"Katta!"<<std::endl;
        return 0;
    }
    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    fclose( file );
    
    glGenTextures( 1, &texture ); //generate the texture with
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE ); //set texture environment parameters
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    //repeat texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    
    //generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_BGR, GL_UNSIGNED_BYTE, data);
    free(data);
    return texture;
}
#endif
