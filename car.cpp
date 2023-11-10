#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include<bits/stdc++.h>
#include<SOIL/SOIL.h>

// Global variables used in functions
int proj = 1, new_mouse = 0, mouse_rot = 1, mouse_trn = 0, headlightsON = 0, leftDoorOpen = 0, rightDoorOpen = 0;

float turn = 50.0, tipp = 30.0, twst = 0.0, x_click = 0, y_click = 0, xtm = 0.0, ytm = 0.0,
      scl = 1.0, xcr = 0.0, rightAngle=0,  leftAngle=0, wrot = 0, wiper_angle = 0, theta = 0.0, angle = 15;

int animating_wipers = 0, animating_doors = 0;

const double PI = 3.1415926535897;

GLuint texture[5];

// Light attributes values
GLfloat lit1_spc[] = {0.20, 0.20,  0.15, 1.00},
        lit1_dfs[] = {0.70, 0.70,  0.65, 1.00},
		lit1_amb[] = {0.25, 0.25,  0.20, 1.00},
		lit1_pos[] = {0.20, 2.50,  5.20, 1.00};

GLfloat lit2_spc[] = { 0.00,  0.00,  0.00, 1.00},
        lit2_dfs[] = { 0.30,  0.30,  0.35, 1.00},
		lit2_amb[] = { 0.20,  0.20,  0.25, 1.00},
		lit2_pos[] = { 1.50, -2.00, -4.00, 0.01};

// -------------------------------------- wipers & wipers_animation ----------------------------------------------

// Function to update wiper angle
void update_wipers(int value) {
    if(animating_wipers==1 && value==0) {
        wiper_angle -= 1.0;
        glutPostRedisplay();
        if(wiper_angle==-120) glutTimerFunc(4.0, update_wipers, 1);
        else glutTimerFunc(4.0, update_wipers, 0);
    }
    else if(animating_wipers==1 && value==1) {
        wiper_angle += 1.0;
        glutPostRedisplay();
        if(wiper_angle==0) glutTimerFunc(4.0, update_wipers, 0);
        else glutTimerFunc(4.0, update_wipers, 1);
    }
}

