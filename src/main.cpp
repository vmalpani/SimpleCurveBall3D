/*
 Author : Vaibhav Malpani
 Uni: vom2102
 Course: Computer Graphics COMS 4160
 Programming Assignment 1 - Pic Breaker
 Columbia University
 */
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

#include "helper.h"
#include "draw_all.h"
#include "GLScreenCapturer.h"

using namespace std;

GLfloat velX = 0.0, velY = 0.0, velZ = 0.0;
GLfloat paddle1_velx = 0.0, paddle1_vely = 0.0;

bool fullScreen = false, textStyle = false;
bool keys_paddle1[4] = {false,false,false,false};
bool FLAG_NEW_GAME = false, FLAG_DIR = false, FLAG_REVEAL = false;

GLuint brick, box, tunnel, pattern;
GLint ball_dir = 1, vscale = 2;
GLint score = 0, FPS = 60;
GLint windowWidth  = 640*1.5;
GLint windowHeight = 480*1.5;
GLint windowPosX   = 50;
GLint windowPosY   = 50;
GLint dir[8][3] = {{1,1,1},{-1,1,1},{1,-1,1},{1,1,-1},{-1,-1,1},{1,-1,-1},{-1,1,-1},{-1,-1,-1}};

char temp[25], last_scr_val[25];
char game[50] = "PIC BREAKER";
char scr[25] = "SCORE ";
char name[25] = "SQUASH";
char inst[25] = "CLICK BELOW";
char play[25] = "Play";
char quit[25] = "Quit";
char reveal[25] = "Reveal";
char saveReveal[25] = "Reveal";
char hide[25] = "Hide?";
char last_score[50] = "LAST SCORE ";
char restart[50]= " " ;
char restart_txt[50]="New Game? Click Play!";

static GLScreenCapturer screenshot("screenshot-%d.ppm");

#define brickImageWidth 256
#define brickImageHeight 128

/*
spawn sphere in random direction in a new game
*/
void sphere_init(int idx){
    if (FLAG_NEW_GAME)
    {
        velZ = -1*abs(dir[idx][0]*0.015);
        velY = dir[idx][1]*0.0075;
        velX = dir[idx][2]*0.01;
    }
    else
    {
        velZ = 0.0;
        velY = 0.0;
        velX = 0.0;
    }
}

/*
 Initialize sphere and paddle for a new game.
 Reset all tiles to hide the image.
 */
void new_game(void){
    //initialize ball
    xTranslated = 0.0, yTranslated = 0.0, zTranslated = -4.75;
    
    //initialize paddle
    paddle1_x = 0.0, paddle1_y = 0.0;
    paddle1_velx = 0.0, paddle1_vely = 0.0;
    sphere_init(rand() % 8);
    FLAG_NEW_GAME = false;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            FLAG_PATTERN[i][j]=false;
        }
    }
}

void redisplayFunc(void)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // clear the identity matrix.
    glLoadIdentity();
    
    // instancing box, tunnel and pattern display lists for efficient rendering
    glCallList(box);
    
    glPushMatrix();
    glColor3f(0.0f,1.0f,0.0f);
    glTranslatef(0.0,0.0,-2.75);
    for(int i=0; i<7; i++){
        glTranslatef(0.0,0.0,-0.5);
        glCallList(tunnel);
    }
    glPopMatrix();
    
    glPushMatrix();
        setCamera();
    glPopMatrix();
    
    // draw ball marker, sphere and paddle
    track_ball();
    sphere();
    paddles();
    
    // load texture to be mapped on the tiles
    glPolygonMode( GL_FRONT, GL_FILL );
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    brick = LoadTexture( "../src/kat.bmp", brickImageWidth, brickImageHeight);
    glBindTexture(GL_TEXTURE_2D, brick);
    backPattern();
    glPushMatrix();
    glColor3f(1.0f,1.0f,1.0f);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            glCallList(pattern);
            glTranslatef(0.5,0.0,0.0);
        }
        glTranslatef(-2.0,-0.5,0.0);
    }
    glPopMatrix();
    glDisable(GL_BLEND);
    
    if (FLAG_NEW_GAME) new_game();
    
    // rendering text to the screen
    glColor3f(1.0f,1.0f,1.0f);
    Sprint(-0.28, 0.92, -2.75, restart, textStyle);
    
    glColor3f(1.0f,1.0f,0.0f);
    Sprint(1.25, 0.65, -2.75, temp, textStyle);
    Sprint(1.25, 0.35, -2.75, last_scr_val, textStyle);
    Sprint(-1.36, 0.45, -2.75, reveal, textStyle);
    
    glDisable(GL_TEXTURE_2D);
    // swap buffers for double buffering
    glutSwapBuffers();
    glFlush();
}

