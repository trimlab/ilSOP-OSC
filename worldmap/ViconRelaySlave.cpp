
#include "Client.h"

#include "/share/apps/glew/1.9.0/include/GL/glew.h"
#include <GL/glut.h>

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sstream>

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>

#include <string>
#include <vector>
#include <time.h>
#include <sys/time.h>

#include "../boost_1_53_0/boost/format.hpp"
#include "imageio.h"

using namespace std;
using namespace ViconDataStreamSDK::CPP;
using boost::format;

#define output_stream if(!LogFile.empty()) ; else std::cout

// ***VICON***

// Make a new client
Client MyClient;
std::string HostName = "141.219.28.17:801";
//std::string HostName = "localhost:801";

// screen width/height indicate the size of the window on our screen (not the size of the display wall). The aspect ratio must match the actual display wall.
//const GLdouble SCREEN_WIDTH = (1920.0*6)/8.0;
//const GLdouble SCREEN_HEIGHT = (1080.0*4)/8.0;
const GLdouble SCREEN_WIDTH = (1920.0*3);
const GLdouble SCREEN_HEIGHT = 1080.0;
const float screenAspectRatio = SCREEN_WIDTH/SCREEN_HEIGHT;

// socket stuff
struct sockaddr_in si_other;
int slen, s;
int so_broadcast = 1;
pthread_t receiverThread;

//splitting rendering up across multiple displays
double ortho_left;
double ortho_right;
double ortho_bottom;
double ortho_top;

// timestamp stuff
time_t curtime;
char timebuff[30];
struct timeval tv;


string ipAddress;
unsigned int port;
vector<string> objectsToTrack;
string texturePath;
string dataToSend;

const GLuint MAX_TILES = 100;
GLuint numTiles=0;
GLuint texNames[MAX_TILES];
float aspectRatio;

float userX = 0.0f;
float userY = 0.0f;
float userZ = 0.0f;

//string splitting functions
vector<string> &split(const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  return split(s, delim, elems);
}

