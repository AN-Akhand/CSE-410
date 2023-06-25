#define _USE_MATH_DEFINES

#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <cmath>
#include <iostream>

using namespace std;

int frameCount = 0;
int refreshMillis = 10;

GLfloat secondAngle = 0.0f;
GLfloat minuteAngle = 0.0f;
GLfloat hourAngle = 0.0f;


GLfloat dSecondAngle = -6.0f;
GLfloat dMinuteAngle = -0.1f;
GLfloat dHourAngle = -30.0f/3600.0f;

GLfloat pendulumAngle = 45.0f;

GLfloat dPendulumAngle = 0.0f;

GLfloat sunAngle = 0.0f;
GLfloat moonAngle = 0.0f;

GLfloat w = M_PI;

int t = 0;

void getSunMoonAngle(){
	SYSTEMTIME time;
	GetLocalTime(&time);
	if(time.wHour >= 5 && time.wHour < 17){
		sunAngle = (time.wHour - 5) + time.wMinute / 60.0f;
		sunAngle = sunAngle / 12.0f;
		moonAngle = -1 + sunAngle;
	}
	else{
		time.wHour = time.wHour <= 5 ? time.wHour + 24 : time.wHour;
		moonAngle = (time.wHour - 17) + time.wMinute / 60.0f;
		moonAngle = moonAngle / 12.0f;
		sunAngle = -1 + moonAngle;
	}
}

void initClockAngles(){
	SYSTEMTIME time;
	GetLocalTime(&time);
	secondAngle = -6.0f * time.wSecond;
	minuteAngle = -6.0f * time.wMinute - 0.1f * time.wSecond;
	hourAngle = -30.0f * (time.wHour % 12) - 0.5f * time.wMinute - 0.5f/60.0f * time.wSecond;
	if(time.wHour >= 5 && time.wHour < 17){
		sunAngle = (time.wHour - 5) + time.wMinute / 60.0f;
		sunAngle = sunAngle / 12.0f;
		moonAngle = -1 + sunAngle;
	}
	else{
		time.wHour = time.wHour <= 5 ? time.wHour + 24 : time.wHour;
		moonAngle = (time.wHour - 17) + time.wMinute / 60.0f;
		moonAngle = moonAngle / 12.0f;
		sunAngle = -1 + moonAngle;
	}
}

void drawCircleSegment(GLfloat x, GLfloat y, GLfloat radius, GLfloat startAngle, GLfloat endAngle, int lineAmount){
	glLineWidth(1);

	int i;
	GLfloat twicePi = 2.0f * M_PI;

	glBegin(GL_LINE_STRIP);
		for(i = 0; i <= lineAmount;i++) {
			if(i * twicePi / lineAmount >= startAngle && i * twicePi / lineAmount <= endAngle){
				glVertex2f(
				    x + (radius * cos(i *  twicePi / lineAmount)),
				    y + (radius* sin(i * twicePi / lineAmount))
				);
			}
		}
	glEnd();
}

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius, int lineAmount){
	glLineWidth(1);

	int i;
	GLfloat twicePi = 2.0f * M_PI;

	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) {
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)),
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, int lineAmount){
	glLineWidth(1);

	int i;
	GLfloat twicePi = 2.0f * M_PI;

	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= lineAmount;i++) {
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)),
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}

void drawSun(){
	glPushMatrix();
		glScalef(0.7, 0.7, 0.7);
		glColor3f(1.0f,1.0f,0.0f);  // Yellow
		drawFilledCircle(0,0,0.1,100);
		for(int i = 0; i < 8; i++){
			glBegin(GL_LINE_STRIP);
		    	for(float i = 0; i <= 5 * M_PI; i += 0.01){
		    		glVertex2f( 0.01f * sin(i), 0.0185f/2 * i);
		    	}
			glEnd();
			glRotatef(45, 0, 0, 1);
		}
	glPopMatrix();
}

void drawMoon(){
	glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glColor3f(0.85f,0.85f,0.85f);
		drawFilledCircle(0,0,0.1,100);
	glPopMatrix();
}

void drawSunMoon(){
	glPushMatrix();
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(0, -1.45, 0);
		drawCircleSegment(0.0, 0.0, 0.78, M_PI / 2 - 0.6702633602, M_PI/2 + 0.6702633602, 100);
		GLfloat theta;
		if(sunAngle >= 0){
			theta = M_PI / 2 - 0.6702633602 + sunAngle * (0.6702633602 * 2);
			glTranslatef(0.78*cos(theta), 0.78 * sin(theta), 0);
			drawSun();
		}
		else if(moonAngle >= 0){
			theta = M_PI / 2 - 0.6702633602 + moonAngle * (0.6702633602 * 2);
			glTranslatef(0.78*cos(theta), 0.78 * sin(theta), 0);
			drawMoon();
		}
	glPopMatrix();
}

