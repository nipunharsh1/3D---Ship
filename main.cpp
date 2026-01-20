#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

// --- VARIABLES ---
float angleX = 10.0f;
float angleY = -45.0f;
float zoom = 1.0f;

// Ship Position
float shipX = 0.0f;
float shipZ = 0.0f;
float shipRotation = 0.0f;

// Propeller Animation
float propAngle = 0.0f;

// Camera
bool followCamera = false;

// Input
bool keyState[256] = { false };

// Speed
float moveSpeed = 0.2f;
float turnSpeed = 1.5f;

#define PI 3.14159265

// --- PARTICLE SYSTEM (WAKE) ---
struct WakeParticle {
    float x, z;
    float life;
    float size;
    bool active;
};

#define MAX_PARTICLES 500
WakeParticle wake[MAX_PARTICLES];
int pIndex = 0;

// --- INITIALIZATION ---
void init() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat lightPos0[] = { 30.0f, 50.0f, 30.0f, 1.0f };
    GLfloat sunColor[] = { 1.0f, 0.95f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunColor);

    GLfloat lightPos1[] = { -10.0f, 5.0f, -10.0f, 0.0f };
    GLfloat dimLight[] = { 0.2f, 0.3f, 0.4f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dimLight);

    glEnable(GL_FOG);
    GLfloat fogColor[] = {0.5f, 0.8f, 1.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, 0.005f);

    for(int i=0; i<MAX_PARTICLES; i++) wake[i].active = false;
}

