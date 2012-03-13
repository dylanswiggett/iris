#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"
#include <string>

using namespace std;

/*
 * Define the basic parameters for the SDL window
 */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP    16	//Bits per pixel

/*
 * Derp boolean definitions. We shouldn't need this!!!!
 */
#define TRUE  1
#define FALSE 0

SDL_Surface *surface;

GLuint* tex1;

bool light; //Light on
bool lp; //L pressed
bool fp; //R pressed

GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};

bool keys[255];

/*
 * Exit the program, cleaning up all memory uses and returning to
 * the default desktop properly.
 */
void Quit(int returnCode){
  SDL_Quit();
  exit(returnCode);
}

/*
 * Load a bitmap into a openGL Texture.
 */
GLuint* LoadGLTexture(char* path){
  //Create storage for the image
  SDL_Surface* TextureImage[1];

  GLuint* texture = (GLuint*) malloc(sizeof(texture));

  //Load the bitmap, check for errors, and convert to a texture
  if ((TextureImage[0] = SDL_LoadBMP(path))){
    //Generate the texture
    glGenTextures(1, texture);

    //Set up bitmap mapping to texture
    glBindTexture(GL_TEXTURE_2D, *texture);

    //Generate the texture itself
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->w, TextureImage[0]->h,
		 0, GL_BGR, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);

    //Set up the images filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST); //Use mipmapping for near/large textures
  } else {
    fprintf(stderr, "Error, failed to load texture %s", path);
    Quit(0);
    return NULL;
  }

  //Clean up
  if (TextureImage[0])
    SDL_FreeSurface(TextureImage[0]);

  if (texture){
    return texture;
  } else {
    fprintf(stderr, "Error, failed to load texture %s", path);
    Quit(0);
    return NULL;
  }
}


/*
 * Resize the window, then recalculate the viewport.
 */
int resizeWindow(int width, int height){

  //Ratio of width / height
  GLfloat whratio;

  if (height == 0) height = 1;

  whratio = (GLfloat) width / (GLfloat) height;
  
  //Change viewport
  glViewport(0, 0, (GLint) width, (GLint) height);

  //Set up perspective!
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective( 45.0f, whratio, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  return TRUE;
}

/*
 * Handle any and all key events here
 */
void handleKeyPress(SDL_keysym* keysym){
  switch (keysym->sym){
  case SDLK_ESCAPE:
    Quit(0);
    break;
  case SDLK_F11:
    // Toggle full screen if F11 is pressed
    SDL_WM_ToggleFullScreen(surface);
    break;
  default:
    break;
  }
}

/*
 * Initialize openGL for use.
 */
int initGL(){
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  //Set up lighting
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);

  //Set up blending
  glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  return TRUE;
}

/*
 * All drawing to the screen, using the surface.
 */
int drawGLScene(){
  //Calculate FPS
  static GLint T0 = 0;
  static GLint Frames = 0;
  
  //Clean the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glLoadIdentity();

  /*
   * ALL DRAWING CODE GOES HERE
   */

  glTranslatef(-1.5f, 0.0f, -6.0f);

  glRotatef((float) Frames / 100.0f, 0.0f, 1.0f, 0.0f);
  
  if (!tex1)
    tex1 = LoadGLTexture("tex.bmp");

  /* Triangle */
  /*
  glBegin(GL_TRIANGLES);
  {
    //Front
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    //Right
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f,-1.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    //Back
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f,-1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    //Left
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);


  }
  glEnd();
  */
  glLoadIdentity();
  glTranslatef(1.0f, 0.0f, -10.0f);

  glRotatef((float) Frames / 47.0f, 1.0f, 30.0f, 45.0f);

  glBindTexture(GL_TEXTURE_2D, *tex1);

  glEnable(GL_TEXTURE_2D);

  glColor3f(1.0f, 1.0f, 1.0f);

  glBegin(GL_QUADS);
  {
    //Front
    //    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    //Right
    //    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    //Back
    //    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    //Left
    //    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    //Top
    //    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    //Bottom
    //    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

  }
  glEnd();

  glDisable(GL_TEXTURE_2D);


  /*
   * END OF DRAWING CODE
   */

  //Draw to the screen
  SDL_GL_SwapBuffers();

  //Calculate FPS
  Frames ++;
  {
    GLint t = SDL_GetTicks();
    if (Frames % 100 == 0){
      GLfloat seconds = (t - T0) / 1000.0;
      GLfloat fps = Frames / seconds;
      printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
    }
  }

  return TRUE;
}


int main(int argc, char **argv){
  //Flags for SDL_SetVideoMode
  int videoFlags;
  //Main loop termination check
  int done = FALSE;
  //Event collection
  SDL_Event event;
  //Display information
  const SDL_VideoInfo *videoInfo;
  //Whether the window is active
  int isActive = TRUE;

  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0){
    fprintf( stderr, "SDL Video Initialization failed: %s\n", SDL_GetError());
    Quit(1);
  }

  //Get the display information
  videoInfo = SDL_GetVideoInfo();

  if (!videoInfo){
    fprintf( stderr, "SDL could not get video information: %s\n", SDL_GetError());
    Quit(1);
  }

  /*
   * Enable openGL, double buffering, hardware palette use, and window resizing in the videoFlags
   */
  videoFlags =  SDL_OPENGL;
  videoFlags |= SDL_GL_DOUBLEBUFFER;
  videoFlags |= SDL_HWPALETTE;
  videoFlags |= SDL_RESIZABLE;

  //Check if surfaces can be stored in memory
  if (videoInfo -> hw_available){
    videoFlags |= SDL_HWSURFACE;
  } else {
    videoFlags |= SDL_SWSURFACE;
  }

  //Try to enable hardware blits (I'm not sure what this is....)
  if (videoInfo -> blit_hw)
    videoFlags |= SDL_HWACCEL;

  //Enable openGL double buffering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  //Get a surface
  surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);

  if (!surface){
    fprintf( stderr, "SDL failed to initialize the surface: %s\n", SDL_GetError());
    Quit(1);
  }

  if (initGL() == FALSE){
    fprintf( stderr, "Failed to initialize openGL");
    Quit(1);
  }

  //Size the initial window
  resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

  //Wait for events
  while (!done){
    //Iterate through all buffered events
    while (SDL_PollEvent(&event)){
      switch (event.type){
      case SDL_ACTIVEEVENT:  //Change in focus
	if (event.active.gain == 0)
	  isActive = false;
	else
	  isActive = true;
	break;
      case SDL_VIDEORESIZE:
	surface = SDL_SetVideoMode(event.resize.w, event.resize.h, SCREEN_BPP, videoFlags);
	break;
      case SDL_KEYDOWN:
	keys[event.key.keysym.sym] = true; //Store that the key is pressed
	handleKeyPress(&event.key.keysym);
	break;
      case SDL_KEYUP:
	keys[event.key.keysym.sym] = false; //Store that the key has been released
	break;
      case SDL_QUIT:
	done = TRUE;
	break;
      default:
	break;
      }
    }
    
    //Redraw the screen after all events have been handled
    if (isActive)
      drawGLScene();
  }

  Quit(0);

  return 0;
}