void drawClockMarkings(){
	glColor3f(1.0f,1.0f,1.0f);  // White
	glTranslatef(0, 0, 0);
	for(int i = 0; i < 12; i++){
		if(i % 3 == 0){
			glLineWidth(3);
			glBegin(GL_LINES);
			    glVertex2f( 0.0f, 0.5f);
			    glVertex2f( 0.0f, 0.4f);
			glEnd();
			glRotatef(30, 0, 0, 1);
		}
		else{
			glLineWidth(1);
			glBegin(GL_LINES);
			    glVertex2f( 0.0f, 0.5f);
			    glVertex2f( 0.0f, 0.45f);
			glEnd();
			glRotatef(30, 0, 0, 1);
		}
	}
}

void drawClockFrame(){
	glTranslatef(0, 0, 0);
	glColor3f(0.0, abs(sin(w * t/100)), abs(cos(w * t/100)));
	glBegin(GL_POLYGON);
		glVertex2f( -0.75f, 0.63f);
		glVertex2f( -0.75f, -0.58f);
		glVertex2f( 0.0f, -1.5f);
		glVertex2f( 0.75f, -0.58f);
		glVertex2f( 0.75f, 0.63f);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f( -0.65f, 0.58f);
		glVertex2f( -0.65f, -0.58f);
		glVertex2f( 0.0f, -1.4f);
		glVertex2f( 0.65f, -0.58f);
		glVertex2f( 0.65f, 0.58f);
	glEnd();
}

void drawClockBody(){
	glPushMatrix();
		drawClockFrame();
		glColor3f(abs(cos(w * t/100)), 0.0, abs(sin(w * t/100)));
		glTranslatef(0, 0, 0);
		drawFilledCircle(0,0,0.55,100);
		glColor3f(0.0f,0.0f,0.0f);
		drawFilledCircle(0,0,0.5,100);
		drawClockMarkings();
	glPopMatrix();
}

void drawPendulumPivot(){
	glColor3f(abs(cos(w * t/100)), 0.0, abs(sin(w * t/100)));
	glTranslatef(0, 0, 0);
	drawFilledCircle(0,0,0.03,100);
}

void drawPendulumRod(){
	glLineWidth(6);
	glTranslatef(0, 0, 0);
	glBegin(GL_LINES);  // Each set of 2 vertices form a line of single pixel width
		glColor3f(abs(cos(w * t/100)), 0.0, abs(sin(w * t/100)));
	    glVertex2f( 0.0f, -0.01f);
		glColor3f(0.5f,0.5f,0.5f);
	    glVertex2f( 0.0f, -0.25f);
		glColor3f(0.5f,0.5f,0.5f);
	    glVertex2f( 0.0f, -0.25f);
		glColor3f(0.0, abs(sin(w * t/100)), abs(cos(w * t/100)));
	    glVertex2f( 0.0f, -0.5f);
	glEnd();
}

void drawPendulumBob(){
	glColor3f(0.0, abs(sin(w * t/100)), abs(cos(w * t/100)));
	glTranslatef(0, 0, 0);
	drawFilledCircle(0,-0.48,0.05,100);
}

void drawPendulum(){
	glPushMatrix();
		glTranslatef(0, -0.55, 0);
		glRotatef(pendulumAngle, 0, 0, 1);
		drawPendulumPivot();
		drawPendulumRod();
		drawPendulumBob();
	glPopMatrix();
}

void drawSecondHand(){
	glPushMatrix();
		glColor3f(1.0f,0.0f,0.0f);
		glTranslatef(0, 0, 0);
		glRotatef(secondAngle, 0, 0, 1);
    	glBegin(GL_POLYGON);
    	    glVertex2f( 0.008f, -0.1f);
    	    glVertex2f( -0.008f, -0.1f);
			glVertex2f( 0.00f, 0.4f);
    	glEnd();
	glPopMatrix();
}

void drawMinuteHand(){
	glLineWidth(2);
	glPushMatrix();
		glColor3f(0.0f,1.0f,0.0f);  // White
		glTranslatef(0, 0, 0);
		glRotatef(minuteAngle, 0, 0, 1);
    	//draw vertical sine wave from 0 to 6*pi
		glBegin(GL_LINE_STRIP);
		    for(float i = 0; i <= 12 * M_PI; i += 0.01){
		    	glVertex2f( 0.01f * sin(i), 0.0185f/2 * i);
		    }
		glEnd();
	glPopMatrix();
}