void setMaterialColor(float r, float g, float b, float a = 1.0f) {
    GLfloat mat_ambient[] = { r*0.3f, g*0.3f, b*0.3f, a };
    GLfloat mat_diffuse[] = { r, g, b, a };
    GLfloat mat_specular[] = { 0.2f, 0.2f, 0.2f, a };
    GLfloat shininess[] = { 10.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void setWaterMaterial() {
    GLfloat mat_ambient[] = { 0.0f, 0.1f, 0.3f, 1.0f };
    GLfloat mat_diffuse[] = { 0.0f, 0.2f, 0.6f, 0.75f };
    GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat shininess[] = { 80.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

// --- WAKE LOGIC ---
void spawnWakeParticle(float x, float z, float spread) {
    wake[pIndex].x = x + ((rand()%100)/100.0f * spread - spread/2.0f);
    wake[pIndex].z = z + ((rand()%100)/100.0f * spread - spread/2.0f);
    wake[pIndex].life = 1.0f;
    wake[pIndex].size = 0.5f + (rand()%50)/100.0f;
    wake[pIndex].active = true;
    pIndex++;
    if (pIndex >= MAX_PARTICLES) pIndex = 0;
}

void updateWake() {
    for(int i=0; i<MAX_PARTICLES; i++) {
        if(wake[i].active) {
            wake[i].life -= 0.01f;
            wake[i].size += 0.02f;
            if(wake[i].life <= 0.0f) wake[i].active = false;
        }
    }

    if (keyState['w'] || keyState['W']) {
        float rad = shipRotation * PI / 180.0f;
        float sternX = shipX - sin(rad) * 3.5f;
        float sternZ = shipZ - cos(rad) * 3.5f;
        spawnWakeParticle(sternX, sternZ, 1.0f);
        spawnWakeParticle(sternX, sternZ, 1.5f);

        float rightX = shipX + sin(rad + PI/2) * 1.2f;
        float rightZ = shipZ + cos(rad + PI/2) * 1.2f;
        spawnWakeParticle(rightX, rightZ, 0.3f);

        float leftX = shipX + sin(rad - PI/2) * 1.2f;
        float leftZ = shipZ + cos(rad - PI/2) * 1.2f;
        spawnWakeParticle(leftX, leftZ, 0.3f);
    }
}

void drawWake() {
    glDepthMask(GL_FALSE);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for(int i=0; i<MAX_PARTICLES; i++) {
        if(wake[i].active) {
            setMaterialColor(1.0f, 1.0f, 1.0f, wake[i].life * 0.8f);
            float s = wake[i].size / 2.0f;
            float y = -0.9f;
            glVertex3f(wake[i].x - s, y, wake[i].z - s);
            glVertex3f(wake[i].x - s, y, wake[i].z + s);
            glVertex3f(wake[i].x + s, y, wake[i].z + s);
            glVertex3f(wake[i].x + s, y, wake[i].z - s);
        }
    }
    glEnd();
    glDepthMask(GL_TRUE);
}

// --- DRAWING HELPERS ---
void drawSun() {
    glDisable(GL_LIGHTING); glDisable(GL_FOG);
    glColor3f(1.0f, 0.95f, 0.5f);
    glPushMatrix(); glTranslatef(30.0f, 50.0f, 30.0f); glutSolidSphere(3.0f, 20, 20); glPopMatrix();
    glEnable(GL_FOG); glEnable(GL_LIGHTING);
}

void drawPyramid(float baseSize, float height) {
    float half = baseSize / 2.0f; float slopeRatio = baseSize / height;
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, slopeRatio, 1.0f); glVertex3f(0.0f, height, 0.0f); glVertex3f(-half, 0.0f, half); glVertex3f(half, 0.0f, half);
    glNormal3f(1.0f, slopeRatio, 0.0f); glVertex3f(0.0f, height, 0.0f); glVertex3f(half, 0.0f, half); glVertex3f(half, 0.0f, -half);
    glNormal3f(0.0f, slopeRatio, -1.0f); glVertex3f(0.0f, height, 0.0f); glVertex3f(half, 0.0f, -half); glVertex3f(-half, 0.0f, -half);
    glNormal3f(-1.0f, slopeRatio, 0.0f); glVertex3f(0.0f, height, 0.0f); glVertex3f(-half, 0.0f, -half); glVertex3f(-half, 0.0f, half);
    glEnd();
}

void drawRock() {
    glPushMatrix();
    setMaterialColor(0.4f, 0.35f, 0.3f);
    glScalef(0.8f, 0.6f, 0.9f);
    glutSolidDodecahedron();
    glPopMatrix();
}

void drawCoconutTree() {
    glPushMatrix();
        setMaterialColor(0.55f, 0.40f, 0.25f); glRotatef(-90, 1.0f, 0.0f, 0.0f);
        GLUquadricObj *q = gluNewQuadric(); gluCylinder(q, 0.2f, 0.15f, 2.0f, 8, 8);
        glTranslatef(0.0f, 0.0f, 2.0f); setMaterialColor(0.1f, 0.6f, 0.1f);
        for(int i=0; i<6; i++) {
            glPushMatrix();
                glRotatef(i * 60.0f, 0.0f, 0.0f, 1.0f); glRotatef(40.0f, 0.0f, 1.0f, 0.0f);
                glBegin(GL_TRIANGLES); glNormal3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.4f, 1.5f, 0.0f); glVertex3f(-0.4f, 1.5f, 0.0f); glEnd();
            glPopMatrix();
        }
    glPopMatrix();
}

void drawMountains() {
    // 1. Center Island
    glPushMatrix();
        glTranslatef(0.0f, -1.5f, -60.0f);
        setMaterialColor(0.8f, 0.7f, 0.5f); drawPyramid(70.0f, 4.0f);
        setMaterialColor(0.2f, 0.5f, 0.1f); glTranslatef(0.0f, 1.0f, 0.0f); drawPyramid(50.0f, 35.0f);
        for(int i=0; i<5; i++) { glPushMatrix(); float angle=i*72.0f*PI/180.0f; float d=25.0f; glTranslatef(sin(angle)*d, -1.0f, cos(angle)*d); glScalef(2.0f, 2.0f, 2.0f); drawCoconutTree(); glPopMatrix(); }
        for(int i=0; i<8; i++) { glPushMatrix(); float angle=(i*45.0f+10.0f)*PI/180.0f; float d=28.0f; glTranslatef(sin(angle)*d, 0.2f, cos(angle)*d); glScalef(1.5f, 1.5f, 1.5f); drawRock(); glPopMatrix(); }
    glPopMatrix();

    // 2. Right Island
    glPushMatrix();
        glTranslatef(35.0f, -1.5f, -50.0f);
        setMaterialColor(0.8f, 0.7f, 0.5f); drawPyramid(45.0f, 3.0f);
        setMaterialColor(0.3f, 0.6f, 0.2f); glTranslatef(0.0f, 1.0f, 0.0f); drawPyramid(30.0f, 20.0f);
        for(int i=0; i<3; i++) { glPushMatrix(); float angle=i*120.0f*PI/180.0f; glTranslatef(sin(angle)*15.0f, -1.0f, cos(angle)*15.0f); glScalef(1.5f, 1.5f, 1.5f); drawCoconutTree(); glPopMatrix(); }
        for(int i=0; i<5; i++) { glPushMatrix(); float angle=(i*72.0f)*PI/180.0f; glTranslatef(sin(angle)*18.0f, 0.2f, cos(angle)*18.0f); glScalef(1.2f, 1.2f, 1.2f); drawRock(); glPopMatrix(); }
    glPopMatrix();

    // 3. Left Island
    glPushMatrix();
        glTranslatef(-30.0f, -1.5f, -55.0f);
        setMaterialColor(0.8f, 0.7f, 0.5f); drawPyramid(55.0f, 3.5f);
        setMaterialColor(0.15f, 0.45f, 0.1f); glTranslatef(0.0f, 1.0f, 0.0f); drawPyramid(40.0f, 25.0f);
        for(int i=0; i<4; i++) { glPushMatrix(); float angle=i*90.0f*PI/180.0f; glTranslatef(sin(angle)*20.0f, -1.0f, cos(angle)*20.0f); glScalef(1.8f, 1.8f, 1.8f); drawCoconutTree(); glPopMatrix(); }
        for(int i=0; i<6; i++) { glPushMatrix(); float angle=(i*60.0f+15.0f)*PI/180.0f; glTranslatef(sin(angle)*23.0f, 0.2f, cos(angle)*23.0f); glScalef(1.4f, 1.4f, 1.4f); drawRock(); glPopMatrix(); }
    glPopMatrix();
}

void drawOcean() {
    glDepthMask(GL_FALSE);
    setWaterMaterial();
    float time = glutGet(GLUT_ELAPSED_TIME) / 500.0f;
    float step = 4.0f;
    float start = -200.0f; float end = 200.0f;
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (float x = start; x < end; x += step) {
        glBegin(GL_QUAD_STRIP);
        for (float z = start; z <= end; z += step) {
            float y1 = sin(x * 0.08f + time) * 0.6f + cos(z * 0.09f + time * 0.4f) * 0.4f;
            glVertex3f(x, y1 - 1.5f, z);
            float y2 = sin((x + step) * 0.08f + time) * 0.6f + cos(z * 0.09f + time * 0.4f) * 0.4f;
            glVertex3f(x + step, y2 - 1.5f, z);
        }
        glEnd();
    }
    glDepthMask(GL_TRUE);
}

// --- PROPELLER ASSEMBLY (With Struts) ---
void drawPropellerAssembly() {
    // 1. V-STRUTS
    setMaterialColor(0.8f, 0.6f, 0.2f); // Gold
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.1f);
        glPushMatrix(); glRotatef(45.0f, 0.0f, 0.0f, 1.0f); glRotatef(-90, 1.0f, 0.0f, 0.0f); GLUquadricObj *q1 = gluNewQuadric(); gluCylinder(q1, 0.03f, 0.03f, 0.4f, 8, 8); glPopMatrix();
        glPushMatrix(); glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); glRotatef(-90, 1.0f, 0.0f, 0.0f); GLUquadricObj *q2 = gluNewQuadric(); gluCylinder(q2, 0.03f, 0.03f, 0.4f, 8, 8); glPopMatrix();
    glPopMatrix();

    // 2. SHAFT
    setMaterialColor(0.1f, 0.1f, 0.1f);
    glPushMatrix();
        glRotatef(90, 0.0f, 1.0f, 0.0f); glTranslatef(0.0f, 0.0f, -0.3f);
        GLUquadricObj *q = gluNewQuadric(); gluCylinder(q, 0.05f, 0.05f, 0.6f, 8, 8);
    glPopMatrix();

    // 3. PROPELLER
    setMaterialColor(0.8f, 0.6f, 0.2f); // Brass
    glPushMatrix();
        glRotatef(90, 0.0f, 1.0f, 0.0f); glTranslatef(0.0f, 0.0f, 0.3f);
        glPushMatrix(); glScalef(0.1f, 0.1f, 0.15f); glutSolidSphere(1.0f, 10, 10); glPopMatrix();
        for(int i=0; i<3; i++) {
            glPushMatrix();
            glRotatef(i * 120.0f + propAngle, 0.0f, 0.0f, 1.0f); glTranslatef(0.2f, 0.0f, 0.0f); glRotatef(30.0f, 1.0f, 0.0f, 0.0f); glScalef(0.5f, 0.12f, 0.02f);
            glutSolidSphere(1.0f, 8, 8);
            glPopMatrix();
        }
    glPopMatrix();
}