float readfile(char *filename, GLuint *texName, GLuint *numTiles)
{
	static int verbose=1;  // change this to 0 to print out less info

	imageio_info iioinfo;
	iioinfo.filename = filename;
	iioinfo.type = CharPixel;
	iioinfo.map = (char*) "RGBA";
	//	iioinfo.colorspace = RGBColorspace;
	char *image = (char*) imagein(&iioinfo);

	if(image == NULL)
	{
		fprintf(stderr, "\n%s: Unable to read image.\n", filename);
		return -1;
	}

	/* "image" is a 1D array of characters (unsigned bytes) with four
	 * bytes for each pixel (red, green, blue, alpha). The data in "image"
	 * is in row major order. The first 4 bytes are the color information
	 * for the lowest left pixel in the texture. */
	int width  = (int)iioinfo.width;
	int height = (int)iioinfo.height;
	float original_aspectRatio = (float)width/height;
	if(verbose)
		printf("%s: Finished reading, dimensions are %dx%d\n", filename, width, height);

	if(height > 4096*2)
	{
		printf("Source image must <= 8192 pixels tall.");
		exit(1);
	}

	int subimgH = height/2;
	int workingWidth = width;
	*numTiles = 1;
	while(workingWidth > 4096)
	{
		*numTiles = *numTiles * 2;
		workingWidth = workingWidth / 2;
	}
	int subimgW = workingWidth;

	if(*numTiles > MAX_TILES/2.0)
	{
		printf("Too many tiles");
		exit(1);
	}

	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGB, subimgW, subimgH,
	             0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	int tmp;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tmp);

	if(tmp == 0)
	{
		fprintf(stderr, "%s: File is too large (%d x %d). I can't load it!\n", filename, subimgW, subimgH);
		fprintf(stderr, "This ONLY works under chromium since ivs.research itself doesn't support large enough texture sizes.\n");
		free(image);
		exit(1);
	}


	glGenTextures(*numTiles*2, texName);

	for(GLuint curTile=0; curTile < *numTiles*2; curTile = curTile+2)
	{
		glPixelStorei(GL_UNPACK_ROW_LENGTH,  width);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, curTile/2.0*subimgW);
		glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);

		glBindTexture(GL_TEXTURE_2D, texName[curTile]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, subimgW, subimgH,
		             0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glPixelStorei( GL_UNPACK_SKIP_PIXELS, curTile/2.0*subimgW );
		glPixelStorei( GL_UNPACK_SKIP_ROWS, subimgH);

		glBindTexture(GL_TEXTURE_2D, texName[curTile+1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, subimgW, subimgH,
		             0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}

	free(image);
	return original_aspectRatio;
}

void display() {

    userY += 2.0f;
    userZ -= 1.2f; // assume this is approx. where most people will hold the wand
    float scaleMap = 1.0f + (userY / 1.3f);
    if (scaleMap < 1.0f) scaleMap = 1.0f;
    float xScale = -userX / scaleMap;
    float zScale = -userZ * (scaleMap / 3.0f);

    float quadHeightScale = 1;
    float tileWidth = 2.0/(numTiles);

//    glEnable(GL_NORMALIZE);
//    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
//    glFrustum(ortho_left, ortho_right, ortho_bottom, ortho_top, 0.1, 5000);
    glEnable(GL_TEXTURE_2D);

    glColor3f(1,1,1); // color of quad

//    glPushMatrix();
//    glTranslatef((-userX / 3.0f), (userZ / 3.0f), userY / 3.0f);
    for(GLuint i=0; i<numTiles*2; i=i+2)
    {
        glBindTexture(GL_TEXTURE_2D, texNames[i]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2d(i/2    *tileWidth-1, quadHeightScale*-1);
        glTexCoord2f(1.0, 0.0); glVertex2d((i/2+1)*tileWidth-1, quadHeightScale*-1);
        glTexCoord2f(1.0, 1.0); glVertex2d((i/2+1)*tileWidth-1, quadHeightScale*0);
        glTexCoord2f(0.0, 1.0); glVertex2d(i/2    *tileWidth-1, quadHeightScale*0);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texNames[i+1]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2d(i/2    *tileWidth-1, quadHeightScale*0);
        glTexCoord2f(1.0, 0.0); glVertex2d((i/2+1)*tileWidth-1, quadHeightScale*0);
        glTexCoord2f(1.0, 1.0); glVertex2d((i/2+1)*tileWidth-1, quadHeightScale*1);
        glTexCoord2f(0.0, 1.0); glVertex2d(i/2    *tileWidth-1, quadHeightScale*1);
        glEnd();
    }
//    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0,0,0,.3);
    glBegin(GL_QUADS);
    glVertex2d(-1,-1);
    glVertex2d(-.5,-1);
    glVertex2d(-.5,-.96);
    glVertex2d(-1,-.96);
    glEnd();

    glColor4f(1,1,1,.9);
    glRasterPos2f(-.98,-.98);
    void *font = GLUT_BITMAP_TIMES_ROMAN_24;
    char *str = texturePath.c_str();
    for(GLuint i=0; i<strlen(str); i++)
        glutBitmapCharacter(font, str[i]);

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();

}

void receiver()
{
  while (true)
  {

    char buf[1024];

    gettimeofday(&tv, NULL);
    curtime=tv.tv_sec;
    strftime(timebuff, 30, "%m-%d-%Y %T.", localtime(&curtime));


    if(recvfrom(s, buf, 1024, 0, (struct sockaddr*)&si_other,
      &slen) == -1)
    {

    }

    string recievedData (buf);
    vector<string> splitData = split(recievedData, '~');

    userX = atof(splitData[0].c_str());
    userY = atof(splitData[1].c_str());
    userZ = atof(splitData[2].c_str());

    usleep(4000);
  }
}

int main(int argc, char* argv[]) {

  if (argc < 2) {
    printf("Usage: ViconRelaySlave TexturePath\n");
    return 1;
  }

  texturePath = string(argv[1]);

//  printf("IP address passed in was %s\n", ipAddress.c_str()); // OK, that's working.

  // OpenGL initialization
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Vicon Relay");
  int glew_err = glewInit();
  if (glew_err != GLEW_OK) fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(glew_err));
  glutDisplayFunc(display);

  // load image
  readfile(texturePath.c_str(), texNames, &numTiles);

  // Go
  glutMainLoop();

  return 0;

}