// Function to draw wipers
void wipers() {
    glPushMatrix();
        glTranslatef(8, 10.2, 13.5);
        glRotatef(-30, 0, 0, 1);
        glRotatef(wiper_angle, 1, 0, 0);
        glTranslatef(0, 0, 2.5);
        glPushMatrix();
            glTranslatef(0, 0, 2.5);
            glRotatef(-15, 1, 0, 0);
            glTranslatef(0, 0, 2);
            glScalef(0.24, 0.24, 4);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glScalef(0.24, 0.24, 5);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(8, 10.2, -0.1);
        glRotatef(-30, 0, 0, 1);
        glRotatef(wiper_angle, 1, 0, 0);
        glTranslatef(0, 0, 2.5);
        glPushMatrix();
            glTranslatef(0, 0, 2.5);
            glRotatef(-15, 1, 0, 0);
            glTranslatef(0, 0, 2);
            glScalef(0.24, 0.24, 4);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glScalef(0.24, 0.24, 5);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

// ---------------------------------------- doors_animation ------------------------------------------

// Function to update the left door angle
void update_left_door(int value) {
    if(animating_doors) {
        // value -> 0 to open the door & 1 to close the door
        if(value==0) {
            leftAngle += 1.0;
            glutPostRedisplay();
            if(leftAngle >= 60) {
                animating_doors = !animating_doors;
            }
            else glutTimerFunc(1, update_left_door, 0);
        }
        else if(value==1) {
            leftAngle -= 1.0;
            glutPostRedisplay();
            if(leftAngle <= 0) {
                animating_doors = !animating_doors;
            }
            else glutTimerFunc(1, update_left_door, 1);
        }
    }
    
}

// Function to update the right door angle
void update_right_door(int value) {
    if(animating_doors) {
        // value -> 0 to open the door & 1 to close the door
        if(value==0) {
            rightAngle += 1.0;
            glutPostRedisplay();
            if(rightAngle >= 60) {
                animating_doors = !animating_doors;
            }
            else glutTimerFunc(1, update_right_door, 0);
        }
        else if(value==1) {
            rightAngle -= 1.0;
            glutPostRedisplay();
            if(rightAngle <= 0) {
                animating_doors = !animating_doors;
            }
            else glutTimerFunc(1, update_right_door, 1);
        }
    }
}

//----------------------------------------- keybord --------------------------------------------

// Function to handle keyboard input
void Keyboard (unsigned char ch, int x, int y) {

    switch (ch)  {
       case 'p' :  proj      = !proj;                                           break;
       case 'r' :  mouse_trn = 0;
                   mouse_rot = 1;                                               break;
       case 't' :  mouse_trn = 1;
                   mouse_rot = 0;                                               break;
       case 'z' :  turn = 0.0; tipp = 0.0;
                   xtm  = 0.0; ytm  = 0.0;
                   scl  = 1.0;                                                  break;
       case '-' :  scl *= 0.92;                                                 break;
       case '+' :  scl *= 1.06;                                                 break;
       case 'a' :  xcr += 2.00; wrot += 15;                                     break;
       case 'd' :  xcr -= 2.00; wrot -= 15;                                     break;
       case 'h' : headlightsON = !headlightsON;                                 break;
       case 'k' : animating_doors = !animating_doors;
                  if(leftDoorOpen) glutTimerFunc(0, update_left_door, 0);         
                  else glutTimerFunc(0, update_left_door, 1);
                  leftDoorOpen = !leftDoorOpen;           break;
       case 'l' : animating_doors = !animating_doors;
                  if(rightDoorOpen) glutTimerFunc(0, update_right_door, 0);         
                  else glutTimerFunc(0, update_right_door, 1);
                  rightDoorOpen = !rightDoorOpen;           break;
       case 'q' : animating_wipers = !animating_wipers; 
                  printf("%d", animating_wipers);
                  if (animating_wipers) glutTimerFunc(0, update_wipers, 0);    break;
    }
    glutPostRedisplay();
}

//------------------------------------------   Mouse   ---------------------------------------------

// This function is called when a mouse button is pressed or released
void Mouse (int button, int state, int x, int y) {
    // Store the current mouse coordinates in global variables
    x_click = x;
    y_click = y;

    new_mouse = 1;
}

//------------------------------------------   Motion   --------------------------------------------

// This function is called when the mouse is moved
void Motion (int x, int y) {
    float turn_del, tipp_del, x_del, y_del;

    static int c = 0, x_sav, y_sav;
    static float turn_save = 0.0, tipp_save = 0.0;

    if (new_mouse)  {
       new_mouse = 0;
       x_sav = x_click;
       y_sav = y_click;
    }

    if (mouse_rot)  {

       turn_del = x - x_sav;
       turn += turn_del;

       tipp_del = y - y_sav;
       tipp += tipp_del;

    }  else if (mouse_trn)  {

       x_del = float(x - x_sav) * 0.01;
       xtm  += x_del;

       y_del = float(y - y_sav) * 0.01;
       ytm  -= y_del;

    }

    x_sav = x;
    y_sav = y;

    glutPostRedisplay();
}

//---------------------------------------   Init_Lights   ------------------------------------------

// This function sets up the lighting for the scene
void Init_Lights (void) {
    glLightfv (GL_LIGHT1, GL_SPECULAR, lit1_spc);
    glLightfv (GL_LIGHT1, GL_DIFFUSE , lit1_dfs);
    glLightfv (GL_LIGHT1, GL_AMBIENT , lit1_amb);
    glLightfv (GL_LIGHT1, GL_POSITION, lit1_pos);

    glLightfv (GL_LIGHT2, GL_SPECULAR, lit2_spc);
    glLightfv (GL_LIGHT2, GL_DIFFUSE , lit2_dfs);
    glLightfv (GL_LIGHT2, GL_AMBIENT , lit2_amb);
    glLightfv (GL_LIGHT2, GL_POSITION, lit2_pos);

    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHT2);

    glEnable (GL_AUTO_NORMAL);
    glEnable (GL_NORMALIZE  );
}

