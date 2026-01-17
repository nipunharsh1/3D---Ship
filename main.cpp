#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <cmath>

// Camera variables
float cameraAngleX = 20.0f;
float cameraAngleY = 45.0f;
float cameraDistance = 15.0f;

// Animation variables
float shipRotation = 0.0f;
float waveOffset = 0.0f;

// Mouse control variables
int lastMouseX = 0;
int lastMouseY = 0;
bool isRotating = false;

// Function to draw the ship hull
void drawShipHull() {
    glColor3f(0.4f, 0.2f, 0.1f); // Brown color for hull
    
    // Main hull (bottom part)
    glPushMatrix();
    glScalef(3.0f, 0.8f, 1.5f);
    
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
    
    glPopMatrix();
    
    // Ship bow (front pointed part)
    glColor3f(0.35f, 0.18f, 0.08f);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.7f, 0.0f, 0.7f);
    glVertex3f(1.5f, -0.4f, 0.0f);
    glVertex3f(2.2f, -0.4f, 0.0f);
    glVertex3f(1.5f, 0.4f, 0.75f);
    
    glNormal3f(0.7f, 0.0f, -0.7f);
    glVertex3f(1.5f, -0.4f, 0.0f);
    glVertex3f(1.5f, 0.4f, -0.75f);
    glVertex3f(2.2f, -0.4f, 0.0f);
    
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(1.5f, -0.4f, 0.75f);
    glVertex3f(1.5f, -0.4f, -0.75f);
    glVertex3f(2.2f, -0.4f, 0.0f);
    
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.5f, 0.4f, 0.75f);
    glVertex3f(2.2f, -0.4f, 0.0f);
    glVertex3f(1.5f, 0.4f, -0.75f);
    glEnd();
}

