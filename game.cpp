#include <cstdlib>                      // standard definitions
#include <iostream>                     // C++ I/O
#include <cstdio>                       // C I/O (for sprintf) 
#include <cmath>                        // standard definitions
#include <cstdlib>
#include <ctime>

#include <GL/glut.h>                    // GLUT
#include <GL/glu.h>                     // GLU
#include <GL/gl.h>                      // OpenGL

using namespace std;                    // make std accessible

class Vehicle {
  public:
    float lane;
    float position;
    int direction;             // 0 is left to right, 1 is right to left
    float type;                //0.026 is car, 0.052 is truck
};

GLint TIMER_DELAY = 10;                     
GLfloat RED_RGB[] = { 1.0, 0.0, 0.0 };
GLfloat BLUE_RGB[] = { 0.0, 0.0, 1.0 };
GLfloat WHITE_RGB[] = { 1, 1, 1 };
GLfloat BLACK_RGB[] = { 0, 0, 0 };
float* lanes = new float[18]();
float* roads = new float[25]();
int* laneControl = new int[18]();
Vehicle* vehicles = new Vehicle[100]();
int* vehicleControl = new int[100]();
int numberOfVehicle = 0;

void myReshape(int w, int h) {
    cout << "MyReshape called width=" << w << " height=" << h << endl;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void fillArrays() {

    int j = 0;
    for (int i = 10; i < 95; i = i + 16) {
        float number = i / 100.0;
        lanes[j++] = number - 0.04;
        lanes[j++] = number;
        lanes[j++] = number + 0.04;
    }

    j = 0;
    for (int i = 2; i < 99; i+= 4) {
        float number = i / 100.0;
        roads[j++] = number;
    }

    for (int i = 0; i < 18; i++) {
        laneControl[i] = 0;
    }

    for (int i = 0; i < 100; i++) {
        vehicleControl[i] = 0;
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


void moveVehicles() {

    for (int i = 0; i < 100; i++) {
        if (vehicleControl[i] == 1) {
            glColor3fv(BLUE_RGB);
            glRectf(vehicles[i].position, vehicles[i].lane - 0.013, vehicles[i].position + vehicles[i].type, vehicles[i].lane + 0.013);

            if (vehicles[i].direction == 0)
                vehicles[i].position += 0.003;
            else
                vehicles[i].position -= 0.003;

            if (vehicles[i].position < 0 || vehicles[i].position > 1) {
                Vehicle v;
                vehicles[i] = v;
                vehicleControl[i] = 0;
                numberOfVehicle -= 1;
            }
        }
    }

    cout << numberOfVehicle << "\n";

}


void createVehicle() {

    int randomLane = std::rand() % 18 + 0;
    int randomVehicle = std::rand() % 2 + 0;
    int randomDirection = std::rand() % 2 + 0;


    Vehicle v;
    v.lane = lanes[randomLane];
    if (randomVehicle == 0)
        v.type = 0.026;
    else
        v.type = 0.052;

    v.direction = randomLane % 2 == 0;
    v.position = randomLane % 2 == 0;
 

    int index = -1;
    for (int i = 0; i < 100; i++) {
        if (vehicleControl[i] == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        vehicles[index] = v;
        vehicleControl[index] = 1;
        numberOfVehicle += 1;
    }

}

void myDisplay(void) {

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawRoads();
    drawLines();

    int road = 0;

    glBegin(GL_TRIANGLES);
		glColor3fv(RED_RGB);
		glVertex2f(0.5, roads[road] + 0.013);
		glVertex2f(0.490, roads[road] - 0.013);
		glVertex2f(0.510, roads[road] - 0.013);
	glEnd();

    int random = std::rand() % 1000 + 1;
    if (random < 100) {
        createVehicle();
    }
    moveVehicles();

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
    std::srand(std::time(0));
    fillArrays();

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