// --- REFINED STERN BOX ---
void drawSternBox() {
    setMaterialColor(0.1f, 0.1f, 0.1f); // Black
    glPushMatrix();
        glTranslatef(-3.2f, -0.55f, 0.0f); // Tucked high and tight
        glScalef(0.6f, 0.15f, 0.5f); // Sleek, small box
        glutSolidCube(1.0f);
    glPopMatrix();
}

// --- SHIP MODEL PARTS ---
void drawVBlock(float length, float height, float topW, float botW, float yPos) {
    float halfLen = length / 2.0f; float halfH = height / 2.0f; float topZ = topW / 2.0f; float botZ = botW / 2.0f;
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f); glVertex3f(-halfLen, yPos + halfH, -topZ); glVertex3f(-halfLen, yPos + halfH,  topZ); glVertex3f( halfLen, yPos + halfH,  topZ); glVertex3f( halfLen, yPos + halfH, -topZ);
    glNormal3f(0.0f, -1.0f, 0.0f); glVertex3f(-halfLen, yPos - halfH, -botZ); glVertex3f( halfLen, yPos - halfH, -botZ); glVertex3f( halfLen, yPos - halfH,  botZ); glVertex3f(-halfLen, yPos - halfH,  botZ);
    glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(-halfLen, yPos + halfH, topZ); glVertex3f(-halfLen, yPos - halfH, botZ); glVertex3f( halfLen, yPos - halfH, botZ); glVertex3f( halfLen, yPos + halfH, topZ);
    glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(-halfLen, yPos + halfH, -topZ); glVertex3f( halfLen, yPos + halfH, -topZ); glVertex3f( halfLen, yPos - halfH, -botZ); glVertex3f(-halfLen, yPos - halfH, -botZ);
    glNormal3f(-1.0f, 0.0f, 0.0f); glVertex3f(-halfLen, yPos + halfH,  topZ); glVertex3f(-halfLen, yPos + halfH, -topZ); glVertex3f(-halfLen, yPos - halfH, -botZ); glVertex3f(-halfLen, yPos - halfH,  botZ);
    glNormal3f(1.0f, 0.0f, 0.0f); glVertex3f( halfLen, yPos + halfH,  topZ); glVertex3f( halfLen, yPos - halfH,  botZ); glVertex3f( halfLen, yPos - halfH, -botZ); glVertex3f( halfLen, yPos + halfH, -topZ);
    glEnd();
}

