////////////////////////////////////////////////////////
//  Spring 2018
// Lab 2
// <ROSTOM>

////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// produit vectoriel a = b x c
#define cross(a,b,c) \
	(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
	(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
	(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];

// produit de vecteur par une constante w = a * v
#define scale(w,a,v) \
  (w)[0] = (a) * (v)[0]; \
  (w)[1] = (a) * (v)[1]; \
  (w)[2] = (a) * (v)[2];

// produit de vecteur par une constante et addition:  w = w + a * v
#define saxpy(w,a,v) \
  (w)[0] += (a) * (v)[0]; \
  (w)[1] += (a) * (v)[1]; \
  (w)[2] += (a) * (v)[2];

// a = b + c
#define add(a,b,c) \
  (a)[0] = (b)[0] + (c)[0]; \
  (a)[1] = (b)[1] + (c)[1]; \
  (a)[2] = (b)[2] + (c)[2];

// a = b - c
#define sub(a,b,c) \
  (a)[0] = (b)[0] - (c)[0]; \
  (a)[1] = (b)[1] - (c)[1]; \
  (a)[2] = (b)[2] - (c)[2];

//global  plain vs line mode drawing
char pm = 1;
double a=0;
int LightPos[4] = {1,1,0,1};
int MatSpec [4] = {1,1,1,1};

//
GLuint roof;
GLuint grass;
GLuint wall;
GLuint window;
GLuint door;

//gluPerspective default parameters
GLfloat fovy = 60.0;
GLfloat aspect = 1.0;
GLfloat near = 2.0f;
GLfloat far = -2.0f;

// gluLookAt parameters
// camera (x, y, z) position
GLfloat pos[3] = {0.0f, 0.0f, 150.0f};

// camera (x, y, z) forward direction
GLfloat coi[3] = {0.0f, 0.0f, 0.0f};

// camera (x, y, z) up direction
GLfloat up[3] = {0.0f, 1.0f, 0.0f};

// Initialize OpenGL's rendering modes

void draw_cube()
{
    glutSolidCube(1.0);
}

void draw_pyramid( GLfloat* texcoord )
{
   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles

   // front
   if(NULL != texcoord) glTexCoord2fv(texcoord); 
   glVertex3f( 0.0f,  0.5f, 0.0f);   // top

   if(NULL != texcoord) glTexCoord2fv(texcoord+2);    
   glVertex3f( 0.5f, -0.5f, 0.5f);   // right

   if(NULL != texcoord) glTexCoord2fv(texcoord+4);    
   glVertex3f(-0.5f, -0.5f, 0.5f);   // left

   // right
   if(NULL != texcoord) glTexCoord2fv(texcoord);    
   glVertex3f( 0.0f,  0.5f, 0.0f);   // top

   if(NULL != texcoord) glTexCoord2fv(texcoord+2);    
   glVertex3f( 0.5f, -0.5f, 0.5f);   // near

   if(NULL != texcoord) glTexCoord2fv(texcoord+4);    
   glVertex3f( 0.5f, -0.5f,-0.5f);   // far


   // back
   if(NULL != texcoord) glTexCoord2fv(texcoord);    
   glVertex3f( 0.0f,  0.5f, 0.0f);   // top

   if(NULL != texcoord) glTexCoord2fv(texcoord+2);    
   glVertex3f( 0.5f, -0.5f,-0.5f);   // right

   if(NULL != texcoord) glTexCoord2fv(texcoord+4);    
   glVertex3f(-0.5f, -0.5f,-0.5f);   // left

   // left
   if(NULL != texcoord) glTexCoord2fv(texcoord); 
   glVertex3f( 0.0f,  0.5f, 0.0f);   // top

   if(NULL != texcoord) glTexCoord2fv(texcoord+2);    
   glVertex3f(-0.5f, -0.5f, 0.5f);   // near

   if(NULL != texcoord) glTexCoord2fv(texcoord+4);    
   glVertex3f(-0.5f, -0.5f,-0.5f);   // far

   glEnd();   // Done drawing the pyramid
}

void draw_body(GLfloat* texcoord)
{
    glBegin(GL_QUADS);                     // draw a quadrilateral

//front
    if(NULL != texcoord) glTexCoord2fv(texcoord); 
    glVertex3f(-0.5f, -0.5f, 0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 2); 
    glVertex3f( 0.5f, -0.5f, 0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 4); 
    glVertex3f( 0.5f,  0.5f, 0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 6); 
    glVertex3f(-0.5f,  0.5f, 0.5f);

//right
    if(NULL != texcoord) glTexCoord2fv(texcoord); 
    glVertex3f(0.5f, -0.5f,  0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 2); 
    glVertex3f(0.5f, 0.5f,  0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 4); 
    glVertex3f(0.5f, 0.5f, -0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 6); 
    glVertex3f(0.5f, -0.5f, -0.5f);

//left
    if(NULL != texcoord) glTexCoord2fv(texcoord); 
    glVertex3f(-0.5f, -0.5f, -0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 2); 
    glVertex3f(-0.5f, -0.5f,  0.5f);
    

    if(NULL != texcoord) glTexCoord2fv(texcoord + 4); 
    glVertex3f(-0.5f, 0.5f,  0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 6); 
    glVertex3f(-0.5f, 0.5f, -0.5f);

//back
    if(NULL != texcoord) glTexCoord2fv(texcoord); 
    glVertex3f(-0.5f,  0.5f, -0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 2); 
    glVertex3f( 0.5f,  0.5f, -0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 4); 
    glVertex3f( 0.5f, -0.5f, -0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 6); 
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glEnd();
}


void draw_square(GLfloat* texcoord)
{
    glBegin(GL_QUADS);                     // draw a quadrilateral

    if(NULL != texcoord) glTexCoord2fv(texcoord); 
    glVertex3f(-0.5f, -0.5f, 0.0f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 2); 
    glVertex3f( 0.5f, -0.5f, 0.0f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 4); 
    glVertex3f( 0.5f,  0.5f, 0.0f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 6); 
    glVertex3f(-0.5f,  0.5f, 0.0f);

    glEnd();
}

void draw_land(GLfloat* texcoord)
{
    glBegin(GL_QUADS);                     // draw a quadrilateral

    if(NULL != texcoord) glTexCoord2fv(texcoord); 
    glVertex3f(-0.5f, 0.0f, -0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 2); 
    glVertex3f( 0.5f, 0.0f, -0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 4); 
    glVertex3f( 0.5f, 0.0f, 0.5f);

    if(NULL != texcoord) glTexCoord2fv(texcoord + 6); 
    glVertex3f(-0.5f, 0.0f, 0.5f);

    glEnd();
}




void draw_triangle( GLfloat* texcoord )
{
   glBegin(GL_TRIANGLES);

   if(NULL != texcoord) glTexCoord2fv(texcoord); 
   glVertex2f( 0.0f,  0.5f);

   if(NULL != texcoord) glTexCoord2fv(texcoord + 2); 
   glVertex2f( 0.5f, -0.5f);

   if(NULL != texcoord) glTexCoord2fv(texcoord + 4); 
   glVertex2f(-0.5f, -0.5f);

   glEnd();
}


void draw_lines()
{
    glLineWidth(1.0f);
    glBegin(GL_LINES);                     // draw a quadrilateral
    glVertex3f(-0.5f,  0.0f, 0.0f);
    glVertex3f( 0.5f,  0.0f, 0.0f);
    glVertex3f( 0.0f, -0.5f, 0.0f);
    glVertex3f( 0.0f,  0.5f, 0.0f);
    glEnd();
}

// void draw_land()
// {
//     glLineWidth(10.0f);
//     glBegin(GL_LINES);                     // draw a quadrilateral
//     glVertex2f(-0.5f,  0.0f);
//     glVertex2f( 0.5f,  0.0f);
//     glEnd();
// }


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );          // the color will be cleared

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);	// Active l'?lairage
 	glEnable(GL_LIGHT0);

    glLoadIdentity();
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    gluLookAt( pos[0], pos[1], pos[2], coi[0], coi[1], coi[2], up[0], up[1], up[2]);

