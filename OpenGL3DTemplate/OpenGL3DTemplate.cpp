
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <cstdio>

float cameraX = 0.0f;
float cameraY = 5.0f;
float cameraZ = 20.0f;
int timer = 90;
float playerX = 0.0f;
float playerY = 1.25f;
float playerZ = 0.0f;
float playerRotation = 0.0f;
double angle = 0.0f;
float wallColorR = 1.0f;
float wallColorG = 1.0f;
float wallColorB = 1.0f;
bool goalVisible = true;
bool gameEnded = false;
bool playerWins = false;
bool playerCanMove = true;

bool animateTrashBin = false;


void toggleTrashBinAnimation() {
    animateTrashBin = !animateTrashBin;
}

void Anim() {
    angle += 1.0f;
    if (angle >= 360.0f) {
        angle -= 360.0f;
    }


    glutPostRedisplay();
}

void drawCylinder(float radius, float height, int segments) {
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = (2.0f * 3.1415926f * float(i)) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        glVertex3f(x, y, 0.0f);
        glVertex3f(x, y, height);
    }
    glEnd();
}
void drawCylinder1(float radius, float height, int segments) {
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = (2.0f * 3.1415926f * float(i)) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        glVertex3f(x, height, y);
        glVertex3f(x, 0.0f, y);
    }
    glEnd();
}
void drawPlayer() {
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glTranslatef(playerX, playerY, playerZ);
    glRotatef(playerRotation, 0.0f, 1.0f, 0.0f);
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.9, 0.6, 0.4);
    glTranslatef(playerX, playerY + 0.5, playerZ);
    glRotatef(playerRotation, 0.0f, 1.0f, 0.0f);
    glScalef(0.25, 0.25, 0.25);
    glutSolidIcosahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(playerX - 0.3, playerY - 0.5, playerZ);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.1, 0.5, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(playerX + 0.3, playerY - 0.5, playerZ);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.1, 0.5, 20);
    glPopMatrix();

    glColor3f(0.0, 0.0, 1.0);
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(playerX + i * 0.6, playerY, playerZ);
        glRotatef(playerRotation, 0.0, 1.0, 0.0);
        glScalef(0.2, 0.4, 0.2);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawGround() {
    glPushMatrix();

    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_QUADS);
    glVertex3f(-10.0, 0.0, -10.0);
    glVertex3f(10.0, 0.0, -10.0);
    glVertex3f(10.0, 0.0, 10.0);
    glVertex3f(-10.0, 0.0, 10.0);
    glEnd();

    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_QUADS);

    glVertex3f(-5.0, 0.0, -10.0);
    glVertex3f(-5.0, 0.0, 10.0);
    glVertex3f(5.0, 0.0, 10.0);
    glVertex3f(5.0, 0.0, -10.0);


    glVertex3f(-10.0, 0.0, -5.0);
    glVertex3f(10.0, 0.0, -5.0);
    glVertex3f(10.0, 0.0, 5.0);
    glVertex3f(-10.0, 0.0, 5.0);
    glEnd();

    glPopMatrix();
}
void drawCube(float size) {
    glPushMatrix();
    glScalef(size, size, size);
    glutSolidCube(1.0f);
    glPopMatrix();
}
void drawWalls() {
    glPushMatrix();

    glBegin(GL_QUAD_STRIP);
    glColor3f(wallColorR, 0.76, wallColorB);
    glVertex3f(-10.0, 0.0, -10.0);
    glVertex3f(-10.0, 5.0, -10.0);
    glVertex3f(-10.0, 0.0, 10.0);
    glVertex3f(-10.0, 5.0, 10.0);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(-10.0, 5.0, -10.0);
    glVertex3f(-10.0, 5.0, 10.0);
    glVertex3f(-8.0, 5.0, -10.0);
    glVertex3f(-8.0, 5.0, 10.0);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(10.0, 0.0, -10.0);
    glColor3f(wallColorR, wallColorG, 0.3);
    glVertex3f(10.0, 5.0, -10.0);
    glVertex3f(10.0, 0.0, 10.0);
    glVertex3f(10.0, 5.0, 10.0);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(10.0, 5.0, -10.0);
    glVertex3f(10.0, 5.0, 10.0);
    glVertex3f(8.0, 5.0, -10.0);
    glVertex3f(8.0, 5.0, 10.0);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(-10.0, 0.0, -10.0);
    glColor3f(0.6, wallColorG, wallColorB);
    glVertex3f(10.0, 0.0, -10.0);
    glVertex3f(-10.0, 5.0, -10.0);
    glVertex3f(10.0, 5.0, -10.0);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(10.0, 5.0, -10.0);
    glVertex3f(-8.0, 5.0, -10.0);
    glVertex3f(10.0, 5.0, -8.0);
    glVertex3f(-8.0, 5.0, -8.0);
    glEnd();

    glPushMatrix();
    glTranslatef(0.0, 3.0, -10.0);
    drawCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-9.0, 3.0, 0.0);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    drawCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(9.0, 3.0, 0.0);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    drawCube(1);
    glPopMatrix();


    glPopMatrix();
}
void updateWallColor(int value) {
    wallColorR = (float)rand() / (float)RAND_MAX;
    wallColorG = (float)rand() / (float)RAND_MAX;
    wallColorB = (float)rand() / (float)RAND_MAX;

    glutPostRedisplay();

    glutTimerFunc(5000, updateWallColor, 0);
}
void drawTree() {
    glColor3f(0.5, 0.2, 0.0);
    glPushMatrix();
    glTranslatef(2.0, 0.0, 2.0);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    drawCylinder1(0.1, 2.0, 20);
    glPopMatrix();


    glColor3f(0.0, 0.8, 0.0);
    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glTranslatef(2.0, 1.5 + i * 0.2, 2.0);
        glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
        glRotatef(i * 90.0, 0.0, 1.0, 0.0);
        glutSolidCone(0.5, 1.0, 30, 30);
        glPopMatrix();
    }
}
void drawTree1() {
    glColor3f(0.5, 0.2, 0.0);
    glPushMatrix();
    glTranslatef(-2.0, 0.0, 2.0);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    drawCylinder1(0.1, 2.0, 20);
    glPopMatrix();


    glColor3f(0.0, 0.8, 0.0);
    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glTranslatef(-2.0, 1.5 + i * 0.2, 2.0);
        glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
        glRotatef(i * 90.0, 0.0, 1.0, 0.0);
        glutSolidCone(0.5, 1.0, 30, 30);
        glPopMatrix();
    }
}