void drawHullDecoration() {
    setMaterialColor(1.0f, 1.0f, 1.0f);
    float y1 = 0.0f, y2 = 0.15f; float z1 = (0.45f + 0.3f * y1) + 0.01f; float z2 = (0.45f + 0.3f * y2) + 0.01f; float len = 3.0f;
    glBegin(GL_QUADS); glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(-len, y2, z2); glVertex3f(-len, y1, z1); glVertex3f( len, y1, z1); glVertex3f( len, y2, z2); glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(-len, y2, -z2); glVertex3f( len, y2, -z2); glVertex3f( len, y1, -z1); glVertex3f(-len, y1, -z1); glEnd();
    glBegin(GL_QUAD_STRIP); for(float angle=0; angle<=180; angle+=10) { float rad=angle*PI/180.0f; float x=3.0f+(sin(rad)*2.5f); glNormal3f(sin(rad), 0.5f, cos(rad)); glVertex3f(x, y2, cos(rad)*z2); glVertex3f(x, y1, cos(rad)*z1); } glEnd();
    setMaterialColor(1.0f, 0.0f, 0.0f);
    float y3 = -0.3f, y4 = -0.15f; float z3 = (0.45f + 0.3f * y3) + 0.01f; float z4 = (0.45f + 0.3f * y4) + 0.01f;
    glBegin(GL_QUADS); glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(-len, y4, z4); glVertex3f(-len, y3, z3); glVertex3f( len, y3, z3); glVertex3f( len, y4, z4); glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(-len, y4, -z4); glVertex3f( len, y4, -z4); glVertex3f( len, y3, -z3); glVertex3f(-len, y3, -z3); glEnd();
    glBegin(GL_QUAD_STRIP); for(float angle=0; angle<=180; angle+=10) { float rad=angle*PI/180.0f; float x=3.0f+(sin(rad)*2.5f); glNormal3f(sin(rad), 0.5f, cos(rad)); glVertex3f(x, y4, cos(rad)*z4); glVertex3f(x, y3, cos(rad)*z3); } glEnd();
    setMaterialColor(0.3f, 0.3f, 0.3f);
    for(float lx = -2.5f; lx < 3.0f; lx += 1.5f) { float ly_top=0.4f, ly_bot=-0.4f; float lz_top=(0.45f+0.3f*ly_top)+0.015f; float lz_bot=(0.45f+0.3f*ly_bot)+0.015f; float wid=0.05f; glBegin(GL_QUADS); glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(lx, ly_top, lz_top); glVertex3f(lx, ly_bot, lz_bot); glVertex3f(lx+wid, ly_bot, lz_bot); glVertex3f(lx+wid, ly_top, lz_top); glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(lx, ly_top, -lz_top); glVertex3f(lx+wid, ly_top, -lz_top); glVertex3f(lx+wid, ly_bot, -lz_bot); glVertex3f(lx, ly_bot, -lz_bot); glEnd(); }
}