void Timer(int value) {
    //restricting paddle movement on the front plane of the cube
    if (paddle1_x + paddle_wd + paddle1_velx < 1 && paddle1_x - paddle_wd + paddle1_velx > -1) paddle1_x += paddle1_velx;
    if (paddle1_y + paddle_ht + paddle1_vely < 1 && paddle1_y - paddle_ht + paddle1_vely > -1) paddle1_y += paddle1_vely;
    
    //handle sphere reflection from left and right wall
    if(xTranslated <= radius - 1 || xTranslated >= 1 - radius) {
        velX *= -1;
    }
    if(yTranslated <= radius - 1 || yTranslated >= 1 - radius){
        velY *= -1;
    }
    
    //collision detection and reflection of sphere from front and back plane
    if(zTranslated <= radius - 6.75) {
        velZ *= -1;
        int xLabel = 3-floor((1+yTranslated)/(0.5));
        int yLabel = floor((1+xTranslated)/(0.5));
        FLAG_PATTERN[xLabel][yLabel] = true;     //reveal tile hit by the sphere
    }
    
    if(zTranslated >= -2.75 - radius){
        //increase sphere velocity by 10% after each hit and update the score
        if(fabs(fabs(xTranslated) - fabs(paddle1_x)) < paddle_wd + radius && fabs(fabs(yTranslated) - fabs(paddle1_y)) < paddle_ht + radius) {
            score += 10;
            velZ *= -1.1;
            sprintf(temp, "%d", score);
            Sprint(0.0, 0.0, -4.75, "Hit!", textStyle);
        }
        else{
            // start a new game if sphere misses the paddle
            strcpy (restart,restart_txt);
            sprintf(last_scr_val, "%d", score);
            Sprint(0.0, 0.0, -4.75, "Miss!", textStyle);
            score = 0;
            sprintf(temp, "%d", score);
            new_game();
        }
    }
    // move sphere as per its velocity
    xTranslated += vscale*velX;
    yTranslated += vscale*velY;
    zTranslated += vscale*velZ;
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, Timer, value); // subsequent timer calls for uniform animation on all GPU/CPU
}

void reshapeFunc(int x, int y)
{
    if (y == 0 || x == 0) return;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //Angle of view      : 40 degrees
    //Near clipping plane: 0.5
    //Far clipping plane : 20.0
    gluPerspective(50,(GLdouble)x/(GLdouble)y,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);
}


void idleFunc(void)
{
    //glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y) {
    switch ( key ) {
        case ' ':									// Space bar - Restart Game
            FLAG_NEW_GAME = true;
            strcpy(restart, "");
            glutPostRedisplay();
            break;
            
        case 27:									// Escape key - Quit Game
            exit(1);
            
        case 'r':
            printf("save current screen\n");
            screenshot.capture();
            break;
    }
}

void keySpecial(int key, int x, int y){
    screenshot.capture();
    GLfloat acc = 0.025;
    // F1 - key to toggle between full-screen and default size
    if (key == GLUT_KEY_F1){
        fullScreen = !fullScreen;
        if (fullScreen) {
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
        } else {
            glutReshapeWindow(windowWidth, windowHeight);
            glutPositionWindow(windowPosX, windowPosX);
        }
    }
    
    // set arrow keys flags for smooth transition
    if(key == GLUT_KEY_UP) keys_paddle1[0] = true;
    if(key == GLUT_KEY_DOWN) keys_paddle1[1] = true;
    if(key == GLUT_KEY_LEFT) keys_paddle1[2] = true;
    if(key == GLUT_KEY_RIGHT) keys_paddle1[3] = true;
    
    // increase appropriate velocity with the defined acceleration to avoid jerky movements
    if (keys_paddle1[0] == true)
    {
        paddle1_vely = 1*acc; //Move up
    }
    
    if (keys_paddle1[1] == true)
    {
        paddle1_vely = -1*acc; //Move down
    }
    if (keys_paddle1[2] == true)
    {
        paddle1_velx = -1*acc; //Move left
    }
    
    if (keys_paddle1[3] == true)
    {
        paddle1_velx = 1*acc; //Move right
    }
    
    //check for bounds and update paddle position
    if (paddle1_x-paddle_wd+paddle1_velx < -1.0)
    {
        paddle1_x = -1.0+paddle_wd;
    }
    
    if (paddle1_x+paddle_wd+paddle1_velx > 1.0)
    {
        paddle1_x = 1.0 -paddle_wd;
    }
    
    if (paddle1_y+paddle_ht+paddle1_vely > 1.0)
    {
        paddle1_y = 1.0-paddle_ht;
    }

    if (paddle1_y-paddle_ht+paddle1_vely < -1.0)
    {
        paddle1_y = -1.0+paddle_ht;
    }
}

void keySpecialUp(int key, int x, int y){
    //reset key flags when keys are released
    if(key == GLUT_KEY_UP) keys_paddle1[0] = false;
    if(key == GLUT_KEY_DOWN) keys_paddle1[1] = false;
    if(key == GLUT_KEY_LEFT) keys_paddle1[2] = false;
    if(key == GLUT_KEY_RIGHT) keys_paddle1[3] = false;
    
    if(keys_paddle1[0] == false && keys_paddle1[1] == false) paddle1_vely = 0;
    if(keys_paddle1[2] == false && keys_paddle1[3] == false) paddle1_velx = 0;
}

