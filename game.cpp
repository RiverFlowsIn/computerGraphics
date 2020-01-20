#include <cstdlib>                      // standard definitions
#include <iostream>                     // C++ I/O
#include <cstdio>                       // C I/O (for sprintf) 
#include <cmath>                        // standard definitions

#include <GL/glut.h>                    // GLUT
#include <GL/glu.h>                     // GLU
#include <GL/gl.h>                      // OpenGL

using namespace std;                    // make std accessible

GLint TIMER_DELAY = 10;                     
GLfloat RED_RGB[] = { 1.0, 0.0, 0.0 };
GLfloat BLUE_RGB[] = { 0.0, 0.0, 1.0 };
GLfloat WHITE_RGB[] = { 1, 1, 1 };
GLfloat BLACK_RGB[] = { 0, 0, 0 };
float* lanes = new float[18]();
float vehicle = 0.0;
float car = 1.0;

void myReshape(int w, int h) {
    cout << "MyReshape called width=" << w << " height=" << h << endl;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void fillArray() {

    int j = 0;
    for (int i = 10; i < 95; i = i + 16) {
        float number = i / 100.0;
        lanes[j++] = number - 0.04;
        lanes[j++] = number;
        lanes[j++] = number + 0.04;
    }

}

void drawRoads() {

    for (int i = 4; i < 85; i = i + 16) {
        double value = i / 100.0;
        glColor3fv(WHITE_RGB);
        glRectf(0, value, 1, value + 0.12);
    }

}

void drawLines() {

    for (int i = 8; i < 95; i = i + 16) {
        double number = i / 100.0;
        for (int j = 0; j < 10; j = j + 1) {
            double value = j / 10.0 + 0.01;
            glColor3fv(BLACK_RGB);
            glRectf(value, number - 0.002, value + 0.08, number + 0.002);

            glColor3fv(BLACK_RGB);
            glRectf(value, number + 0.04 - 0.002, value + 0.08, number + 0.04 + 0.002);
        }
    }
}

void drawTruck() {

    for (int i = 0; i < 18; i++) {
        glColor3fv(BLUE_RGB);
        glRectf(vehicle, lanes[i] - 0.013, vehicle + 0.052, lanes[i] + 0.013);
    }
    vehicle += 0.001;

    if (vehicle > 1)
        vehicle = 0;

}

void drawCar() {

    for (int i = 0; i < 18; i++) {
        glColor3fv(BLUE_RGB);
        glRectf(car, lanes[i] - 0.013, car + 0.026, lanes[i] + 0.013);
    }

    car -= 0.001;

    if (car < 0)
        car = 1;

}

void myDisplay(void) {

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawRoads();
    drawLines();
    drawTruck();
    drawCar();
    glutSwapBuffers();

}

void myTimer(int id) {

    glutPostRedisplay();
    glutTimerFunc(TIMER_DELAY, myTimer, 0);
}

void myMouse(int b, int s, int x, int y) {
    if (s == GLUT_DOWN) {
        cout << "Mouse click detected at coordinates x="
            << x << " and y=" << y << endl;
        if (b == GLUT_LEFT_BUTTON) {
            cout << "Left mouse click.  Reversing colors." << endl;
            glutPostRedisplay();
        }
    }
}

void myKeyboard(unsigned char c, int x, int y) {
    switch (c) {
    case 'q':
        exit(0);
        break;
    default:
        cout << "Hit q to quit.  All other characters ignored" << endl;
        break;
    }
}

int main(int argc, char** argv)
{

    fillArray();

    glutInit(&argc, argv);                      // OpenGL initializations
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);// double buffering and RGB
    glutInitWindowSize(500, 600);               // create a 500x600 window
    glutInitWindowPosition(0, 0);               // ...in the upper left
    glutCreateWindow(argv[0]);                  // create the window

    glutDisplayFunc(myDisplay);                 // setup callbacks
    glutReshapeFunc(myReshape);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutTimerFunc(TIMER_DELAY, myTimer, 0);
    glutMainLoop();                             // start it running
    return 0;                                   // ANSI C expects this

}