void drawHourHand(){
	glPushMatrix();
		glColor3f(0.0f,0.0f,1.0f);
		glTranslatef(0, 0, 0);
		glRotatef(hourAngle, 0, 0, 1);
    	glBegin(GL_POLYGON);  // Each set of 2 vertices form a line of single pixel width
    	    glVertex2f( 0.012f, -0.05f);
    	    glVertex2f( -0.012f, -0.05f);
			glVertex2f( -0.02f, 0.25f);
			glVertex2f( 0.0f, 0.3f);
			glVertex2f( 0.02f, 0.25f);
    	glEnd();
		glColor3f(0.0f,0.0f,0.0f);
		glBegin(GL_POLYGON);  // Each set of 2 vertices form a line of single pixel width
    	    glVertex2f( 0.005f, -0.04f);
    	    glVertex2f( -0.005f, -0.04f);
			glVertex2f( -0.015f, 0.25f);
			glVertex2f( 0.0f, 0.28f);
			glVertex2f( 0.015f, 0.25f);
    	glEnd();
	glPopMatrix();
}

void drawCovers(){
	glPushMatrix();
		glTranslatef(0, -1.4, 0);
		GLfloat theta = M_PI / 2 - 0.6702633602;
		glBegin(GL_QUADS);
			glColor3f(0.0, abs(sin(w * t/100)), abs(cos(w * t/100)));
			glVertex2d(0.9 * cos(theta), 0.9 * sin(theta));
			glVertex2d(0.6 * cos(theta), 0.6 * sin(theta));
			glVertex2d(0.6 * cos(theta), 0.6 * sin(theta) - 0.12);
			glVertex2d(0.9 * cos(theta), 0.9 * sin(theta) - 0.12);

			glColor3f(0,0,0);
			glVertex2d(0.6 * cos(theta), 0.6 * sin(theta) - 0.12);
			glVertex2d(0.9 * cos(theta), 0.9 * sin(theta) - 0.12);
			glVertex2d(0.9 * cos(theta) + 0.5, 0.9 * sin(theta));
			glVertex2d(0.6 * cos(theta) + 0.5, 0.6 * sin(theta));
		glEnd();
		glRotatef(180, 0, 1, 0);
		glBegin(GL_QUADS);
			glColor3f(0.0, abs(sin(w * t/100)), abs(cos(w * t/100)));
			glVertex2d(0.9 * cos(theta), 0.9 * sin(theta));
			glVertex2d(0.6 * cos(theta), 0.6 * sin(theta));
			glVertex2d(0.6 * cos(theta), 0.6 * sin(theta) - 0.12);
			glVertex2d(0.9 * cos(theta), 0.9 * sin(theta) - 0.12);

			glColor3f(0,0,0);
			glVertex2d(0.6 * cos(theta), 0.6 * sin(theta) - 0.12);
			glVertex2d(0.9 * cos(theta), 0.9 * sin(theta) - 0.12);
			glVertex2d(0.9 * cos(theta) + 0.5, 0.9 * sin(theta));
			glVertex2d(0.6 * cos(theta) + 0.5, 0.6 * sin(theta));
		glEnd();

	glPopMatrix();

}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
	glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

	glScalef(0.8, 0.8, 0.8);

	glTranslatef(0, 0.4, 0);

	drawClockBody();

	drawHourHand();

	drawMinuteHand();

	drawSecondHand();

	drawSunMoon();

	drawPendulum();

	drawCovers();

    glFlush();  // Render now
}

void timer(int value){
	frameCount++;
	t++;
	//if(frameCount == 300*100){
	//	frameCount = 0;
	//	initClockAngles();
	//}
	if(frameCount == 100){
		frameCount = 0;
		secondAngle += dSecondAngle;
		minuteAngle += dMinuteAngle;
		hourAngle += dHourAngle;
		if(secondAngle == 360.0f){
			secondAngle = 0.0f;
		}
		if(minuteAngle == 360.0f){
			minuteAngle = 0.0f;
		}
		if(hourAngle == 360.0f){
			hourAngle = 0.0f;
		}
	}

	pendulumAngle = 45.0f * cos(w * t/100);

	if(t == 1000) {
		getSunMoonAngle();
		t = 0;
	}

	glutPostRedisplay();    // Post re-paint request to activate display()
	glutTimerFunc(refreshMillis, timer, 0);             // First timer call immediately
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
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutCreateWindow("Clock");  // Create a window with the given title
	initClockAngles();
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
	glutReshapeFunc(reshape);               // Register callback handler for window re-shape
	glutTimerFunc(0, timer, 0);             // First timer call immediately
    glutMainLoop();                         // Enter the infinitely event-processing loop
    return 0;
}