void drawCurvedStern() {
    float hullX = -3.0f; float topY = 0.5f; float botY = -0.5f; float redBotY = -0.6f; float radiusTopZ = 0.6f; float radiusBotZ = 0.3f; float radiusRedZ = 0.15f; float lenX = 1.0f;
    setMaterialColor(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0.0f, 1.0f, 0.0f); glVertex3f(hullX, topY, 0.0f); for(float a=90; a<=270; a+=10) { float rad=a*PI/180.0f; glVertex3f(hullX+(cos(rad)*lenX), topY, sin(rad)*radiusTopZ); } glEnd();
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0.0f, -1.0f, 0.0f); glVertex3f(hullX, botY, 0.0f); for(float a=90; a<=270; a+=10) { float rad=a*PI/180.0f; glVertex3f(hullX+(cos(rad)*lenX*0.6f), botY, sin(rad)*radiusBotZ); } glEnd();
    glBegin(GL_QUAD_STRIP); for(float a=90; a<=270; a+=10) { float rad=a*PI/180.0f; glNormal3f(cos(rad), 0.2f, sin(rad)); glVertex3f(hullX+(cos(rad)*lenX), topY, sin(rad)*radiusTopZ); glVertex3f(hullX+(cos(rad)*lenX*0.6f), botY, sin(rad)*radiusBotZ); } glEnd();
    setMaterialColor(0.6f, 0.1f, 0.1f);
    glBegin(GL_QUAD_STRIP); for(float a=90; a<=270; a+=10) { float rad=a*PI/180.0f; glNormal3f(cos(rad), 0.2f, sin(rad)); glVertex3f(hullX+(cos(rad)*lenX*0.6f), botY, sin(rad)*radiusBotZ); glVertex3f(hullX+(cos(rad)*lenX*0.4f), redBotY, sin(rad)*radiusRedZ); } glEnd();
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0.0f, -1.0f, 0.0f); glVertex3f(hullX, redBotY, 0.0f); for(float a=90; a<=270; a+=10) { float rad=a*PI/180.0f; glVertex3f(hullX+(cos(rad)*lenX*0.4f), redBotY, sin(rad)*radiusRedZ); } glEnd();
}