//---------------------------------------------------------------------------------
// draw the sceen
//---------------------------------------------------------------------------------

// global translation of the sceen
    glTranslatef(40.0f,5.0f,0.0f);

    // draw the house body
    GLfloat walltexcoord[] = {  0.0, 0.0, 3*1.0, 0.0, 3*1.0, 3*1.0, 0.0, 3*1.0 };
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wall);
    
    glPushMatrix();
    glScalef(40.0f,60.0f,30.0f);     
    glTranslatef(0.0f,0.5f,-0.5f);   
    glColor3f(0.0,1.0,0.0); 
//    draw_square();
    // draw_cube();
    draw_body(walltexcoord);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

// draw the door
    GLfloat doortexcoord[] = {  0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, door);

    glPushMatrix();
    glScalef(11.0f,20.0f,1.0f);
    glTranslatef(0.0f,0.5f,0.01f);

    glColor3f(1.0f,0.0f,0.0f);
    draw_square(doortexcoord);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

// draw the roof
    GLfloat rooftexcoord[] = { 2*0.5, 2*1.0, 2*1.0, 0.0, 0.0, 0.0 };
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roof);
    glPushMatrix();
    glTranslatef(0.0f,60.0f,0.0f);
    glScalef(40.0f,30.0f,30.0f); // z 1.0 -> 30.0
    glTranslatef(0.0f,0.5f,-0.5f); // 0.0 -> -0.5

    glColor3f(0.0f,1.0f,1.0f);
