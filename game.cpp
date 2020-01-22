#include <cstdlib>                      // standard definitions
#include <iostream>                     // C++ I/O
#include <cstdio>                       // C I/O (for sprintf) 
#include <cmath>                        // standard definitions
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <sstream>
#include <string>
#include <cstring>

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

class Agent {
    public:
    int roadPos;
    float road;       //25 possible road
    float position;   //left right position 
    int direction;    // 0 is up,  1 is down
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
int point = 0;
int isStopped = 0;
int isFinised = 0;
Agent agent;

void myReshape(int w, int h) {
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

    agent.roadPos = 0;
    agent.road = roads[0];
    agent.position = 0.5;
    agent.direction = 0;

}

void drawString(float x, float y, float z, char *string) {
  
  glRasterPos3f(x, y, z);
  for (char* c = string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
}

void finish() {
    isStopped = 1;
    isFinised = 1;
}

void drawFinishText() {
    char finishText[] = "Press q to quit.";
    drawString(0.38, 0.49, 1, finishText);
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


void drawAgent() {

    if (agent.direction == 0) {
        glBegin(GL_TRIANGLES);
            glColor3fv(RED_RGB);
            glVertex2f(agent.position, agent.road + 0.013);
            glVertex2f(agent.position - 0.010, agent.road - 0.013);
            glVertex2f(agent.position + 0.010, agent.road - 0.013);
        glEnd();
    } else {
        glBegin(GL_TRIANGLES);
            glColor3fv(RED_RGB);
            glVertex2f(agent.position, agent.road - 0.013);
            glVertex2f(agent.position - 0.010, agent.road + 0.013);
            glVertex2f(agent.position + 0.010, agent.road + 0.013);
        glEnd();

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

            if (agent.road > vehicles[i].lane - 0.013 && agent.road < vehicles[i].lane + 0.013) {
                if (agent.position > vehicles[i].position && agent.position < vehicles[i].position + vehicles[i].type) {
                    finish();
                }
            }
        }
    }

}

void drawVehicles() {

    for (int i = 0; i < 100; i++) {
        if (vehicleControl[i] == 1) {
            glColor3fv(BLUE_RGB);
            glRectf(vehicles[i].position, vehicles[i].lane - 0.013, vehicles[i].position + vehicles[i].type, vehicles[i].lane + 0.013);
        }
    }
    
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


void moveAgent(int move) {

    if (move == 3) {
        if (agent.roadPos < 24) {
            agent.roadPos += 1;
            agent.road = roads[agent.roadPos];
            if (agent.direction == 0) {
                point += 1;
            } else if (agent.direction == 1) {
                finish();
            }
            if (agent.roadPos == 24) {
                agent.direction = 1;
            }
        }
    } else if (move == 2) {
        if (agent.roadPos > 0) {
            agent.roadPos -= 1;
            agent.road = roads[agent.roadPos];
            if (agent.direction == 1) {
                point += 1;
            } else if (agent.direction == 0) {
                finish();
            }
            if (agent.roadPos == 0) {
                agent.direction = 0;
            }
        }
    } else if (move == 0) {
        if (agent.position > 0.025) {
            agent.position -= 0.025;
        }
    } else if (move == 1) {
        if (agent.position < 0.950) {
            agent.position += 0.025;
        }
    }
}

void drawPoint() {
    std::string puan = "Puan: ";
    std::string result;
    std::stringstream sstm;
    sstm << puan << point;
    result = sstm.str();
    
    char *cstr = &result[0];

    drawString(0, 0.005, 1, cstr);
}

void drawNumberOfVehicle() {
    std::string puan = "Vehicle: ";
    std::string result;
    std::stringstream sstm;
    sstm << puan << numberOfVehicle;
    result = sstm.str();
    
    char *cstr = &result[0];

    drawString(0.775, 0.005, 1, cstr);
}

void myDisplay(void) {

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawRoads();
    drawLines();
    drawAgent();

    if (isStopped == 0) {
        int random = std::rand() % 1000 + 1;
        if (random < 100) {
            createVehicle();
        }
        moveVehicles();
    } else {
        drawVehicles();
    }


    drawPoint();
    drawNumberOfVehicle();

    if (isFinised == 1) {
        drawFinishText();
    }

    glutSwapBuffers();

}

void myTimer(int id) {

    glutPostRedisplay();
    glutTimerFunc(TIMER_DELAY, myTimer, 0);
}

void myMouse(int b, int s, int x, int y) {

    if (isFinised == 0) {
        if (s == GLUT_DOWN) {
            if (b == GLUT_LEFT_BUTTON) {
                if (isStopped == 0)
                    isStopped = 1;
                else
                    isStopped = 0;
            }
            if (b == GLUT_RIGHT_BUTTON) {
                if (isStopped == 0)
                    isStopped = 1;
                
                if (isStopped == 1) {
                    moveVehicles();
                }
            }
        }
    }

}

void myKeyboard(unsigned char c, int x, int y) {
    switch (c) {
    case 'q':
        exit(0);
        break;
    default:
        break;
    }
}

void catchKey(int key, int x, int y) {
    if (isStopped == 0) {
        if(key == GLUT_KEY_LEFT)    
            moveAgent(0);
        else if(key == GLUT_KEY_RIGHT)
            moveAgent(1);
        else if(key == GLUT_KEY_DOWN)
            moveAgent(2);
        else if(key == GLUT_KEY_UP)
            moveAgent(3);
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
    glutSpecialFunc(catchKey);
    glutTimerFunc(TIMER_DELAY, myTimer, 0);
    glutMainLoop();                             // start it running
    return 0;                                   // ANSI C expects this

}