void drawDeckWindows(float length, float width, float height) {
    setMaterialColor(0.1f, 0.1f, 0.2f); float zSide = width/2.0f + 0.02f; float windowSize = 0.12f; float gap = 0.25f; float startX = -(length/2.0f) + 0.3f; float endX = (length/2.0f) - 0.3f;
    glBegin(GL_QUADS); for(float x=startX; x<endX; x+=gap) { glNormal3f(0.0f, 0.0f, 1.0f); glVertex3f(x, -windowSize/2, zSide); glVertex3f(x+windowSize, -windowSize/2, zSide); glVertex3f(x+windowSize, windowSize/2, zSide); glVertex3f(x, windowSize/2, zSide); glNormal3f(0.0f, 0.0f, -1.0f); glVertex3f(x, -windowSize/2, -zSide); glVertex3f(x+windowSize, -windowSize/2, -zSide); glVertex3f(x+windowSize, windowSize/2, -zSide); glVertex3f(x, windowSize/2, -zSide); } glEnd();
}

void drawBridge() {
    setMaterialColor(0.95f, 0.95f, 0.95f);
    glPushMatrix(); glTranslatef(1.8f, 1.25f, 0.0f); glScalef(0.8f, 0.3f, 0.7f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(1.8f, 1.2f, 0.0f); glScalef(0.4f, 0.15f, 1.4f); glutSolidCube(1.0f); glPopMatrix();
    setMaterialColor(0.1f, 0.1f, 0.2f);
    float frontX = 1.8f + 0.41f; float yBot = 1.25f; float yTop = 1.35f; float zWid = 0.3f;
    glBegin(GL_QUADS); glNormal3f(1.0f, 0.0f, 0.0f); glVertex3f(frontX, yBot, zWid); glVertex3f(frontX, yBot, -zWid); glVertex3f(frontX, yTop, -zWid); glVertex3f(frontX, yTop, zWid); glEnd();
    setMaterialColor(0.5f, 0.5f, 0.5f);
    glPushMatrix(); glTranslatef(1.8f, 1.5f, 0.0f); glScalef(0.1f, 0.4f, 0.1f); glutSolidCube(1.0f); glTranslatef(0.0f, 0.2f, 0.0f); glScalef(1.0f, 0.2f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
}

void drawShipModel() {
    glPushMatrix();
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    glScalef(3.0f, 3.0f, 3.0f);
    setMaterialColor(0.2f, 0.2f, 0.2f); drawVBlock(6.0f, 1.0f, 1.2f, 0.6f, 0.0f);
    setMaterialColor(0.6f, 0.1f, 0.1f); drawVBlock(6.0f, 0.1f, 0.6f, 0.3f, -0.55f);
    setMaterialColor(0.2f, 0.2f, 0.2f);
    float startX = 3.0f, topY = 0.5f, bottomY = -0.5f; float sideZTop = 0.6f, sideZBot = 0.3f;
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0.0f, 1.0f, 0.0f); glVertex3f(startX, topY, 0.0f); for(float angle=0; angle<=180; angle+=10) { float rad=angle*PI/180.0f; glVertex3f(startX+(sin(rad)*2.5f), topY, cos(rad)*sideZTop); } glEnd();
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0.0f, -1.0f, 0.0f); glVertex3f(startX, bottomY, 0.0f); for(float angle=0; angle<=180; angle+=10) { float rad=angle*PI/180.0f; glVertex3f(startX+(sin(rad)*2.5f), bottomY, cos(rad)*sideZBot); } glEnd();
    glBegin(GL_QUAD_STRIP); for(float angle=0; angle<=180; angle+=10) { float rad=angle*PI/180.0f; float x=startX+(sin(rad)*2.5f); glNormal3f(sin(rad), 0.5f, cos(rad)); glVertex3f(x, topY, cos(rad)*sideZTop); glVertex3f(x, bottomY, cos(rad)*sideZBot); } glEnd();
    setMaterialColor(0.6f, 0.1f, 0.1f);
    float redTopY = -0.5f, redBotY = -0.6f, sideZKeel = 0.15f;
    glBegin(GL_QUAD_STRIP); for(float angle=0; angle<=180; angle+=10) { float rad=angle*PI/180.0f; float x=startX+(sin(rad)*2.5f); glNormal3f(sin(rad), 0.5f, cos(rad)); glVertex3f(x, redTopY, cos(rad)*sideZBot); glVertex3f(x, redBotY, cos(rad)*sideZKeel); } glEnd();
    drawHullDecoration();
    drawCurvedStern();

    // --- DRAW BLACK BOX (Mounting Block) ---
    drawSternBox();

    float d1_len = 5.5f, d1_h = 0.2f, d1_wid = 1.0f; glPushMatrix(); glTranslatef(0.0f, 0.6f, 0.0f); glPushMatrix(); setMaterialColor(0.95f, 0.95f, 0.95f); glScalef(d1_len, d1_h, d1_wid); glutSolidCube(1.0f); glPopMatrix(); drawDeckWindows(d1_len, d1_wid, d1_h); glPopMatrix();
    float d2_len = 5.0f, d2_h = 0.2f, d2_wid = 0.9f; glPushMatrix(); glTranslatef(0.0f, 0.8f, 0.0f); glPushMatrix(); setMaterialColor(0.95f, 0.95f, 0.95f); glScalef(d2_len, d2_h, d2_wid); glutSolidCube(1.0f); glPopMatrix(); drawDeckWindows(d2_len, d2_wid, d2_h); glPopMatrix();
    float d3_len = 4.5f, d3_h = 0.2f, d3_wid = 0.8f; glPushMatrix(); glTranslatef(0.0f, 1.0f, 0.0f); glPushMatrix(); setMaterialColor(0.95f, 0.95f, 0.95f); glScalef(d3_len, d3_h, d3_wid); glutSolidCube(1.0f); glPopMatrix(); drawDeckWindows(d3_len, d3_wid, d3_h); glPopMatrix();
    drawBridge();
    setMaterialColor(0.8f, 0.2f, 0.0f); for(int k = -1; k <= 1; k++) { glPushMatrix(); glTranslatef((float)k * 1.2f, 1.1f, 0.0f); glRotatef(-90, 1.0f, 0.0f, 0.0f); GLUquadricObj *q = gluNewQuadric(); gluCylinder(q, 0.25f, 0.25f, 0.8f, 20, 20); glTranslatef(0.0f, 0.0f, 0.65f); setMaterialColor(0.1f, 0.1f, 0.1f); gluCylinder(q, 0.26f, 0.26f, 0.2f, 20, 20); setMaterialColor(0.8f, 0.2f, 0.0f); glPopMatrix(); }
    setMaterialColor(0.4f, 0.2f, 0.0f); glPushMatrix(); glTranslatef(-3.8f, 0.5f, 0.0f); glRotatef(-90, 1.0f, 0.0f, 0.0f); glutSolidCone(0.05f, 2.0f, 10, 10); glPopMatrix();
    setMaterialColor(1.0f, 0.0f, 0.0f); glPushMatrix(); glTranslatef(-3.8f, 2.0f, 0.0f); glScalef(0.6f, 0.4f, 0.05f); glTranslatef(-0.5f, 0.0f, 0.0f); glutSolidCube(1.0f); glPopMatrix();

    // --- DRAW PROPELLERS (Attached to Stern Box) ---
    // Left
    glPushMatrix(); glTranslatef(-3.2f, -0.65f, 0.25f); glScalef(0.35f, 0.35f, 0.35f); drawPropellerAssembly(); glPopMatrix();
    // Right
    glPushMatrix(); glTranslatef(-3.2f, -0.65f, -0.25f); glScalef(0.35f, 0.35f, 0.35f); drawPropellerAssembly(); glPopMatrix();

    // --- RUDDERS REMOVED ---

    glPopMatrix();
}