//    draw_triangle();
    draw_pyramid( rooftexcoord );
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

// draw the left window
    GLfloat wind1texcoord[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, window);
    glPushMatrix();
    glTranslatef(-10.0f,35.0f,0.0f);
    glScalef(10.0f,10.0f,1.0f);
    glTranslatef(0.0f,0.5f,0.01f);
    glColor3f(1.0f,1.0f,1.0f);
    draw_square(wind1texcoord);
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,0.01f);
    draw_lines();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

// draw the right window
    GLfloat wind2texcoord[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, window);
    glPushMatrix();
    glTranslatef( 10.0f,35.0f,0.0f);
    glScalef(10.0f,10.0f,1.0f);
    glTranslatef(0.0f,0.5f,0.01f);
    glColor3f(1.0f,1.0f,1.0f);
    draw_square(wind2texcoord);
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,0.01f);
    draw_lines();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

// draw the land
    GLfloat grasstexcoord[] = { 0.0, 0.0, 3*1.0, 0.0, 3*1.0, 3*1.0, 0.0, 3*1.0 };
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grass);
    glPushMatrix();
    glTranslatef(0.0f,0.5f,0.0f);
    glScalef(120.0f,1.0f,120.0f);
//    glTranslatef(0.0f,0.5f,0.0f);
    glColor3f(0.5f,0.5f,0.5f);
//    draw_land();
//    draw_cube();
    draw_land(grasstexcoord);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

// draw the tree trunk
    glPushMatrix();
    glTranslatef(50.0f,0.0f,0.0f);
    glScalef(3.0f,50.0f,1.0f);
    glTranslatef(0.0f,0.5f,-0.5f);
    glColor3f(0.0,1.0,0.0);             // set the drawing color
    draw_square(NULL);
    glPopMatrix();
    //initRendering(10,10,3, 0, 1, 1);

    glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,100);
		


	glLightiv(GL_LIGHT0,GL_POSITION,LightPos);
	gluLookAt(0,6,6,0,0,0,0,1,0);
	//glRotated(a,0,1,0);
	glutSolidSphere(1,50,50);
	a=+1;
	glutSwapBuffers();
	
	glutPostRedisplay();

// draw the tree leafs
   glPushMatrix();
    glTranslatef(30.0f, 57.0f, 0.0f);  // feuille
    glRotatef(148.0,0,0,1);
    glScalef(24.0f,6.0f,0.0f);
    glColor3f(0.0,1.0,0.0);             // set the drawing color
    draw_triangle(NULL);
    glPopMatrix();

    // draw the tree leafs
   glPushMatrix();
   glTranslatef(30.0f, 57.0f, 0.0f);  // feuille
   glRotatef(-148.0,0,0,1);
   glScalef(24.0f,6.0f,0.0f);
   glColor3f(0.0,1.0,0.0);             // set the drawing color
   draw_triangle(NULL);
   glPopMatrix();

//    glutSwapBuffers();

    glFlush();                             // force exec of all OpenGl commands
}

void updatecameraviewdir(float dyaw, float dpitch, float dx, float dy, float dz, char fixed)
{
  GLfloat forward[3];
  
  sub(forward, coi, pos); // forward = coi - pos

  // forward direction
  float mforward = sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
  forward[0] /= mforward;
  forward[1] /= mforward;
  forward[2] /= mforward;

  // up direction
  float mup = sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
  up[0] /= mup;
  up[1] /= mup;
  up[2] /= mup;

  // right
  GLfloat right[3];
  cross(right, forward, up);
  float mright = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
  right[0] /= mright;
  right[1] /= mright;
  right[2] /= mright;

  // 90° Up
  cross(up, right, forward);

   // camera frame is (right, forward, up)

   // current viewing direction is parallel with 'forward' (slant = 90°, tilt = 0°)

/*
   newview = cos(yaw) * cos(pitch) * right;
             sin(yaw) * cos(pitch) * view;
             sin(pitch) * up;

   newpos = right * dx + up * dy + forward * dz;
*/
   GLfloat dpos[3];

   scale( dpos, dx, right );
   saxpy( dpos, dy, up );
   saxpy( dpos, dz, forward );
   saxpy( pos, 1.0f, dpos);

   // new direction (yaw += dyaw, pitch += pitch)
   float yaw   = M_PI_2 + dyaw; // slant
   float pitch = dpitch;        // tilt

   scale( forward, sin(yaw) * cos(pitch), forward );
   saxpy( forward, cos(yaw) * cos(pitch), right );
   saxpy( forward, sin(pitch), up );

   if(1 == fixed)  
   {
       // rotate around the coi point
       scale( pos, -mforward, forward);
       saxpy( pos, 1.0f, coi);
   }
   else  
   {    
       // rotate the camera
     scale( coi, mforward, forward);
     saxpy( coi, 1.0f, pos);
   }  
}