void drawLampPost(float x, float z) {
    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(x, 0.1, z);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    drawCylinder1(0.1, 2.0, 20);
    glPopMatrix();

    glColor3f(0.9, 0.2, 0.1);
    glPushMatrix();
    glTranslatef(x, 2.3, z);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    glutSolidCube(0.5);
    glPopMatrix();

    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(x, 2.7, z);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
}

void drawLampPosts() {
    drawLampPost(-7.0, -7.0);
    drawLampPost(4.0, -7.0);
    drawLampPost(-3.0, 8.0);
    drawLampPost(4.0, 7.0);
}

void drawTrashCan() {
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-7.0, 1.7, 0.0);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 1.0);
    drawCylinder(0.5, 1.0, 20);
    glPopMatrix();

    glColor3f(0.4, 0.7, 0.0);
    glPushMatrix();
    glTranslatef(-7.0, 1.7, 0.0);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 1.0);
    glutSolidTorus(0.1, 0.6, 20, 20);
    glPopMatrix();


    glColor3f(1, 1, 0.2);
    glPushMatrix();
    glTranslatef(-7, 0.1, 0.0);
    drawCylinder1(0.1, 1.0, 20);

    glPopMatrix();

}

void drawCar() {
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(7.0, 0.2, 0.0);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    glScalef(1.0, 0.2, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();


    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(7.0, 0.35, 0.0);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    glScalef(0.6, 0.2, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glTranslatef(7.0 + i * 0.4, 0.1, j * 0.3);
            glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
            glutSolidTorus(0.1, 0.1, 10, 20);
            glPopMatrix();
        }
    }
}

