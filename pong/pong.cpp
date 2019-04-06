#include <string>
#include <bits/stdc++.h>
#include <sstream> 
#include <math.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/freeglut.h"
#pragma comment(lib, "OpenGL32.lib")
// keycodes

#define PI 3.14159

// window size and update rate (60 fps)
int width = 500;
int height = 200;
int interval = 1000 / 60;

// score
int score_left = 0;
int score_right = 0;

// rackets in general
int racket_width = 10;
int racket_height = 80;
int racket_speed = 3;

// left racket position
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

// right racket position
float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;

// ball
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 2;

unsigned char key;


void draw();
void updateBall();
void update(int value);
void enable2D(int width, int height);
void drawText(float x, float y, std::string text);
void drawRect(float x, float y, float width, float height );
void keyboard(unsigned char key, int x, int y);
void updateBall();
void vec2_norm(float &x, float &y);
std::string int2str(int x);
void drawFilledCircle(float x, float y, float radius);


// program entry point
int main(int argc, char** argv) {
    // initialize opengl (via glut)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Pong");

    // Register callback functions   
    glutDisplayFunc(draw);
    glutTimerFunc(interval, update, 0);

    // setup scene to 2d mode and set draw color to white
    enable2D(width, height);
    glColor3f(1.0f, 1.0f, 1.0f);

    glutKeyboardFunc(keyboard);
    // start the whole thing
    glutMainLoop();
    return 0;
}

void draw() {
    // clear (has to be done at the beginning)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // draw rackets
    drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
    drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

    // draw score
    drawText(width / 2 - 10, height - 15,
             int2str(score_left) + ":" + int2str(score_right)); 

    // draw ball
	drawFilledCircle(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size /2);

    // swap buffers (has to be done at the end)
    glutSwapBuffers();
}

void drawFilledCircle(float x, float y, float radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	float twicePi = 2.0f * PI;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

void update(int value) {
   // input handling

   // update ball
   updateBall();

   // Call update() again in 'interval' milliseconds
   glutTimerFunc(interval, update, 0);

   // Redisplay frame
   glutPostRedisplay();
}

void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}
std::string int2str(int x) {
    // converts int to string
    std::stringstream ss;
    ss << x;
    return ss.str( );
}
void drawText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}


void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void keyboard(unsigned char key, int x, int y) {
    // left racket
    if (key == 'w') racket_left_y += racket_speed;
    if (key == 's') racket_left_y -= racket_speed;
    
    // right racket
    if (key == 'i') racket_right_y += racket_speed;
    if (key == 'k') racket_right_y -= racket_speed;
}

void updateBall() {
    // fly a bit
    ball_pos_x += ball_dir_x * ball_speed;
    ball_pos_y += ball_dir_y * ball_speed;
    
    // hit by left racket?
    if (ball_pos_x < racket_left_x + racket_width && 
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = t;
    }
    
    // hit by right racket?
    if (ball_pos_x > racket_right_x && 
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = t;
    }

    // hit left wall?
    if (ball_pos_x < 0) {
        ++score_right;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = 0;
    }

    // hit right wall?
    if (ball_pos_x > width) {
        ++score_left;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = 0;
    }

    // hit top wall?
    if (ball_pos_y > height) {
        ball_dir_y = -fabs(ball_dir_y); // force it to be negative
    }

    // hit bottom wall?
    if (ball_pos_y < 0) {
        ball_dir_y = fabs(ball_dir_y); // force it to be positive
    }

    // make sure that length of dir stays at 1
    vec2_norm(ball_dir_x, ball_dir_y);
}

void vec2_norm(float& x, float &y) {
        // sets a vectors length to 1 (which means that x + y == 1)
        float length = sqrt((x * x) + (y * y));
        if (length != 0.0f) {
            length = 1.0f / length;
            x *= length;
            y *= length;
        }
    }