void keyboard(unsigned char key, int x, int y){
//   printf("k=%d\n",key);

   char fixed = 0;

   switch(key)
   {
     case  27: exit(0); break; // escape key
     case  'p':
     case  'P': pm = 1-pm; break;

     case 'G': fixed = 1;
     case 'g': updatecameraviewdir(-0.01f, 0.0f, 0.0f, 0.0f, 0.0f, fixed); break;

     case 'J': fixed = 1;
     case 'j': updatecameraviewdir( 0.01f, 0.0f, 0.0f, 0.0f, 0.0f, fixed); break;

     case 'Y': fixed = 1;
     case 'y': updatecameraviewdir( 0.0f, -0.01f, 0.0f, 0.0f, 0.0f, fixed); break;

     case 'B': fixed = 1;
     case 'b': updatecameraviewdir( 0.0f,  0.01f, 0.0f, 0.0f, 0.0f, fixed); break;

     case 52: updatecameraviewdir( 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0); break; //  left key
     case 54: updatecameraviewdir( 0.0f, 0.0f,-1.0f, 0.0f, 0.0f, 0); break; // right key

     case 50: updatecameraviewdir( 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0); break; // down key
     case 56: updatecameraviewdir( 0.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0); break; // up key

     case 51: updatecameraviewdir( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0); break;
     case 57: updatecameraviewdir( 0.0f, 0.0f, 0.0f, 0.0f,-1.0f, 0); break; // page up


     case 114: break;
     case 84:  break;
     case 116: break;
     case 43:  break;
     case 45:  break;
     default:  break; /* do nothing */
   }

   if(1==pm)
     glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  else
     glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);


     glutPostRedisplay();
}

void reshape(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

  aspect = 1.0f * w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fovy, aspect ,near, far);

//	// Get Back to the Modelview
//	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH );
    glutInitWindowSize(500,500);
    glutInit(&argc, argv);              // initialize glut
    glutCreateWindow("UEF4IG Lab2");       // create a windo

    glViewport(0,0,300,200);

/////////////////////////////////////////////////////////////////////////
    glGenTextures(1, &roof);
    glBindTexture(GL_TEXTURE_2D, roof);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    int width, height, channels;
    unsigned char *data = stbi_load("roof0.jpg", &width, &height, &channels, 0); 

    if(data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        fprintf(stderr, "Failed to load roof texture\n");

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    glGenTextures(1, &door);
    glBindTexture(GL_TEXTURE_2D, door);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("door.jpg", &width, &height, &channels, 0); 

    if(data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        fprintf(stderr, "Failed to load grass texture\n");

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    glGenTextures(1, &grass);
    glBindTexture(GL_TEXTURE_2D, grass);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("grass0.jpg", &width, &height, &channels, 0); 

    if(data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        fprintf(stderr, "Failed to load grass texture\n");

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);


    glGenTextures(1, &wall);
    glBindTexture(GL_TEXTURE_2D, wall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("wall0.jpg", &width, &height, &channels, 0); 

    if(data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        fprintf(stderr, "Failed to load wall texture\n");

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

  glGenTextures(1, &window);
    glBindTexture(GL_TEXTURE_2D, window);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("window.jpg", &width, &height, &channels, 0); 

    if(data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        fprintf(stderr, "Failed to load grass texture\n");

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

//////////////////////////////////////////////////////////////////////

    

    glutDisplayFunc(display);           // say what the 'display' func is
    glutKeyboardFunc(keyboard);         // say what the keyboard func is (clavier);
    glutReshapeFunc(reshape);           // say what the reshape func is (reshape);

//    glClearColor(1.0,0.5,0.0,0.5);      // set the clear color

    glutMainLoop();
    return EXIT_SUCCESS;
}