// ---------------------------------------- SolidCylinder ------------------------------------------

void draw_cylinder(GLfloat radius,
                   GLfloat height)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();

    /** Draw the circle on the bottom of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, 0.0);
    glEnd();
}

//----------------------------------------   Draw_Road   -------------------------------------------

void Draw_Road (void) {
    glColor3f (0.30,0.25,0.30);
    glEnable (GL_LIGHTING);

    glPushMatrix();
       glTranslatef (0.0, -23.0, 0.0);
       glScalef     (10.0, 0.1,10.0);
       glutSolidCube (-20.0f);
    glPopMatrix();
}

// ---------------------------------------- Draw Gear Box ------------------------------------------

void Draw_Gear_Box (void) {

    glScalef(0.75,0.75,0.75);

    glPushMatrix();
       glTranslatef (0.0, 5.0, 0.0);
       glScalef     (0.125,0.125,0.125);
       glutSolidSphere (10.0, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
       glTranslatef (0.0, 5.0, 0.0);
       glScalef     (0.5, 0.5, 0.5);
       glRotatef    (90, 1.0, 0.0, 0.0);
       gluCylinder  (gluNewQuadric(), 1.0, 1.0, 10.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
       glTranslatef (0.0, 0.0, 0.0);
       glScalef     (0.25, 0.1, 0.25);
       glutSolidCube (-20.0f);
    glPopMatrix();
}
 
//---------------------------------------   Render_Wheel   -----------------------------------------

void Render_Wheel (int xt, int yt, int zt) {
    glPushMatrix();
    glPointSize(5);
    glLineWidth(4);
    glBegin(GL_LINE_STRIP);
    	glColor3f(1,1,1);
        for(theta=0;theta<360;theta=theta+45)
        {
            glVertex3f(xt,yt,zt);
            glVertex3f(xt+(5*(cos(((theta+angle)*3.14)/180))),yt+(5*(sin(((theta+angle)*3.14)/180))),zt);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    	glColor3f(0,0,0);
        glTranslatef   (xt, yt, zt);
        glutSolidTorus (2,5,12,36);
    glPopMatrix();
    
}

// ---------------------------------------- Draw Surface   ------------------------------------------
GLfloat triangleVertices[150];

void smc1(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    float dx=1.6,dy=0,dz=0;
    float sx=0,sy=0.8,sz=0.8;
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 25; i++) {
        float angle = 2 * M_PI * i;
        float x = sx;
        float y = sy * sin(angle/50);
        float z = sz * cos(angle/50);

        glVertex3f(x, y, z);
        glVertex3f(x+dx, y+dy, z+dz);
    }
    glEnd();
    

}

void smc2(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glColor3f(1, 1, 0);
    float dx=0,dy=0,dz=1.6;
    float sx=0.8,sy=0.8,sz=0;

}

class CurvedSurface
{
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

public:
    CurvedSurface(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        
        float x,y,z;
        for(r=0;r<rings;r++)for(s = 0; s < sectors; s++) {
 
                if(cos(M_PI * r * R )>0) y = radius * cos(M_PI * r * R );
                else y=0;
                x = radius * cos(2*M_PI * s * S) * sin( M_PI * r * R );
                z = radius * sin(2*M_PI * s * S) * sin( M_PI * r * R );

                if(x>radius/2) x = radius/2;
                if(x<-radius/2) x = -radius/2;
                if(z>radius/2) z = radius/2;
                if(z<-radius/2) z = -radius/2;            

                *v++ = x ;
                *v++ = y ;
                *v++ = z ;

                *n++ = x;
                *n++ = y;
                *n++ = z;
        }

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings/4; r++) for(s = 0; s < sectors; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
        }
    }

    void draw()
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(0,-5/sqrt(2),0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
    }
};

CurvedSurface sf1(5, 48, 48);

class CurvedSurface2
{
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

public:
    CurvedSurface2(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        
        float x,y,z;
        for(r=0;r<rings;r++)for(s = 0; s < sectors; s++) {
 
                if(cos(M_PI * r * R )>0) y = radius * cos(M_PI * r * R );
                else y=0;
                x = radius * cos(2*M_PI * s * S) * sin( M_PI * r * R );
                z = radius * sin(2*M_PI * s * S) * sin( M_PI * r * R );

                if(x>radius/2) x = radius/2;
                if(x<-radius/2) x = -radius/2;
                if(z>radius/2) z = radius/2;
                if(z<-radius/2) z = -radius/2;            

                *v++ = x ;
                *v++ = y ;
                *v++ = z ;

                *n++ = x;
                *n++ = y;
                *n++ = z;
        }

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings/4; r++) for(s = 0; s < sectors/2; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
        }
    }

    void draw()
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(0,-5/sqrt(2),0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
    }
};

CurvedSurface2 sf2(5, 48, 48);

void jointsurface(GLfloat x, GLfloat y, GLfloat z, GLfloat m){
    glPushMatrix();
        glScalef (x,y,z);
        sf2.draw();
        glRotatef(180,0,1,0);
        glScalef (1,1,m);
        sf2.draw();
    glPopMatrix();
}

// --------------------------------- windshield -------------------------------------

void wind_shield() {
    glPushMatrix();
       glTranslatef (11, 12, 10);
       glRotatef(-30, 0, 0, 1);
       glScalef(1, 6, 15);
       glLineWidth(2);
       glBegin(GL_LINES);
       glVertex3f( -1, -1,  1 );
       glVertex3f( -1,  1,  1 );
       glVertex3f( -1,  1,  1 );
       glVertex3f( -1,  1, -1 );
       glVertex3f( -1,  1, -1 );
       glVertex3f( -1, -1, -1 );
       glVertex3f( -1, -1, -1 );
       glVertex3f( -1, -1,  1 );
       glEnd();

    glPopMatrix();
}

//---------------------------------------deck------------------------------------

void deck() {
	glPushMatrix();
       glTranslatef (7.5, 5, 10);
       glRotatef(90, 0, 1, 0);
       glScalef(5.75, 3, 3);
       sf2.draw();
    glPopMatrix();
}

// ---------------------------------------- Draw Seat   ------------------------------------------

void chair() {
    glPushMatrix();
       glTranslatef (35.0, 9, 10);
       glRotatef(-25, 0, 0, 1);
       glScalef(1, 8, 26);
       glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(34.5, 8.5, 10);
        glRotatef(65, 0, 0, 1);
        glScalef(2, 1.25, 5.3);
        sf1.draw();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(30.1, 6, 10);
        glScalef(2, 1.2, 5.3);
        sf1.draw();
    glPopMatrix();

    glPushMatrix();
       glTranslatef (30.0, 3, 10);
       glScalef     (10.0, 6.0, 26.0);
       glutSolidCube (1.0f);
    glPopMatrix();

}

// ---------------------------------------- steering wheel ------------------------------------

void steering_wheel() {
    glPushMatrix();
        glTranslatef(14.5, 1.5+7, 5);
        glRotatef(60, 0, 0, 1);
        glRotatef(90, 0, 1, 0);
        gluCylinder(gluNewQuadric(), 0.4, 0.4, 3, 50, 50);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(16, 4.1+7, 5);
        glRotatef(60, 0, 0, 1);
        glRotatef(90, 0, 1, 0);
        glutSolidTorus(0.5, 3.4, 12, 36);
    glPopMatrix();
           
}

//------------------------------------- Accelarator - Break - Clutch -------------------------------------

void ABC() {
    glPushMatrix();
        glTranslatef(11, 2, 3);
        glPushMatrix();
            glPushMatrix();
                glTranslatef(0, 0, 0);
                glPushMatrix();
                    glRotatef(60, 0, 0, 1);
                    glRotatef(90, 0, 1, 0);
                    gluCylinder(gluNewQuadric(), 0.3, 0.3, 1.6, 50, 50);
                glPopMatrix();
                
                glPushMatrix();
                    glTranslatef(0.83, 1.462, 0);
                    glRotatef(-30, 0, 0, 1);
                    glScalef(2.3, 0.5, 1.6);
                    glutSolidCube(1);
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0, 0, 3);
                glPushMatrix();
                    glRotatef(60, 0, 0, 1);
                    glRotatef(90, 0, 1, 0);
                    gluCylinder(gluNewQuadric(), 0.3, 0.3, 1.6, 50, 50);
                glPopMatrix();
                
                glPushMatrix();
                    glTranslatef(0.83, 1.462, 0);
                    glRotatef(-30, 0, 0, 1);
                    glScalef(2.3, 0.5, 1.6);
                    glutSolidCube(1);
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0, 0, 6);
                glPushMatrix();
                    glRotatef(60, 0, 0, 1);
                    glRotatef(90, 0, 1, 0);
                    gluCylinder(gluNewQuadric(), 0.3, 0.3, 1.6, 50, 50);
                glPopMatrix();
                
                glPushMatrix();
                    glTranslatef(0.83, 1.462, 0);
                    glRotatef(-30, 0, 0, 1);
                    glScalef(2.3, 0.5, 1.6);
                    glutSolidCube(1);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// --------------------------------------- Windshield-glass --------------------------------------------

void glass() {
    const float mirdiff[] = {1.0,1.0,1.0,0.5};
    glPushMatrix();
        glColor4f(0.0, 0.0, 0.0, 0.15);
        glTranslatef(10, 12.5, 10);
        glRotatef(-30, 0, 0, 1);
        glScalef(0.01, 12, 30);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mirdiff);
        glutSolidCube(1);
    glPopMatrix();
}

// ----------------------------------------- Car Body --------------------------------------

void body() {

    glPushMatrix();
        glTranslatef(0,8.25,10);
        glRotatef(90,0,1,0);
        jointsurface(6, 2.5, 3, 2);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-14.5,4,10);
        glScalef(1, 8, 30);
        glutSolidCube(1.0f);
    glPopMatrix();



    // Left gate
    glPushMatrix();
        glPushMatrix();
            glTranslatef(0, 4, 24.5);
            glScalef(30, 8, 1);
            glutSolidCube(1.0f);
        glPopMatrix();


        glPushMatrix();
            glTranslatef(15, 4, 24.5); 
            glRotatef(-leftAngle, 0, 1, 0);
            glTranslatef(7.5, 0, 0);
            glScalef(15, 8, 1);
            glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(15, 4, 24.5); 
            glRotatef(-leftAngle, 0, 1, 0);
            glTranslatef(11, 2.1, 1);
            glRotatef(180, 0, 1, 0);
            glRotatef(20, 1, 0, 0);
            glRotatef(-90, 1, 0, 0);
            glScalef(0.8, 0.9, 0.5);
            sf2.draw();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(40, 4, 24.5);
            glScalef(20, 8, 1);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();

    // Right gate
    glPushMatrix();
        glPushMatrix();
            glTranslatef(0, 4, -4.5);
            glScalef(30, 8, 1);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(15, 4, -4.5); 
            glRotatef(rightAngle, 0, 1, 0);
            glTranslatef(7.5, 0, 0);
            glScalef(15, 8, 1);
            glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(15, 4, -4.5); 
            glRotatef(rightAngle, 0, 1, 0);
            glTranslatef(11, 2.1, -0.7);
            glRotatef(20, 1, 0, 0);
            glRotatef(-90, 1, 0, 0);
            glScalef(0.8, 0.9, 0.5);
            sf2.draw();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(40, 4, -4.5);
            glScalef(20, 8, 1);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(42.5,8.5, 10);
        glScalef(15, 1, 30);
        glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(49.5, 4, 10);
        glScalef(1, 8, 30);
        glutSolidCube(1.0f);
    glPopMatrix();

}

// ----------------------------------- headlights -----------------------------------

void headlights(){
    glPushMatrix();
        if(headlightsON == 1) {
            glColor3f(1, (float)195/256, 0);
        }
        glTranslatef(-16, 5, 2);
        glScalef(2, 2, 2);
        glRotatef(90,0,1,0);
        draw_cylinder(1, 1);
    glPopMatrix();

    glPushMatrix();
        if(headlightsON == 1) {
            glColor3f(1, (float)195/256, 0);
        }
        glTranslatef(-16, 5, 18);
        glScalef(2, 2, 2);
        glRotatef(90,0,1,0);
        draw_cylinder(1, 1);
    glPopMatrix();

} 

// ------------------------------------- side view mirrors ------------------------------------

void sideViewMirrors() {
    glPushMatrix();
        glTranslatef(10, 10, 26);
        glRotatef(15, 0, 1, 0);
        glRotatef(90, 0, 0, 1);
        glScalef(4, 1, 3);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(10, 10, -6);
        glRotatef(-15, 0, 1, 0);
        glRotatef(90, 0, 0, 1);
        glScalef(4, 1, 3);
        glutSolidCube(1);
    glPopMatrix();
}

// ------------------------------------- logo -----------------------------------------

void logo() {
    glPushMatrix();
        glutSolidTorus(0.1, 0.75, 12, 36);
    glPopMatrix();

    glPushMatrix();
        glPushMatrix();
            glTranslatef(0, 0.075, 0);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(gluNewQuadric(), 0.06, 0.06, 0.74, 50, 50);
        glPopMatrix();
        glPushMatrix();
            glRotatef(120, 0, 0, 1);
            glTranslatef(0, 0.075, 0);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(gluNewQuadric(), 0.06, 0.06, 0.74, 50, 50);
        glPopMatrix();
        glPushMatrix();
            glRotatef(240, 0, 0, 1);
            glTranslatef(0, 0.075, 0);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(gluNewQuadric(), 0.06, 0.06, 0.74, 50, 50);
        glPopMatrix();
    glPopMatrix();
}

// ------------------------------------- mesh ----------------------------------------------

void mesh() {
    glBegin(GL_LINES);

    glVertex3f(-16, 3, 5);
    glVertex3f(-16, 7, 5);
    glVertex3f(-16, 3, 7);
    glVertex3f(-16, 7, 7);
    glVertex3f(-16, 3, 9);
    glVertex3f(-16, 7, 9);
    glVertex3f(-16, 3, 11);
    glVertex3f(-16, 7, 11);
    glVertex3f(-16, 3, 13);
    glVertex3f(-16, 7, 13);
    glVertex3f(-16, 3, 15);
    glVertex3f(-16, 7, 15);


    glVertex3f(-16, 3, 5);
    glVertex3f(-16, 3, 15);
    glVertex3f(-16, 7, 5);
    glVertex3f(-16, 7, 15);

    glEnd();
}

// --------------------------------------- Load Texture -----------------------------


int LoadGLTextures(){
    texture[2] = SOIL_load_OGL_texture ( "grass.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);
    if(texture[0] == 0 ){
       return false;
    }
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    

    glEnable(GL_TEXTURE_2D);                // Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);                // Enable Smooth Shading
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Black Background
    glClearDepth(1.0f);                     // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
    return true;                            // Return Success
}

// ---------------------------------------- Number Plate -----------------------------

void num_plate() {
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex3f(51, 3, 2);
    glVertex3f(51, 7, 2);
    glVertex3f(51, 7, 18);
    glVertex3f(51, 3, 18);
    glEnd();

    glColor3f(0,0,0);
    glTranslatef(0,-0.5,0);
    glRasterPos3f(51.5, 5, 12);
    int len, i;
    char s[] = "MP 1234";
    len = (int)strlen(s);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, s[i]);
    }
    glPopMatrix();
}

//-----------------------------------------   Draw_Car   -------------------------------------------

// Main function which assembles all the components of the cars
void Draw_Car (void) {
    glTranslatef (0, 0, 50);

    glEnable (GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    
    // Defining some material properties for better lighting effect
    GLfloat mat_specular[] = { 0.7, 0.8, 0.8, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glShadeModel (GL_SMOOTH);

    // Base
    glColor3f (0.3, 0.3, 0.3);
    glPushMatrix();
        glEnable (GL_LIGHTING);
        glTranslatef (17.5, 0.4, 10.0);
        glScalef     (64.0, 1, 29.0);
        glutSolidCube (1);
    glPopMatrix();

    
     // Axles
    glPushMatrix();
        glTranslated(40,0.5,-5);
        gluCylinder (gluNewQuadric(), 1, 1, 30, 36, 1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0,0.5,-5);
        gluCylinder (gluNewQuadric(), 1, 1, 30, 36, 1);
    glPopMatrix();

      // Gear Box
    glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(18,1.3,12.5);
        Draw_Gear_Box();
    glPopMatrix();

    // Render wheels using torus's with lighting on.
    glPushMatrix();
        glRotatef(wrot, 0, 0, 1);
        Render_Wheel (0, 0.5, 25);
        Render_Wheel (0, 0.5,-5);
    glPopMatrix();

    glPushMatrix();
        if(wrot!=0) {
            glTranslatef(40,0,0);glRotatef(wrot, 0, 0, 1);glTranslatef(-40,0,0);Render_Wheel (40,0.5, 25);
            Render_Wheel (40,0.5,-5);
        }
        else {
            Render_Wheel (40,0.5, 25);
            Render_Wheel (40,0.5,-5);
        }
    glPopMatrix();
      

     // Render the chair
    glPushMatrix();
        glColor3f((float)196/256, (float)164/256, (float)132/256);
        chair();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-15, 10.8, 10);
        glRotatef(90, 0, 1, 0);
        logo();
    glPopMatrix();
     
    glPushMatrix();
        glPushMatrix();
        glTranslatef(0, -2, 0);
        steering_wheel();
        glPopMatrix();
        deck();
    	glColor3f((float)200/255,0,0);
    	glEnable (GL_LIGHTING);
        body();
        wind_shield();
        sideViewMirrors();

        glColor3f(0,0,0);
        
        glColor3f(1,1,1);
        ABC();
        
        glColor3f(1,1,1);
        headlights();

        num_plate();
        mesh();    
    glPopMatrix();

    glPushMatrix();
        wipers();
        glass();
    glPopMatrix();
    glTranslatef (0, 0, -50);
}

//------------------------------------------ Final Mirror ----------------------------------------
void mirror() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearStencil(0x0);

    const GLfloat mat_diff[] = {0.5,0.5,0.5,1};
    const float mirror_diff[] = {1.0,1.0,1.0,0.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);

    const GLfloat mat_spec[] = {1,1.0,0.0,1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec); 
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 90.0);

    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);

    glPushMatrix();
        glTranslatef(-60,16,13);
        glRotatef(-90,0,0,1);
        glScalef(0.0625,0.125,1);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            glVertex3f(20, 20, 0);
            glVertex3f(-20, 20, 0);
            glVertex3f(-20, -20, 0);
            glVertex3f(20, -20, 0);
        glEnd();
        glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
    glPopMatrix();
    
    glPushMatrix();
        glScalef(-1.0,1.0,1.0);
        glEnable(GL_COLOR_MATERIAL);
        Draw_Road();
        glRotatef(90,0,1,0);
        Draw_Car();
        glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    
    glClear(GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glTranslatef(-60,16,13);
        glRotatef(-90,0,0,1);
        glScalef(0.0625,0.125,1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            glVertex3f(20, 20, 0);
            glVertex3f(-20, 20, 0);
            glVertex3f(-20, -20, 0);
            glVertex3f(20, -20, 0);
        glEnd();
        glStencilFunc(GL_ALWAYS, 2, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glPopMatrix();


    glPushMatrix();        
        glPushMatrix();
            glScalef(-1.0,1.0,1.0);
            glEnable(GL_COLOR_MATERIAL);
            Draw_Road();
            glRotatef(90,0,1,0);
            Draw_Car();
            glDisable(GL_COLOR_MATERIAL);
        glPopMatrix();
    glPopMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  back

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    
    glPushMatrix();
        glScalef(-1.0,1.0,1.0);
        glEnable(GL_COLOR_MATERIAL);
        Draw_Road();
        glRotatef(90,0,1,0);
        Draw_Car();
        glDisable(GL_COLOR_MATERIAL);

    glPopMatrix();
    
    //  front

    glStencilFunc(GL_EQUAL, 2, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    
    glPushMatrix();
        glPushMatrix();
            glEnable(GL_COLOR_MATERIAL);
            glDisable(GL_COLOR_MATERIAL);
        glPopMatrix();
    glPopMatrix();

    glDisable(GL_STENCIL_TEST);

    glPushMatrix();
        glTranslatef(-60,16,13);
        glRotatef(-90,0,0,1);
        glScalef(0.0625,0.125,1);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mirror_diff);
        glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            glVertex3f(20, 20, 0);
            glVertex3f(-20, 20, 0);
            glVertex3f(-20, -20, 0);
            glVertex3f(20, -20, 0);
        glEnd();
        float cover_diff[] = {0.0,0.,0.0,1.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cover_diff);
        glBegin(GL_QUADS);
            glNormal3f(0,0,1);
            glVertex3f(30, 30, -0.1);
            glVertex3f(-20, 30, -0.1);
            glVertex3f(-20, -30, -0.1);
            glVertex3f(30, -30, -0.1);
        glEnd();
    glPopMatrix();

}

//------------------------------------------   Poster's Display   ----------------------------------

void Display (void) {
    static float x = 2.0;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode   (GL_PROJECTION);
    glLoadIdentity ();

    if (proj == 0)  glOrtho        (-x, x, -x, x, 0.0, 10.0);
    else            gluPerspective ( 60.0,  1.0,  1.0, 10.0);

    glMatrixMode   (GL_MODELVIEW);
    glLoadIdentity ();
    
    glTranslatef (xtm, ytm, -4.0);       // Place objects between near and far clipping planes.
    glRotatef    (tipp,  1, 0, 0);       // Rotate around screen horizontal axis.
    glRotatef    (turn,  0, 1, 0);       // Rotate around world    vertical axis.
    glLightfv (GL_LIGHT1, GL_POSITION, lit1_pos);
    glLightfv (GL_LIGHT2, GL_POSITION, lit2_pos);
    glScalef     (scl, scl, scl );       // Scale world relative to its origin.
    glScalef     (0.02, 0.02, 0.02);
    
    glPushMatrix();
        glTranslatef(-xcr,0,0);
        glTranslatef(0,-15,-50);
        glRotatef(90,0,1,0);
        mirror();
    glPopMatrix();
    glEnable(GL_COLOR_MATERIAL);

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    Draw_Road();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glTranslatef (-xcr, 0.0, 0.0);
    glTranslatef(0,-15,-50);
    Draw_Car();

    glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(0, 0, w, h);
    float ratio = 1.0 * w / h;
    gluPerspective(50.0, ratio, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//-----------------------------------------   Init_GL   --------------------------------------------

void Init_GL (void) {
    Init_Lights ();

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING  );
    glEnable (GL_COLOR_MATERIAL);

    glColorMaterial (GL_AMBIENT_AND_DIFFUSE, GL_FRONT);
    glColorMaterial (GL_SPECULAR           , GL_FRONT);

    glClearColor (0.2, 0.3, 0.3, 1.0);
    glClearColor (1, 1, 1, 1.0);

}

//----------------------------------------   Init_Glut   -------------------------------------------

void Init_Glut (void) {
    glutInitDisplayMode    (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
    glutInitWindowPosition (800, 200);
    glutInitWindowSize     (800, 800);
    glutCreateWindow       ("Carmine's Car On Road");
    
    glutKeyboardFunc (Keyboard);
    glutDisplayFunc  (Display);

    glutMouseFunc    (Mouse  );
    glutMotionFunc   (Motion );
}

// ----------------------------------------- Init_Mirrors ------------------------------------------
void Init_Mirror(void) {
  
  Init_Lights ();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  

  glMatrixMode(GL_PROJECTION);
  gluPerspective(50.0, 1.0, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);

}

//------------------------------------------   main   ----------------------------------------------

int main (int argc, char **argv)
{
    glutInit (&argc, argv);

    Init_Glut ();
    Init_GL   ();
    Init_Mirror();
    LoadGLTextures();
    glutMainLoop ();

    return (1);
}