// --- COLLISION ---
bool checkCollision(float newX, float newZ) {
    float dx1=newX-0.0f, dz1=newZ-(-60.0f); if ((dx1*dx1+dz1*dz1) < (32.0f*32.0f)) return true;
    float dx2=newX-35.0f, dz2=newZ-(-50.0f); if ((dx2*dx2+dz2*dz2) < (20.0f*20.0f)) return true;
    float dx3=newX-(-30.0f), dz3=newZ-(-55.0f); if ((dx3*dx3+dz3*dz3) < (25.0f*25.0f)) return true;
    return false;
}

// --- UPDATE ---
void updateShip() {
    if (keyState['a'] || keyState['A']) shipRotation += turnSpeed;
    if (keyState['d'] || keyState['D']) shipRotation -= turnSpeed;
    float radianAngle = shipRotation * PI / 180.0f;
    float nextX = shipX, nextZ = shipZ;

    if (keyState['w'] || keyState['W']) {
        nextX += sin(radianAngle)*moveSpeed; nextZ += cos(radianAngle)*moveSpeed;
        updateWake();
        propAngle += 20.0f;
    }
    if (keyState['s'] || keyState['S']) {
        nextX -= sin(radianAngle)*moveSpeed; nextZ -= cos(radianAngle)*moveSpeed;
        propAngle -= 20.0f;
    }
    if (!checkCollision(nextX, nextZ)) { shipX = nextX; shipZ = nextZ; }
    updateWake();
}