// Function to draw the ship deck
void drawDeck() {
    glColor3f(0.6f, 0.5f, 0.4f); // Light brown for deck
    
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 0.0f);
    glScalef(2.8f, 0.1f, 1.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Function to draw the cabin/superstructure
void drawCabin() {
    glColor3f(0.8f, 0.8f, 0.9f); // Light gray/white for cabin
    
    // Main cabin
    glPushMatrix();
    glTranslatef(-0.3f, 0.9f, 0.0f);
    glScalef(1.5f, 0.8f, 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Bridge (upper cabin)
    glColor3f(0.7f, 0.7f, 0.8f);
    glPushMatrix();
    glTranslatef(-0.3f, 1.5f, 0.0f);
    glScalef(1.0f, 0.5f, 0.9f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Windows
    glColor3f(0.3f, 0.5f, 0.7f); // Blue windows
    for (float x = -0.8f; x <= 0.2f; x += 0.3f) {
        glPushMatrix();
        glTranslatef(x, 0.9f, 0.61f);
        glScalef(0.15f, 0.15f, 0.02f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
}

// Function to draw masts
void drawMasts() {
    glColor3f(0.3f, 0.2f, 0.1f); // Dark brown for masts
    
    // Main mast
    glPushMatrix();
    glTranslatef(0.5f, 1.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.08, 0.08, 2.5, 10, 10);
    gluDeleteQuadric(quad);
    glPopMatrix();
    
    // Rear mast
    glPushMatrix();
    glTranslatef(-0.8f, 1.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    quad = gluNewQuadric();
    gluCylinder(quad, 0.06, 0.06, 2.0, 10, 10);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

// Function to draw sails
void drawSails() {
    glColor4f(0.95f, 0.95f, 0.9f, 0.8f); // Off-white sails
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Main sail
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.3f, 1.5f, 0.0f);
    glVertex3f(0.7f, 1.5f, 0.0f);
    glVertex3f(0.7f, 3.5f, 0.0f);
    glVertex3f(0.3f, 3.5f, 0.0f);
    glEnd();
    
    // Rear sail
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.5f, 0.0f);
    glVertex3f(-0.6f, 1.5f, 0.0f);
    glVertex3f(-0.6f, 3.0f, 0.0f);
    glVertex3f(-1.0f, 3.0f, 0.0f);
    glEnd();
    
    glDisable(GL_BLEND);
}

// Function to draw the complete ship
void drawShip() {
    glPushMatrix();
    glRotatef(shipRotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, sin(waveOffset) * 0.2f, 0.0f); // Bobbing motion
    
    drawShipHull();
    drawDeck();
    drawCabin();
    drawMasts();
    drawSails();
    
    glPopMatrix();
}

// Function to draw water surface
void drawWater() {
    glColor4f(0.0f, 0.3f, 0.5f, 0.6f); // Blue water with transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, 0.0f);
    
    // Draw animated water surface
    glBegin(GL_QUADS);
    for (float x = -20.0f; x < 20.0f; x += 1.0f) {
        for (float z = -20.0f; z < 20.0f; z += 1.0f) {
            float h1 = sin(x * 0.3f + waveOffset) * 0.1f;
            float h2 = sin((x + 1.0f) * 0.3f + waveOffset) * 0.1f;
            float h3 = sin((z + 1.0f) * 0.3f + waveOffset * 1.2f) * 0.1f;
            float h4 = sin(z * 0.3f + waveOffset * 1.2f) * 0.1f;
            
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x, h1 + h4, z);
            glVertex3f(x + 1.0f, h2 + h4, z);
            glVertex3f(x + 1.0f, h2 + h3, z + 1.0f);
            glVertex3f(x, h1 + h3, z + 1.0f);
        }
    }
    glEnd();
    
    glPopMatrix();
    glDisable(GL_BLEND);
}

// Function to draw sky
void drawSky() {
    glDisable(GL_LIGHTING);
    
    glBegin(GL_QUADS);
    // Sky gradient from light blue to darker blue
    glColor3f(0.5f, 0.7f, 0.9f);
    glVertex3f(-50.0f, -10.0f, -50.0f);
    glVertex3f(50.0f, -10.0f, -50.0f);
    glColor3f(0.3f, 0.5f, 0.8f);
    glVertex3f(50.0f, 30.0f, -50.0f);
    glVertex3f(-50.0f, 30.0f, -50.0f);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Set up camera
    gluLookAt(
        cameraDistance * cos(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f),
        cameraDistance * sin(cameraAngleX * M_PI / 180.0f),
        cameraDistance * sin(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f),
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    drawSky();
    drawWater();
    drawShip();
    
    glutSwapBuffers();
}

// Reshape callback
void reshape(int width, int height) {
    if (height == 0) height = 1;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Timer callback for animation
void timer(int) {
    shipRotation += 0.3f;
    if (shipRotation > 360.0f) shipRotation -= 360.0f;
    
    waveOffset += 0.05f;
    if (waveOffset > 2.0f * M_PI) waveOffset -= 2.0f * M_PI;
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Mouse callback
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRotating = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isRotating = false;
        }
    }
}

// Mouse motion callback
void mouseMotion(int x, int y) {
    if (isRotating) {
        cameraAngleY += (x - lastMouseX) * 0.5f;
        cameraAngleX += (y - lastMouseY) * 0.5f;
        
        // Clamp camera angle X
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

// Keyboard callback
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'w':
        case 'W':
            cameraDistance -= 0.5f;
            if (cameraDistance < 5.0f) cameraDistance = 5.0f;
            break;
        case 's':
        case 'S':
            cameraDistance += 0.5f;
            if (cameraDistance > 30.0f) cameraDistance = 30.0f;
            break;
        case 27: // ESC key
            exit(0);
            break;
    }
    glutPostRedisplay();
}

// Initialize OpenGL settings
void init() {
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Set up lighting
    GLfloat lightPos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    glEnable(GL_NORMALIZE);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Ship - OpenGL Project");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    
    printf("3D Ship Controls:\n");
    printf("- Left Mouse Button + Drag: Rotate camera\n");
    printf("- W/S: Zoom in/out\n");
    printf("- ESC: Exit\n\n");
    
    glutMainLoop();
    return 0;
}
