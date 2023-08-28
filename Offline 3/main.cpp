#define _USE_MATH_DEFINES

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include "objects.h"
#include "bitmap_image.hpp"

using namespace std;


Point pos;       // position of the eye
Point center;    // position of center
double d;               // distance of eye from center
Vector l;         // look/forward direction
Vector r;         // right direction
Vector u;         // up direction
double angle;           // angle of rotation of eye about center

vector<Sphere> spheres;
vector<Cube> cubes;
vector<Pyramid> pyramids;
CheckerBoard chk;
int nearPlane, farPlane;
double fovY, fovX;
double aspectRatio;
int recursionLevel;
int numberOfPixels;
int noOfObjects;

void calc_vects(){

    // Calculate the look vector
    l = vectorFromPoints(pos, center);
    l.normalize();
    d = l.length();

    // Calculate the right vector
    r = l ^ u;
    r.normalize();

    // Calculate the up vector
    u = r ^ l;
    u.normalize();
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
        glVertex3f(-1000,0,0);
        glVertex3f(1000,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,-1000,0);
        glVertex3f(0,1000,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,-1000);
        glVertex3f(0,0,1000);
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
    gluPerspective(45.0f, aspect, nearPlane, farPlane);
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
    chk.draw(farPlane, pos);
}

void capture(){
    bitmap_image image(numberOfPixels, numberOfPixels);
    for(int i = 0; i < numberOfPixels; i++){
        for(int j = 0; j < numberOfPixels; j++){
            image.set_pixel(i, j, 0, 0, 0);
        }
    }
    Point middle = pos + l * nearPlane;
    double height = 2 * tan(fovY/2 * M_PI/180) * nearPlane;
    double width = 2 * tan(fovX/2 * M_PI/180) * nearPlane;
    double pixelHeight = height / numberOfPixels;
    double pixelWidth = width / numberOfPixels;
    Point topLeft = middle + u * (height/2) - r * (width/2);
    for(int i = 0; i < numberOfPixels; i++){
        for(int j = 0; j < numberOfPixels; j++){
            Point p = topLeft - u * (i * pixelHeight) + r * (j * pixelWidth);
            Vector v = vectorFromPoints(pos, p);
            v.normalize();
            Ray ray(pos, v);
            Color c = trace(ray, spheres, cubes, pyramids, chk, recursionLevel);
            image.set_pixel(i, j, c.r, c.g, c.b);
        }
    }
    image.save_image("out.bmp");
    cout << "Done" << endl;
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

    drawAll();

    glutSwapBuffers();  // Render now
}

void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.1;
    float v = 0.05;
    double len;
	switch(key){

        case '0':
            capture();
            break;

		case '1':
            r = r * cos(rate) + l * sin(rate);
            l = l * cos(rate) - r * sin(rate);
			break;

        case '2':
            r = r * cos(-rate) + l * sin(-rate);
            l = l * cos(-rate) - r * sin(-rate);
			break;

        case '3':
            l = l * cos(rate) + u * sin(rate);
            u = u * cos(rate) - l * sin(rate);
			break;

        case '4':
            l = l * cos(-rate) + u * sin(-rate);
            u = u * cos(-rate) - l * sin(-rate);
			break;

        case '5':
            u = u * cos(rate) + r * sin(rate);
            r = r * cos(rate) - u * sin(rate);
			break;

        case '6':
            u = u * cos(-rate) + r * sin(-rate);
            r = r * cos(-rate) - u * sin(-rate);
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
    double rate = 5.0;
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos = pos + l * rate;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos = pos - l * rate;
			break;

		case GLUT_KEY_RIGHT:
			pos = pos + r * rate;
			break;
		case GLUT_KEY_LEFT :
			pos = pos - r * rate;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos = pos + u * rate;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos = pos - u * rate;
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
    fin >> chk;
    fin >> noOfObjects;
    fovX = fovY * aspectRatio;
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