void mouse(int button, int state, int x, int y) {
    // detect left click of mouse
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (fullScreen){
        }
        else{
            // choose appropriate menu option based on x,y coordinates
            // start new game
            if (x>100 && x<160){
                if(y>175 && y<200){
                    FLAG_NEW_GAME = true;
                    strcpy(restart, "");
                }
                // reveal/hide image
                else if(y>220 && y<245){
                    FLAG_REVEAL = not FLAG_REVEAL;
                    for(int i=0; i<4; i++){
                        for(int j=0; j<4; j++){
                            FLAG_PATTERN[i][j]= not FLAG_PATTERN[i][j];
                        }
                    }
                    if (FLAG_REVEAL){
                        strcpy(reveal, hide);
                    }
                    else {
                        strcpy(reveal, saveReveal);
                    }
                    
                }
                // exit game
                else if(y>265 && y<295){
                    exit(1);
                }
            }
        }
        glutPostRedisplay();
    }
}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);

    //double buffering used to avoid flickering problem in animation
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    //seed for random number generation
    srand(time(0));
    
    //initialize window
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow("ARE YOU GAME?");

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glClearColor(0.0,0.0,0.0,0.0);
    
    //create and define display lists for efficient rendering
    // List 1 --> Menu buttons and Text
    // List 2 --> Tunnel
    // List 3 --> 4*4 empty tiles
    // List 4 --> mapping texture to the tiles
    box = glGenLists (20);
    glNewList(box, GL_COMPILE);
	cube();
    
	glColor3f(1.0f,1.0f,1.0f);
    Sprint(1.15, 0.75, -2.75, scr, textStyle);
    Sprint(1.15, 0.45, -2.75, last_score, textStyle);
    Sprint(-1.53, 0.75, -2.75, inst, textStyle);
    Sprint(-0.2, 1.1, -2.75, game, not textStyle);
    
	glColor3f(1.0f,1.0f,0.0f);
    Sprint(-1.36, 0.61, -2.75, play, textStyle);
    Sprint(-1.36, 0.29, -2.75, quit, textStyle);
    
    glBegin(GL_QUADS);
    glVertex3f(-1.4f,0.58f,-2.75);
    glVertex3f(-1.2f,0.58f,-2.75);
    glVertex3f(-1.2f,0.68f,-2.75);
    glVertex3f(-1.4f,0.68f,-2.75);
    glVertex3f(-1.4f,0.42f,-2.75);
    glVertex3f(-1.13f,0.42f,-2.75);
    glVertex3f(-1.13f,0.52f,-2.75);
    glVertex3f(-1.4f,0.52f,-2.75);
    glVertex3f(-1.4f,0.36f,-2.75);
    glVertex3f(-1.2f,0.36f,-2.75);
    glVertex3f(-1.2f,0.26f,-2.75);
    glVertex3f(-1.4f,0.26f,-2.75);
    glEnd();
    glEndList();
    
    tunnel = box + 1;
    glNewList(tunnel,GL_COMPILE);
    glBegin(GL_QUADS);
    glVertex3f( 1.0f,-1.0f,0.0f);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glVertex3f(-1.0f, 1.0f,0.0f);
    glVertex3f( 1.0f, 1.0f,0.0f);
    glEnd();
    glEndList();
    
    pattern = box + 2;
    glNewList(pattern,GL_COMPILE);
    glBegin(GL_QUADS);
    glVertex3f( -1.0f,1.0f,-6.75f);
    glVertex3f(-0.5f,1.0f,-6.75f);
    glVertex3f(-0.5f,0.5f,-6.75f);
    glVertex3f(-1.0f,0.5f,-6.75f);
    glEnd();
    glEndList();
    
    // single image warped and split into tiles
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            tiles[(4*i)+j] = box + 3 + (4*i)+j;
            glNewList(tiles[(4*i)+j], GL_COMPILE);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0+(j)*0.25,0.75-(i)*0.25);   glVertex3f( -1.0+(j)*0.5,0.5-(i)*0.5,-2.0f);
            glTexCoord2f(0.25+(j)*0.25, 0.75-(i)*0.25); glVertex3f( -0.5+(j)*0.5,0.5-(i)*0.5,-2.0f);
            glTexCoord2f(0.25+(j)*0.25, 1-(i)*0.25);    glVertex3f( -0.5+(j)*0.5,1-(i)*0.5,-2.0f);
            glTexCoord2f(0.0+(j)*0.25, 1-(i)*0.25);     glVertex3f( -1.0+(j)*0.5,1-(i)*0.5,-2.0f);
            glEnd();
            glEndList();
        }
    }
    //event handling
    setupRC();
    glutDisplayFunc(redisplayFunc);
    glutTimerFunc(1000/FPS, Timer, 0);
    glutReshapeFunc(reshapeFunc);
    glutKeyboardFunc(keyPressed);
    
    glutSpecialFunc(keySpecial);
    glutSpecialUpFunc(keySpecialUp);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}