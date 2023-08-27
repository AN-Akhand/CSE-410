#define _USE_MATH_DEFINES

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include "objects.h"

using namespace std;


struct Point pos;       // position of the eye
struct Point center;    // position of center
double d;               // distance of eye from center
struct Point l;         // look/forward direction
struct Point r;         // right direction
struct Point u;         // up direction
double angle;           // angle of rotation of eye about center

vector<Sphere> spheres;
vector<Cube> cubes;
vector<Pyramid> pyramids;
int nearPlane, farPlane;
int fovY;
int aspectRatio;
int recursionLevel;
int numberOfPixels;
double chkWidth;
double chkAmbient, chkDiffuse, chkReflection;
int noOfObjects;

void calc_vects(){

    // Calculate the look vector
    l.x = center.x - pos.x;
    l.y = center.y - pos.y;
    l.z = center.z - pos.z;
    d = sqrt(l.x*l.x + l.y*l.y + l.z*l.z);
    l.x /= d;
    l.y /= d;
    l.z /= d;

    // Calculate the right vector
    r.x = l.y * u.z - l.z * u.y;
    r.y = l.z * u.x - l.x * u.z;
    r.z = l.x * u.y - l.y * u.x;
    double length = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
    r.x /= length;
    r.y /= length;
    r.z /= length;

    // Calculate the up vector
    u.x = r.y * l.z - r.z * l.y;
    u.y = r.z * l.x - r.x * l.z;
    u.z = r.x * l.y - r.y * l.x;
    length = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
    u.x /= length;
    u.y /= length;
    u.z /= length;
}


void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(-1,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,-1,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,-1);
        glVertex3f(0,0,1);
    glEnd();
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 500.0f);
}

void drawAll(){
    for(int i=0;i<spheres.size();i++){
        spheres[i].draw();
    }
    for(int i=0;i<cubes.size();i++){
        cubes[i].draw();
    }
    for(int i=0;i<pyramids.size();i++){
        pyramids[i].draw();
    }
}

void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // control viewing (or camera)
    center.x = pos.x + l.x * d;
    center.y = pos.y + l.y * d;
    center.z = pos.z + l.z * d;
    gluLookAt(pos.x,pos.y,pos.z,
              center.x,center.y,center.z, 
              u.x,u.y,u.z);

    drawAxes();

    // Point p1(0,0,0), p2(2, 6, 2);

    // Sphere s1(p2, 3, Color(1,0,0));

    // s1.draw();

    // Cube c1(p1, 5, Color(0,1,0));

    // c1.draw();

    // Pyramid py1(p1, 5.0, 1.0, Color(0,0,1));
    // py1.draw();

    drawAll();

    glutSwapBuffers();  // Render now
}

void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.1;
    float v = 0.05;
    double len;
	switch(key){

		case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
			break;

        case 'w':
            pos.y += 1*v;
            calc_vects();
            break;
        
        case 's':
            pos.y -= 1*v;
            calc_vects();
            break;

        case 'a':
            angle += rate*360;
            break;
        
        case 'd':
            angle -= rate*360;
            break;

		default:
			break;
	}
	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y)
{
    double rate = 1.0;
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x * rate;
			pos.y+=l.y * rate;
			pos.z+=l.z * rate;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x * rate;
			pos.y-=l.y * rate;
			pos.z-=l.z * rate;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x * rate;
			pos.y+=r.y * rate;
			pos.z+=r.z * rate;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x * rate;
			pos.y-=r.y * rate;
			pos.z-=r.z * rate;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x * rate;
			pos.y+=u.y * rate;
			pos.z+=u.z * rate;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x * rate;
			pos.y-=u.y * rate;
			pos.z-=u.z * rate;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

void input(){
    ifstream fin;
    fin.open("description.txt");
    fin >> nearPlane >> farPlane >> fovY >> aspectRatio >> recursionLevel >> numberOfPixels;
    fin >> chkWidth >> chkAmbient >> chkDiffuse >> chkReflection;
    fin >> noOfObjects;
    for(int i=0;i<noOfObjects;i++){
        string type;
        fin >> type;
        if(type == "sphere"){
            Sphere s;
            fin >> s;
            spheres.push_back(s);
        }
        else if(type == "pyramid"){
            Pyramid p;
            fin >> p;
            pyramids.push_back(p);
        }
        else if(type == "cube"){
            Cube c;
            fin >> c;
            cubes.push_back(c);
        }
    }
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    input();
    pos.x=150;pos.y=100;pos.z=150;
    center.x=0;center.y=0;center.z=0;
    u.x=0;u.y=1;u.z=0;
    calc_vects();

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Ray Tracing");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);               // Register callback handler for window re-shape

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    return 0;
}