// --- INPUT & DISPLAY ---
void keyboardDown(unsigned char key, int x, int y) { keyState[key] = true; if(key=='c'||key=='C') followCamera=!followCamera; }
void keyboardUp(unsigned char key, int x, int y) { keyState[key] = false; }
void specialKeys(int key, int x, int y) { switch (key) { case GLUT_KEY_RIGHT: angleY += 5.0f; break; case GLUT_KEY_LEFT: angleY -= 5.0f; break; case GLUT_KEY_UP: angleX += 5.0f; break; case GLUT_KEY_DOWN: angleX -= 5.0f; break; case GLUT_KEY_PAGE_UP: zoom -= 0.1f; break; case GLUT_KEY_PAGE_DOWN: zoom += 0.1f; break; } glutPostRedisplay(); }

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glLoadIdentity();
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float shipY = sin(shipX * 0.08f + time * 2.0f) * 0.6f + cos(shipZ * 0.09f + time * 0.8f) * 0.4f;

    if (followCamera) {
        float rad = shipRotation * PI / 180.0f;
        float camX = shipX + sin(rad) * 20.0f, camZ = shipZ + cos(rad) * 20.0f;
        gluLookAt(camX, shipY+7.0f, camZ, shipX, shipY+2.0f, shipZ, 0.0f, 1.0f, 0.0f);
    } else {
        glTranslatef(0.0f, -8.0f, -30.0f * zoom); glRotatef(angleX, 1.0f, 0.0f, 0.0f); glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    }

    glPushMatrix(); glLoadIdentity(); drawSun(); glPopMatrix();
    drawMountains();
    drawWake();
    glPushMatrix(); glTranslatef(shipX, shipY + 0.2f, shipZ); glRotatef(shipRotation, 0.0f, 1.0f, 0.0f); drawShipModel(); glPopMatrix();
    drawOcean();
    glutSwapBuffers();
}

void reshape(int w, int h) { if (h == 0) h = 1; float ratio = (float)w / h; glViewport(0, 0, w, h); glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(55.0f, ratio, 1.0f, 800.0f); glMatrixMode(GL_MODELVIEW); }
void timer(int value) { updateShip(); glutPostRedisplay(); glutTimerFunc(16, timer, 0); }

int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); glutInitWindowSize(1024, 768); glutCreateWindow("Cruise Ship - Ultimate");
    init(); glutDisplayFunc(display); glutReshapeFunc(reshape); glutKeyboardFunc(keyboardDown); glutKeyboardUpFunc(keyboardUp); glutSpecialFunc(specialKeys); glutTimerFunc(0, timer, 0); glutMainLoop(); return 0;
}