void drawCandyShop() {
    glColor3f(1.0, 0.0, 0.5);
    glPushMatrix();
    glTranslatef(-7.0, 0.0, 7.0);
    glScalef(2.0, 2.0, 2.0);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-7.0, 2.0, 7.0);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);
    glutSolidCone(1.5, 1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-7.0f, 0.7f, 7.0f);
    glColor3f(1, 1.0, 0.1);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(animateTrashBin ? angle : 0.0f, 0.0, 1.0, 0.0);

    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricTexture(quadric, GL_TRUE);
    gluDisk(quadric, 1.5, 2, 32, 32);

    glPopMatrix();
}
void drawFence() {

    glColor3f(0.5, 0.25, 0.0);
    for (int i = -10; i <= 10; i += 2) {
        for (int j = -10; j <= 10; j += 20) {
            glPushMatrix();
            glTranslatef(i, 0.5, j);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            drawCylinder(0.1, 1.0, 20);
            glPopMatrix();
        }
    }


    glColor3f(0.5, 0.25, 0.0);
    for (int i = -10; i <= 10; i += 2) {
        for (int j = -10; j <= 10; j += 20) {
            glPushMatrix();
            glTranslatef(i, 0.75, j);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            glScalef(2.0, 0.1, 0.1);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }
    glColor3f(0.5, 0.25, 0.0);
    for (int i = -10; i <= 10; i += 2) {
        for (int j = -10; j <= 10; j += 20) {
            glPushMatrix();
            glTranslatef(j, 0.5, i);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
            drawCylinder(0.1, 1.0, 20);
            glPopMatrix();
        }
    }


    glColor3f(0.5, 0.25, 0.0);
    for (int i = -10; i <= 10; i += 2) {
        for (int j = -10; j <= 10; j += 20) {
            glPushMatrix();
            glTranslatef(j, 0.75, i);
            glScalef(2.0, 0.1, 0.1);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }
}

void drawGoal() {
    if (goalVisible) {
        glPushMatrix();

        glColor3f(1.0, 1.0, 0.0);
        glPushMatrix();
        glTranslatef(7.0, 0.7, 7.0);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glutSolidTorus(0.1, 0.5, 20, 20);
        glPopMatrix();


        glColor3f(0.0, 0.0, 1.0);
        glPushMatrix();
        glTranslatef(7.0, 1.6, 7.0);
        glScalef(0.5, 0.5, 0.5);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glutSolidIcosahedron();
        glPopMatrix();


        glColor3f(0.5, 0.25, 0.0);
        glPushMatrix();
        glTranslatef(7.0, 0.0, 7.0);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glScalef(1.0, 0.1, 1.0);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }
}



void updateCameraPosition(unsigned char key) {
    if (!playerCanMove) {
        return;
    }
    float cameraSpeed = 0.5f;
    float playerSpeed = 0.15f;
    float playerRotationSpeed = 2.0f;

    float nextPlayerX = playerX;
    float nextPlayerZ = playerZ;

    switch (key) {
    case 'w':
        nextPlayerZ -= playerSpeed;
        playerRotation += playerRotationSpeed;
        break;
    case 's':
        nextPlayerZ += playerSpeed;
        playerRotation -= playerRotationSpeed;
        break;
    case 'a':
        nextPlayerX -= playerSpeed;
        playerRotation += playerRotationSpeed;
        break;
    case 'd':
        nextPlayerX += playerSpeed;
        playerRotation -= playerRotationSpeed;
        break;


    case 't':

        cameraX = 1.0f;
        cameraY = 20.0f;
        cameraZ = 0.0f;
        break;
    case 'f':

        cameraX = 0.0f;
        cameraY = 5.0f;
        cameraZ = 20.0f;
        break;
    case 'g':

        cameraX = 10.0f;
        cameraY = 5.0f;
        cameraZ = 0.0f;
        break;
    case 'z':
        cameraY -= cameraSpeed;
        break;
    case 'x':
        cameraY += cameraSpeed;
        break;

    }
    if (nextPlayerX > -10 && nextPlayerX < 10 && nextPlayerZ > -10 && nextPlayerZ < 10) {

        playerX = nextPlayerX;
        playerZ = nextPlayerZ;
    }
    if (nextPlayerX > 7 - 0.5 && nextPlayerX < 7 + 0.5 &&
        nextPlayerZ > 7 - 0.5 && nextPlayerZ < 7 + 0.5) {

        playerWins = true;
        goalVisible = false;
    }
}
void specialKeys(int key, int x, int y) {
    float cameraSpeed = 0.5f;

    switch (key) {
    case GLUT_KEY_UP:
        cameraZ -= cameraSpeed;
        break;
    case GLUT_KEY_DOWN:
        cameraZ += cameraSpeed;
        break;
    case GLUT_KEY_LEFT:
        cameraX -= cameraSpeed;
        break;
    case GLUT_KEY_RIGHT:
        cameraX += cameraSpeed;
        break;

        glutPostRedisplay();
    }
}


void updateTimer(int value) {
    if (timer > 0) {
        timer--;
        glutPostRedisplay();
        glutTimerFunc(1000, updateTimer, 0);
    }
    else {

        gameEnded = true;
        glutPostRedisplay();
    }
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'e') {
        animateTrashBin = true;
    }
    else if (key == 'r') {

        toggleTrashBinAnimation();
    }
    updateCameraPosition(key);

    glutPostRedisplay();
}


void setupLights() {
    GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
    GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat shininess[] = { 50 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
    GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    setupLights();
    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(2, 5.2);
    char timerText[50];
    sprintf(timerText, "Timer: %d", timer);
    for (int i = 0; timerText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timerText[i]);
    }

    if (!gameEnded && !playerWins) {

        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(2, 5.2);
        char timerText[50];
        sprintf(timerText, "Timer: %d", timer);
        for (int i = 0; timerText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timerText[i]);
        }
    }
    else  if (playerWins) {
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2f(-1, 0);
        const char* winMessage = "You Win!";
        for (int i = 0; winMessage[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winMessage[i]);
        }

        playerCanMove = false;
    }
    else if (gameEnded) {
        glColor3f(1.0, 0.0, 0.0);
        glRasterPos2f(-1, 0);
        const char* loseMessage = "You Lose!";
        for (int i = 0; loseMessage[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, loseMessage[i]);
        }

        playerCanMove = false;
    }

    drawPlayer();
    drawGround();
    drawWalls();
    drawTree();
    drawTree1();
    drawLampPosts();
    drawTrashCan();
    drawCar();
    drawCandyShop();
    drawFence();
    drawGoal();


    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(50, 50);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("3D Player and Environment");
    glEnable(GL_DEPTH_TEST);
    glutIdleFunc(Anim);
    glutTimerFunc(1000, updateTimer, 0);
    glutTimerFunc(5000, updateWallColor, 0);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);

    glutMainLoop();
    return 0;
}