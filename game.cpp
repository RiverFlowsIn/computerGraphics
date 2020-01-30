#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <string.h>
#include <sstream>
#include <string>
#include <cstring>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;

class Vehicle {
public:
    float lane;         // Vehicle vertical lane position
    float position;     // Vehicle horizontal position
    int direction;      // Left(0) - Right(1)
    float type;         // Car(0.026) - Truck(0.052)
};

class Agent {
public:
    int roadPos;        // 0-24 Agent vertical position number
    float road;         // 0-1 Agent vertical position
    float position;     // Agent horizontal position 
    int direction;      // Up(0) - Down(1)
};

class Coin {
public:
    float lane;         // Coin vertical lane position
    float position;     // Coin horizontal position
    float time;         // Time left to disappear
    int isExist;        // Not exist(0) - Exist(1)
};

GLint TIMER_DELAY = 10;
GLfloat RED_RGB[] = { 1, 0, 0 };
GLfloat BLUE_RGB[] = { 0, 0, 1 };
GLfloat WHITE_RGB[] = { 1, 1, 1 };
GLfloat BLACK_RGB[] = { 0, 0, 0 };
GLfloat YELLOW_RGB[] = { 0.8, 0.8, 0 };
GLfloat ORANGE_RGB[] = { 1, 0.5, 0 };
GLfloat GRAY_RGB[] = { 0.5, 0.5, 0.5 };



float* lanes = new float[18]();
float* roads = new float[25]();
int* laneControl = new int[18]();
Vehicle* vehicles = new Vehicle[100]();
int* vehicleControl = new int[100]();
int numberOfVehicle = 0;
int point = 0;
int isStopped = 0;
int isFinised = 0;
int moveStack = -1;
Agent agent;
Coin coin;
int powerMode = 0;
int powerCounter = 0;
int crashedVehicle = -1;
float vehicleSpeed = 0.003;
int vehicleTime = 100;
int gameMode = 2;

