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

vector<Object*> objects;
vector<Light> lights;
vector<SpotLight> spotLights;
int nearPlane, farPlane;
double fovY, fovX;
double aspectRatio;
int recursionLevel;
int numberOfPixels;
int noOfObjects;
int noOfLights;
int noOfSpotLights;
CheckerBoard *chk;

void calc_vects(){

    // Calculate the look vector
    l = center - pos;
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
    for(int i=0;i<objects.size();i++){
        objects[i]->draw();
        objects[i]->draw(farPlane, pos);
    }
    for(int i=0;i<lights.size();i++){
        lights[i].draw();
    }
    for(int i=0;i<spotLights.size();i++){
        spotLights[i].draw();
    }
}

void capture(string filename){
    cout << "Capturing..." << endl;
    bitmap_image image(numberOfPixels, numberOfPixels);
    for(int i = 0; i < numberOfPixels; i++){
        for(int j = 0; j < numberOfPixels; j++){
            image.set_pixel(i, j, 0, 0, 0);
        }
    }
    Point middle = pos + l * (nearPlane + d);
    double height = 2 * tan(fovY/2 * M_PI/180) * nearPlane;
    double width = 2 * tan(fovX/2 * M_PI/180) * nearPlane;
    double pixelHeight = height / numberOfPixels;
    double pixelWidth = width / numberOfPixels;
    Point topLeft = middle + u * (height/2) - r * (width/2);
    int percent = 0;
    Point p;
    for(int i = 0; i < numberOfPixels; i++){
        if(i % (numberOfPixels/10) == 0){
            cout << percent << "% done" << endl;
            percent += 10;
        }
        for(int j = 0; j < numberOfPixels; j++){
            p = topLeft - u * (i * pixelHeight) + r * (j * pixelWidth);
            Vector v = p - pos;
            v.normalize();
            Ray ray(pos, v);
            Color c = trace(ray, objects, lights, spotLights, farPlane ,recursionLevel) * 255;
            if(c.r > 255) c.r = 255;
            if(c.g > 255) c.g = 255;
            if(c.b > 255) c.b = 255;
            if(c.r < 0) c.r = 0;
            if(c.g < 0) c.g = 0;
            if(c.b < 0) c.b = 0;
            image.set_pixel(j, i, c.r, c.g, c.b);
        }
    }
    cout << p << endl;
    cout << "Done" << endl;
    image.save_image(filename);
    cout << "Image saved" << endl;
}

void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // control viewing (or camera)
    center = pos + l * d;
    gluLookAt(pos.x,pos.y,pos.z,
              center.x,center.y,center.z, 
              u.x,u.y,u.z);

    //drawAxes();

    drawAll();

    glutSwapBuffers();  // Render now
}

void rotateCameraX(double rate){
    l = l * cos(rate) + u * sin(rate);
    u = u * cos(rate) - l * sin(rate);
}

void rotateCameraY(double rate){
    r = r * cos(rate) + l * sin(rate);
    l = l * cos(rate) - r * sin(rate);
}

void rotateCameraZ(double rate){
    u = u * cos(rate) + r * sin(rate);
    r = r * cos(rate) - u * sin(rate);
}

void focusCenterMoveVert(double v){
    pos.y += 1*v;
    calc_vects();
}

void moveVert(double v){
    pos = pos + u * v;
}

void moveHor(double v){
    pos = pos + r * v;
}

void moveDepth(double v){
    pos = pos + l * v;
}

void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.05;
    float v = 0.02;
    double len;
    int frame = 0;
	switch(key){

        case '0':
            capture("out.bmp");
            break;

		case '1':
            rotateCameraY(rate);
			break;

        case '2':
            rotateCameraY(-rate);
			break;

        case '3':
            rotateCameraX(rate);
			break;

        case '4':
            rotateCameraX(-rate);
			break;

        case '5':
            rotateCameraZ(rate);
			break;

        case '6':
            rotateCameraZ(-rate);
			break;

        case 'w':
            focusCenterMoveVert(v);
            break;
        
        case 's':
            focusCenterMoveVert(-v);
            break;

        case 'a':
            angle += rate*360;
            break;
        
        case 'd':
            angle -= rate*360;
            break;

        case ' ':
            chk->tex = !chk->tex;
            break;
        
		default:
			break;
	}
	glutPostRedisplay();
}

void specialKeyListener(int key, int x,int y)
{
    double rate = 10.0;
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			moveDepth(rate);
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			moveDepth(-rate);
			break;

		case GLUT_KEY_RIGHT:
			moveHor(rate);
			break;
		case GLUT_KEY_LEFT :
			moveHor(-rate);
			break;

		case GLUT_KEY_PAGE_UP:
		    moveVert(rate);
			break;
		case GLUT_KEY_PAGE_DOWN:
            moveVert(-rate);
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
    cout << "pos: " << pos << endl;
	glutPostRedisplay();
}

void input(){
    ifstream fin;
    fin.open("description.txt");
    fin >> nearPlane >> farPlane >> fovY >> aspectRatio >> recursionLevel >> numberOfPixels;
    chk = new CheckerBoard();
    fin >> *chk;
    objects.push_back(chk);
    fin >> noOfObjects;
    fovX = fovY * aspectRatio;
    for(int i=0;i<noOfObjects;i++){
        string type;
        fin >> type;
        if(type == "sphere"){
            Sphere *s = new Sphere();
            fin >> *s;
            objects.push_back(s);
        }
        else if(type == "pyramid"){
            Pyramid *p = new Pyramid();
            fin >> *p;
            objects.push_back(p);
        }
        else if(type == "cube"){
            Cube *c = new Cube();
            fin >> *c;
            objects.push_back(c);
        }
    }
    fin >> noOfLights;
    for(int i=0;i<noOfLights;i++){
        Light l;
        fin >> l;
        lights.push_back(l);
    }
    fin >> noOfSpotLights;
    for(int i=0;i<noOfSpotLights;i++){
        SpotLight s;
        fin >> s;
        spotLights.push_back(s);
    }
    cout << "Input taken" << endl;
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    input();
    center = Point(0, 50, 0);
    pos = Point(0, 50, -300);
    u.x=0;u.y=1;u.z=0;
    calc_vects();

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(numberOfPixels, numberOfPixels);           // Set the window's initial width & height
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