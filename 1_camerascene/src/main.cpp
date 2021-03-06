#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "shaders.h"
#include "Transform.h"
#include <FreeImage.h>

using namespace std;

// Main variables in the program.
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp
void display(void);  // prototype for display function.

bool allowGrader = false;
int mouseoldx, mouseoldy ; // For mouse motion

// Uses the Projection matrices (technically deprecated) to set perspective
// We could also do this in a more modern fashion with glm.
void reshape(int width, int height)
{
  w = width;
  h = height;
  mat4 mv; // just like for lookat

  glMatrixMode(GL_PROJECTION);
  float aspect = w / (float) h, zNear = 0.1, zFar = 99.0;

  // I am changing the projection stuff to be consistent with lookAt
  if (useGlu) {
    mv = glm::perspective(fovy,aspect,zNear,zFar);
  } else {
    mv = Transform::perspective(fovy,aspect,zNear,zFar);
  }

  glLoadMatrixf(&mv[0][0]);
  glViewport(0, 0, w, h);
}

void saveScreenshot(string fname)
{
  int pix = w * h;
  BYTE pixels[3*pix];
  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE, pixels);

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}


void printHelp()
{
  std::cout << "\npress 'h' to print this message again.\n"
    << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n"
    << "press 'r' to reset the transformations.\n"
    << "press 'v' 't' 's' to do view [default], translate, scale.\n"
    << "press ESC to quit.\n";
}


void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
    case '+':
      amount++;
      std::cout << "amount set to " << amount << "\n";
      break;
    case '-':
      amount--;
      std::cout << "amount set to " << amount << "\n";
      break;
    case 'h':
      printHelp();
      break;
    case 27:  // Escape to quit
      exit(0);
      break;
    case 'r': // reset eye and up vectors, scale and translate.
      eye = eyeinit;
      up = upinit;
      amount = amountinit;
      transop = view;
      sx = sy = 1.0;
      tx = ty = 0.0;
      break;
    case 'v':
      transop = view;
      std::cout << "Operation is set to View\n";
      break;
    case 't':
      transop = translate;
      std::cout << "Operation is set to Translate\n";
      break;
    case 's':
      transop = scale;
      std::cout << "Operation is set to Scale\n";
      break;
  }
  glutPostRedisplay();
}

//  You will need to enter code for the arrow keys
//  When an arrow key is pressed, it will call your transform functions
void specialKey(int key, int x, int y)
{
  switch(key) {
    case 100: //left
      if (transop == view) Transform::left(amount, eye,  up);
      else if (transop == scale) sx -= amount * 0.01;
      else if (transop == translate) tx -= amount * 0.01;
      break;
    case 101: //up
      if (transop == view) Transform::up(amount,  eye,  up);
      else if (transop == scale) sy += amount * 0.01;
      else if (transop == translate) ty += amount * 0.01;
      break;
    case 102: //right
      if (transop == view) Transform::left(-amount, eye,  up);
      else if (transop == scale) sx += amount * 0.01;
      else if (transop == translate) tx += amount * 0.01;
      break;
    case 103: //down
      if (transop == view) Transform::up(-amount,  eye,  up);
      else if (transop == scale) sy -= amount * 0.01;
      else if (transop == translate) ty -= amount * 0.01;
      break;
  }
  glutPostRedisplay();
}

void init()
{
  // Initialize shaders
  vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl");
  fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl");
  shaderprogram = initprogram(vertexshader, fragmentshader);
  enablelighting = glGetUniformLocation(shaderprogram,"enablelighting");
  lightpos = glGetUniformLocation(shaderprogram,"lightposn");
  lightcol = glGetUniformLocation(shaderprogram,"lightcolor");
  numusedcol = glGetUniformLocation(shaderprogram,"numused");
  ambientcol = glGetUniformLocation(shaderprogram,"ambient");
  diffusecol = glGetUniformLocation(shaderprogram,"diffuse");
  specularcol = glGetUniformLocation(shaderprogram,"specular");
  emissioncol = glGetUniformLocation(shaderprogram,"emission");
  shininesscol = glGetUniformLocation(shaderprogram,"shininess");
}


void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			// Do Nothing ;
		}
		else if (state == GLUT_DOWN) {
			mouseoldx = x ; mouseoldy = y ; // so we can move wrt x , y
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{ // Reset gluLookAt
		eye = eyeinit;
		up = upinit;
		glutPostRedisplay() ;
	}
}

void mousedrag(int x, int y) {
	int yloc = y - mouseoldy  ;    // We will use the y coord to zoom in/out
	vec3 dir = glm::normalize(center - eye);
	eye += 0.005f * (float)yloc * dir ;         // Where do we look from
	mouseoldy = y ;

	glutPostRedisplay() ;
}


int main(int argc, char* argv[])
{
  FreeImage_Initialise();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Test OpenGL");
  init();
  readfile(argv[1]);
  glutDisplayFunc(display);
  glutSpecialFunc(specialKey);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutReshapeWindow(w, h);
  glutMouseFunc(mouse) ;
  glutMotionFunc(mousedrag) ;

  printHelp();
  glutMainLoop();
  FreeImage_DeInitialise();
  return 0;
}