void reshapeFunct(int w, int h) {
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
    for (int i = 2; i < 99; i += 4) {
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

    coin.isExist = 0;

}

void finish() {
    isStopped = 1;
    isFinised = 1;
}

void drawString(float x, float y, float z, char* string) {

    glRasterPos3f(x, y, z);
    glColor3fv(YELLOW_RGB);
    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void drawFinishText() {
    char finishText[] = "Press q to quit.";
    drawString(0.38, 0.49, 1, finishText);
}

void drawPoint() {
    std::string puan = "Puan: ";
    std::string result;
    std::stringstream sstm;
    sstm << puan << point;
    result = sstm.str();

    char* cstr = &result[0];

    drawString(0, 0.005, 1, cstr);
}

void drawRemainingTime() {
    std::string puan = "Time: ";
    std::string result;
    std::stringstream sstm;
    sstm << puan << coin.time / 100;
    result = sstm.str();

    char* cstr = &result[0];

    drawString(0, 0.967, 1, cstr);
}

void drawGameMode() {
    std::string mode = "";
    if (gameMode == 1) {
        mode = "Easy";
    }
    else if (gameMode == 2) {
        mode = "Normal";
    }
    else if (gameMode == 3) {
        mode = "Hard";
    }
    std::string result;
    std::stringstream sstm;
    sstm << mode;
    result = sstm.str();

    char* cstr = &result[0];

    drawString(0.85, 0.967, 1, cstr);
}

void drawNumberOfVehicle() {
    std::string puan = "Vehicle: ";
    std::string result;
    std::stringstream sstm;
    sstm << puan << numberOfVehicle;
    result = sstm.str();

    char* cstr = &result[0];

    drawString(0.775, 0.005, 1, cstr);
}

void drawRoads() {

    for (int i = 4; i < 85; i = i + 16) {
        double value = i / 100.0;

        if (isFinised == 1)
            glColor3fv(GRAY_RGB);
        else
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
    }
    else {
        glBegin(GL_TRIANGLES);
        glColor3fv(RED_RGB);
        glVertex2f(agent.position, agent.road - 0.013);
        glVertex2f(agent.position - 0.010, agent.road + 0.013);
        glVertex2f(agent.position + 0.010, agent.road + 0.013);
        glEnd();

    }

}

void drawVehicles() {

    for (int i = 0; i < 100; i++) {
        if (vehicleControl[i] == 1) {
            if (i == crashedVehicle)
                glColor3fv(ORANGE_RGB);
            else
                glColor3fv(BLUE_RGB);

            glRectf(vehicles[i].position, vehicles[i].lane - 0.013, vehicles[i].position + vehicles[i].type, vehicles[i].lane + 0.013);
        }
    }

}

void drawCoin() {

    if (coin.isExist == 1) {
        drawRemainingTime();
        float x1, y1, x2, y2;
        float angle;
        double radius = 0.02;

        x1 = coin.position;
        y1 = coin.lane;
        glColor3fv(YELLOW_RGB);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x1, y1);

        for (angle = 1.0f; angle < 361.0f; angle += 0.2)
        {
            x2 = x1 + sin(angle) * radius;
            y2 = y1 + cos(angle) * radius;
            glVertex2f(x2, y2);
        }

        glEnd();

        if (isStopped == 0) {
            coin.time -= 1;
        }

        if (coin.time < 0) {
            coin.isExist = 0;
        }
    }

}


void moveVehicles() {

    for (int i = 0; i < 100; i++) {
        if (vehicleControl[i] == 1) {
            glColor3fv(BLUE_RGB);
            glRectf(vehicles[i].position, vehicles[i].lane - 0.013, vehicles[i].position + vehicles[i].type, vehicles[i].lane + 0.013);

            if (vehicles[i].direction == 0)
                vehicles[i].position += vehicleSpeed;
            else
                vehicles[i].position -= vehicleSpeed;

            if (vehicles[i].position < 0 || vehicles[i].position > 1) {
                Vehicle v{};
                vehicles[i] = v;
                vehicleControl[i] = 0;
                numberOfVehicle -= 1;
            }

            if (agent.road > vehicles[i].lane - 0.013 && agent.road < vehicles[i].lane + 0.013) {
                if (agent.position > vehicles[i].position&& agent.position < vehicles[i].position + vehicles[i].type) {
                    finish();
                    crashedVehicle = i;
                }
            }
        }
    }

}

void moveAgent(int move) {

    if (move == 3) {
        if (agent.roadPos < 24) {
            agent.roadPos += 1;
            agent.road = roads[agent.roadPos];
            if (agent.direction == 0) {
                point += 1;
                if (gameMode == 3)
                    point += 1;
            }
            else if (agent.direction == 1) {
                finish();
            }
            if (agent.roadPos == 24) {
                agent.direction = 1;
            }
        }
    }
    else if (move == 2) {
        if (agent.roadPos > 0) {
            agent.roadPos -= 1;
            agent.road = roads[agent.roadPos];
            if (agent.direction == 1) {
                point += 1;
                if (gameMode == 3)
                    point += 1;
            }
            else if (agent.direction == 0) {
                finish();
            }
            if (agent.roadPos == 0) {
                agent.direction = 0;
            }
        }
    }
    else if (move == 0) {
        if (agent.position > 0.025) {
            agent.position -= 0.025;
        }
    }
    else if (move == 1) {
        if (agent.position < 0.950) {
            agent.position += 0.025;
        }
    }

    if (agent.road > coin.lane - 0.013 && agent.road < coin.lane + 0.013) {
        if (agent.position > coin.position - 0.05 && agent.position < coin.position + 0.05) {
            point += 5;
            if (gameMode == 3)
                point += 5;
            coin.isExist = 0;
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

void createCoin() {

    int randomLane = std::rand() % 18 + 0;
    int randomPosition = std::rand() % 39 + 1;
    int randomTime = std::rand() % 400 + 300;

    randomPosition *= 25;
    float rPos = randomPosition / 1000.0;

    coin.isExist = 1;
    coin.time = randomTime;
    coin.lane = lanes[randomLane];
    coin.position = rPos;

}

void displayFunct(void) {

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawRoads();
    drawLines();
    drawAgent();

    if (gameMode == 1) {
        vehicleSpeed = 0.002;
        vehicleTime = 60;
    }
    else if (gameMode == 2) {
        vehicleSpeed = 0.003;
        vehicleTime = 100;
    }
    else if (gameMode == 3) {
        vehicleSpeed = 0.006;
        vehicleTime = 200;
    }

    if (isStopped == 0) {
        int random = std::rand() % 1000 + 1;
        if (random < vehicleTime) {
            createVehicle();
        }
        moveVehicles();
    }
    else {
        drawVehicles();
    }

    glColor3fv(YELLOW_RGB);
    drawPoint();
    drawNumberOfVehicle();
    drawGameMode();

    if (isStopped == 0 && coin.isExist == 0) {
        int random = std::rand() % 1000 + 1;
        if (random < 5) {
            createCoin();
        }
    }
    drawCoin();


    if (powerMode == 1 && isFinised == 0) {
        if (agent.direction == 0) {
            moveAgent(3);
        }
        else {
            moveAgent(2);
        }
        powerCounter += 2;

        if (agent.roadPos == 0 || agent.roadPos == 24) {
            powerMode = 0;
            point += powerCounter;
            powerCounter = 0;
        }
    }

    if (isFinised == 1) {
        drawFinishText();
    }

    glutSwapBuffers();

}

void timerFunct(int id) {

    glutPostRedisplay();
    glutTimerFunc(TIMER_DELAY, timerFunct, 0);
}

void mouseFunct(int b, int s, int x, int y) {

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
                    moveAgent(moveStack);
                    moveStack = -1;
                    if (coin.isExist == 1) {
                        coin.time -= 1;
                    }
                    int random = std::rand() % 1000 + 1;
                    if (random < 100) {
                        createVehicle();
                    }
                }
            }
        }
    }

}

void keyboardFunct(unsigned char c, int x, int y) {
    switch (c) {
    case 'q':
    case 'Q':
        exit(0);
        break;
    case '1':
        gameMode = 1;
        break;
    case '2':
        gameMode = 2;
        break;
    case '3':
        gameMode = 3;
        break;
    default:
        break;
    }

    if ((int)c == 13) {
        if (isStopped == 0)
            powerMode = 1;

    }
}

void catchKeyFunct(int key, int x, int y) {
    if (isStopped == 0) {
        if (key == GLUT_KEY_LEFT)
            moveAgent(0);
        else if (key == GLUT_KEY_RIGHT)
            moveAgent(1);
        else if (key == GLUT_KEY_DOWN)
            moveAgent(2);
        else if (key == GLUT_KEY_UP)
            moveAgent(3);
    }
    else {
        if (key == GLUT_KEY_LEFT)
            moveStack = 0;
        else if (key == GLUT_KEY_RIGHT)
            moveStack = 1;
        else if (key == GLUT_KEY_DOWN)
            moveStack = 2;
        else if (key == GLUT_KEY_UP)
            moveStack = 3;
    }

}

int main(int argc, char** argv) {

    std::srand(std::time(0));
    fillArrays();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(displayFunct);
    glutReshapeFunc(reshapeFunct);
    glutMouseFunc(mouseFunct);
    glutKeyboardFunc(keyboardFunct);
    glutSpecialFunc(catchKeyFunct);
    glutTimerFunc(TIMER_DELAY, timerFunct, 0);
    glutMainLoop();
    return 0;

}
