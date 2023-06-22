#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

// Global variables
GLfloat eyex = 3, eyey = 2, eyez = 3;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;

GLfloat myx = 0, myy = 0, myz = 0;
GLfloat xangle = 0, yangle = 0, zangle = 0;

GLfloat scale = 1.0;
GLfloat centroidx = 1.0/3.0, centroidy = 1.0/3.0, centroidz = 1.0/3.0;

GLfloat cylinderParam1 = 2*scale/3.0 + 1.0/3;
GLfloat cylinderParam2 = -scale/3.0 + 1.0/3;

double xx;
double xy;
double xz;
double xr;

double yx;
double yy;
double yz;
double yr;

double zx;
double zy;
double zz;
double zr; 



std::vector<float> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }

    return vertices;
}


void drawSphereSegment(int subdivision, float radius)
{
    glPushMatrix();
        glTranslatef(scale, 0, 0);
        std::vector<float> vertices = buildUnitPositiveX(subdivision);

        // Compute the number of vertices per row and column
        int pointsPerRow = (int)pow(2, subdivision) + 1;
        int pointsPerColumn = pointsPerRow;

        for (int i = 0; i < pointsPerColumn - 1; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < pointsPerRow; ++j)
            {
                int currentIndex = i * pointsPerRow + j;
                int nextRowIndex = (i + 1) * pointsPerRow + j;

                glVertex3f(vertices[currentIndex * 3] * radius, vertices[currentIndex * 3 + 1] * radius, vertices[currentIndex * 3 + 2] * radius);
                glVertex3f(vertices[nextRowIndex * 3] * radius, vertices[nextRowIndex * 3 + 1] * radius, vertices[nextRowIndex * 3 + 2] * radius);
            }
            glEnd();
        }
    glPopMatrix();
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


void drawOctahedronFace(GLfloat angle){
    glPushMatrix();
        glTranslatef(0, 0, 0);
        glRotatef(angle, 0, 0, 1);
        glTranslatef(centroidx, centroidy, centroidz);
        glScalef(scale, scale, scale);
        glTranslatef(-centroidx, -centroidy, -centroidz);
        glBegin(GL_TRIANGLES);
            glVertex3f(0, 0, 1);
            glVertex3f(1, 0, 0);
            glVertex3f(0, 1, 0);
        glEnd();
    glPopMatrix();
}

void drawOctahedron(){
    glColor3f(1.0f,1.0f,0.0f);
    drawOctahedronFace(0);   

    glColor3f(0.0f,0.0f,1.0f);
    drawOctahedronFace(90);

    glColor3f(1.0f,0.0f,0.0f);
    drawOctahedronFace(180);

    glColor3f(0.0f,1.0f,0.0f);
    drawOctahedronFace(270);
    
    glTranslatef(0, 0, 0);
    glRotatef(180, 1, 0, 0);


    glColor3f(1.0f,1.0f,0.0f);
    drawOctahedronFace(0);   

    glColor3f(0.0f,0.0f,1.0f);
    drawOctahedronFace(90);

    glColor3f(1.0f,0.0f,0.0f);
    drawOctahedronFace(180);

    glColor3f(0.0f,1.0f,0.0f);
    drawOctahedronFace(270);
}

void drawCylinderSegment(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    glPushMatrix();
        glTranslatef(1.0/2 * scale, 0, 1.0/2 * scale);
        glRotatef(-45 , 0, 1, 0);
        glRotatef(-36 , 0, 0, 1);
        glBegin(GL_QUADS);
            for (int i = 1; i < (segments / 360.0 * 74); i++) {
                double theta = i * 2.0 * M_PI / segments;
                currx = radius * cos(theta);
                curry = radius * sin(theta);

                glVertex3f(currx, curry, height/2);
                glVertex3f(currx, curry, -height/2);

                glVertex3f(tempx, tempy, -height/2);
                glVertex3f(tempx, tempy, height/2);

                tempx = currx;
                tempy = curry;
            }
        glEnd();
    glPopMatrix();
}

void drawCylinders(){
    glColor3f(0.7f,0.8f,0.9f);
    glPushMatrix();
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
        glRotatef(90, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
        glRotatef(-180, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(90, 0, 1, 0);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
        glRotatef(90, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
        glRotatef(-180, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
        glRotatef(90, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
        glRotatef(-180, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (1 - scale) / (3 * cos(0.9553166178212)), 100);
    glPopMatrix();
    glPushMatrix();
        glRotatef(270, 0, 1, 0);
        drawCylinderSegment(sqrt(2) * scale, (-scale/3.0 + 1.0/3) / cos(0.9553166178212), 100);
        glRotatef(90, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (-scale/3.0 + 1.0/3) / cos(0.9553166178212), 100);
        glRotatef(-180, 0, 0, 1);
        drawCylinderSegment(sqrt(2) * scale, (-scale/3.0 + 1.0/3) / cos(0.9553166178212), 100);
    glPopMatrix();
}

void drawSphereSegments(){
    glPushMatrix();
        glColor3f(1.0f,1.0f,0.0f);
        drawSphereSegment(4, (1.0 - scale)/sqrt(3));
        glColor3f(0.0f,1.0f,0.0f);
        glRotatef(90, 0, 0, 1);
        drawSphereSegment(4, (1.0 - scale)/sqrt(3));
        glColor3f(0.0f,0.0f,1.0f);
        glRotatef(-180, 0, 0, 1);
        drawSphereSegment(4, (1.0 - scale)/sqrt(3));
    glPopMatrix();
    glPushMatrix();
        glColor3f(1.0f,0.0f,1.0f);
        glRotatef(90, 0, 1, 0);
        drawSphereSegment(4, (1.0 - scale)/sqrt(3));
    glPopMatrix();
    glPushMatrix();
        glColor3f(0.0f,1.0f,1.0f);
        glRotatef(180, 0, 1, 0);
        drawSphereSegment(4, (1.0 - scale)/sqrt(3));
    glPopMatrix();
    glPushMatrix();
        glColor3f(1.0f,0.0f,0.0f);
        glRotatef(270, 0, 1, 0);
        drawSphereSegment(4, (1.0 - scale)/sqrt(3));
    glPopMatrix();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(eyex,eyey,eyez,
              centerx,centery,centerz,
              upx,upy,upz);

    glPushMatrix();

        drawAxes();
    
    glPopMatrix();

    glPushMatrix();

        glTranslatef(myx,myy,myz);

        //glScalef(0.5,0.5,0.5);

        drawAxes();

        drawOctahedron();

        drawCylinders();

        drawSphereSegments();
    
    glPopMatrix();

    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
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
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 0.05;
    GLfloat vangle = 10.0 * M_PI / 180.0;

    zx = centerx - eyex;
    zy = centery - eyey;
    zz = centerz - eyez;
    zr = sqrt(zx*zx + zz*zz + zy*zy);
    zx /= zr;
    zy /= zr;
    zz /= zr;

    xx = upy * zz - upz * zy;
    xy = -(upx * zz - upz * zx);
    xz = upx * zy - upy * zx;
    xr = sqrt(xx*xx + xy*xy + xz*xz);
    xx /= xr;
    xy /= xr;
    xz /= xr;

    yx = xy * zz - xz * zy;
    yy = -(xx * zz - xz * zx);
    yz = xx * zy - xy * zx;
    yr = sqrt(yx*yx + yy*yy + yz*yz);
    yx /= yr;
    yy /= yr;
    yz /= yr;


    //double circle_curr_vect_x = myx - eyex;
    //double circle_curr_vect_y = myy - eyey;
    //double circle_curr_vect_z = myz - eyez;
    //double circle_curr_vect_r = sqrt(circle_curr_vect_x * circle_curr_vect_x + circle_curr_vect_y * circle_curr_vect_y + circle_curr_vect_z * circle_curr_vect_z);

    double circle_init_x = -eyex + zr * xx;
    double circle_init_y = -eyey + zr * xy;
    double circle_init_z = -eyez + zr * xz; 

    double circle_init_vect_x = circle_init_x - eyex;
    double circle_init_vect_y = circle_init_y - eyey;
    double circle_init_vect_z = circle_init_z - eyez;
    double circle_init_vect_r = sqrt(circle_init_vect_x * circle_init_vect_x + circle_init_vect_y * circle_init_vect_y + circle_init_vect_z * circle_init_vect_z);
    
    double dot;
    double det;
    switch (key) {
    case '1':
        //calculate the angle between the initial vector and the current vector
        dot = circle_init_vect_x * zx + circle_init_vect_y * zy + circle_init_vect_z * zz;
        det = circle_init_vect_r;
        yangle = acos(dot/det);
        yangle += vangle;
        centerx = -eyex + zr * cos(yangle) * xx + zr * sin(yangle) * zx;
        centery = -eyey + zr * cos(yangle) * xy + zr * sin(yangle) * zy;
        centerz = -eyez + zr * cos(yangle) * xz + zr * sin(yangle) * zz;
        break;
    case '2':
        dot = circle_init_vect_x * zx + circle_init_vect_y * zy + circle_init_vect_z * zz;
        det = circle_init_vect_r;
        yangle = acos(dot/det);
        yangle -= vangle;
        centerx = -eyex + zr * cos(yangle) * xx + zr * sin(yangle) * zx;
        centery = -eyey + zr * cos(yangle) * xy + zr * sin(yangle) * zy;
        centerz = -eyez + zr * cos(yangle) * xz + zr * sin(yangle) * zz;
        break;
    // control eyey
    case '3':
        break;
    case '4':
        break;
    // control eyez
    case '5':
        zangle += vangle;
        upx = -sin(zangle);
        upy = cos(zangle);
        upz = 0;
        break;
    case '6':
        zangle -= vangle;
        upx = -sin(zangle);
        upy = cos(zangle);
        upz = 0;
        break;
    case 'w':
        eyex += v*upx;
        eyey += v*upy;
        eyez += v*upz;
        break;
    case 's':
        eyex -= v*upx;
        eyey -= v*upy;
        eyez -= v*upz;
        break;
    //control scale
    case '.':
        if(scale < 1)
            scale += v;
        break;
    case ',':
        if(scale > 0)
            scale -= v;
        break;
    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y) {
    double v = 0.25;
    zx = centerx - eyex;
    zy = centery - eyey;
    zz = centerz - eyez;
    zr = sqrt(zx*zx + zz*zz + zy*zy);
    zx /= zr;
    zy /= zr;
    zz /= zr;

    xx = upy * zz - upz * zy;
    xy = -(upx * zz - upz * zx);
    xz = upx * zy - upy * zx;
    xr = sqrt(xx*xx + xy*xy + xz*xz);
    xx /= xr;
    xy /= xr;
    xz /= xr;

    yx = xy * zz - xz * zy;
    yy = -(xx * zz - xz * zx);
    yz = xx * zy - xy * zx;
    yr = sqrt(yx*yx + yy*yy + yz*yz);
    yx /= yr;
    yy /= yr;
    yz /= yr;

    switch (key) {
    case GLUT_KEY_LEFT:
        myx += xx * v;
        myy += xy * v;
        myz += xz * v;
        break;
    case GLUT_KEY_RIGHT:
        myx -= xx * v;
        myy -= xy * v;
        myz -= xz * v;
        break;
    case GLUT_KEY_DOWN:
        myx += v * zx;
        myy += v * zy;
        myz += v * zz;
        break;
    case GLUT_KEY_UP:
        myx -= v * zx;
        myy -= v * zy;
        myz -= v * zz;
        break;
    case GLUT_KEY_PAGE_UP:
        myx += yx * v;
        myy += yy * v;
        myz += yz * v;
        break;
    case GLUT_KEY_PAGE_DOWN:
        myx -= yx * v;
        myy -= yy * v;
        myz -= yz * v;
        break;
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}
