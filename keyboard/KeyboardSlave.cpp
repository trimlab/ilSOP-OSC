
#include "Client.h"

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

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

using namespace std;
using namespace ViconDataStreamSDK::CPP;
using boost::format;

#define output_stream if(!LogFile.empty()) ; else std::cout


// screen width/height indicate the size of the window on our screen (not the size of the display wall). The aspect ratio must match the actual display wall.
const GLdouble SCREEN_WIDTH = (1920.0*3);
const GLdouble SCREEN_HEIGHT = 1080.0;
const float screenAspectRatio = SCREEN_WIDTH/SCREEN_HEIGHT;

double ortho_left;
double ortho_right;
double ortho_bottom;
double ortho_top;

// socket stuff
struct sockaddr_in si_other;
int slen, s;
int so_broadcast = 1;
pthread_t recieverThread;

// timestamp stuff
time_t curtime;
char timebuff[30];
struct timeval tv;

string ipAddress;
unsigned int port;
vector<string> objectsToTrack;
string dataToSend;

const int OCTAVES = 3;
const int WHITE_KEYS = 7;

bool whiteKeysPressed[OCTAVES * WHITE_KEYS];
bool blackKeysPressed[OCTAVES * WHITE_KEYS];

GLfloat light_ambient0[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat light_diffuse0[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_specular0[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_position0[] = {0.0f, 4.0f, 0.0f, 1.0f};

GLfloat light_ambient4[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat light_diffuse4[] = {1.0f, 1.1f, 1.1f, 1.0f};
GLfloat light_specular4[] = {1.0f, 1.1f, 1.1f, 1.0f};
GLfloat light_position4[] = {0.0f, 3.0f, 0.0f, 1.0f};
GLfloat light_direction4[] = {0.0f, -1.0f, 0.0f, 1.0f};

GLfloat keys_ambient[] = {0.2125f, 0.2125f, 0.2125f, 1.0f};
GLfloat keys_diffuse[] = {0.714f, 0.714f, 0.714f, 1.0f};
GLfloat keys_specular[] = {0.393548f, 0.393548f, 0.393548f, 1.0f};
GLfloat keys_shininess[] = {3.0f};

GLfloat accs_ambient[] = {0.02125f, 0.02125f, 0.02125f, 1.0f};
GLfloat accs_diffuse[] = {0.0714f, 0.0714f, 0.0714f, 1.0f};
GLfloat accs_specular[] = {0.0393548f, 0.0393548f, 0.0393548f, 1.0f};
GLfloat accs_shininess[] = {5.0f};

GLfloat pressed_ambient[] = {0.02125f, 0.02125f, 0.2125f, 1.0f};
GLfloat pressed_diffuse[] = {0.0714f, 0.0714f, 0.714f, 1.0f};
GLfloat pressed_specular[] = {0.0393548f, 0.0393548f, 0.393548f, 1.0f};
GLfloat pressed_shininess[] = {3.0f};

float userPos[100][3];

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

void drawWhiteKey() {
  glPushMatrix();
  glScalef(1.0f, 0.5f, 6.0f);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void drawPressedWhiteKey() {

  glMaterialfv(GL_FRONT, GL_AMBIENT, pressed_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, pressed_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, pressed_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, pressed_shininess);

  glPushMatrix();
  glTranslatef(0.0f, -0.3f, 0.0f);
  glRotatef(5.0f, 5.0f, 0.0f, 0.0f);
  glScalef(1.0f, 0.5f, 6.0f);
  glutSolidCube(1.0f);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_AMBIENT, keys_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, keys_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, keys_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, keys_shininess);
}

void drawBlackKey() {
  glPushMatrix();
  glTranslatef(0.0f, 0.3f, -1.0f);
  glScalef(0.7f, 0.5f, 3.75f);
  glutSolidCube(1.0f);
  glPopMatrix();
}

void drawPressedBlackKey() {

  glMaterialfv(GL_FRONT, GL_AMBIENT, pressed_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, pressed_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, pressed_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, pressed_shininess);

  glPushMatrix();
  glTranslatef(0.0f, 0.2f, -1.0f);
  glRotatef(5.0f, 5.0f, 0.0f, 0.0f);
  glScalef(0.7f, 0.5f, 3.75f);
  glutSolidCube(1.0f);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_AMBIENT, accs_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, accs_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, accs_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, accs_shininess);
}

void display()
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0 * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glFrustum(ortho_left, ortho_right, ortho_bottom, ortho_top, 0.1, 5000);
  gluLookAt(0.0, 8.5, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

  glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient4);
  glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
  glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular4);
  glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
  glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_direction4);
  glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30.f);

  glMaterialfv(GL_FRONT, GL_AMBIENT, keys_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, keys_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, keys_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, keys_shininess);

  for (int i = 0; i < OCTAVES * WHITE_KEYS; i++) {
    glPushMatrix();
    glTranslatef((i * 1.0625f) - (OCTAVES * WHITE_KEYS * 1.0625f / 2.0f) + 0.5f, 0.0f, 0.0f);
    if (!whiteKeysPressed[i]) drawWhiteKey();
    else drawPressedWhiteKey();
    glPopMatrix();
  }

  glMaterialfv(GL_FRONT, GL_AMBIENT, accs_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, accs_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, accs_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, accs_shininess);

  int keyCount = 1;
  for (int i = 0; i < OCTAVES * WHITE_KEYS; i++) {
    if (keyCount == 1 || keyCount == 2 || keyCount == 4 || keyCount == 5
      || keyCount == 6)
    {
      glPushMatrix();
      glTranslatef(((i+1) * 1.0625f) - (OCTAVES * WHITE_KEYS * 1.0625f / 2.0f), 0.0f, 0.0f);
      //glTranslatef(((OCTAVES * WHITE_KEYS / 2.0f) - i) * 1.0625f, 0.0f, 0.0f);
      if (!blackKeysPressed[i]) drawBlackKey();
      else drawPressedBlackKey();
      glPopMatrix();
    }
    keyCount++;
    if (keyCount > 7) keyCount = 1;
  }

  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();

}

const float c_firstKey = -3.17f;
const float c_lastKey = 2.3f;
const int   c_whiteKeys = 21;
const float c_range = c_lastKey - c_firstKey;
const float c_keySize = c_range / c_whiteKeys;
const float c_halfKey = c_keySize / 2;

void reciever()
{
  cout << "Starting Receiver Thread" << endl;
  ofstream file;

  bool logged = false;

  while (true)
  {
    for (int i = 0; i < NELEMS(userPos); i++) {
      for (int j = 0; j < 3; j++) {
        userPos[i][j] = -99999.0f;
      }
    }

    dataToSend.clear();

    char buf[1024];
    if (recvfrom(s, buf, 1024, 0, (struct sockaddr*)&si_other,
      &slen) == -1)
    {

    }
    string itrmdt(buf);

    printf("Received %s\n", buf);

    vector<string> splitLine = split(itrmdt, '~');

    if (i < NELEMS(userPos)) {
      for (int j = 0; j < 3; j++) {
        userPos[i][j] = atof(splitLine[i].c_str()) / 1000.0f;
      }
    }

    float x = c_firstKey;
    for (int i = 0; i < OCTAVES * WHITE_KEYS; i++)
    {
      whiteKeysPressed[i] = false;
      blackKeysPressed[i] = false;
      for (int j = 0; j < NELEMS(objectsToTrack); j++)
      {
        if (j < NELEMS(objectsToTrack))
        {
          if (userPos[j][1] < 0)
          {
            if (userPos[j][2] < 0.2 && userPos[j][0] > x - c_halfKey * 2 && userPos[j][0] < x)
            {
              whiteKeysPressed[i] = true;
            }
          }
          else
          {
            if (userPos[j][2] < 0.2 && userPos[j][0] > x - c_halfKey && userPos[j][0] < x + c_halfKey)
            {
              blackKeysPressed[i] = true;
            }
          }
        }
      }
      x += c_keySize;
    }

    //printf("x is %f\n", userX);
    //usleep(1000);
  }
}


int main(int argc, char** argv) {

  if (argc < 4) {
    printf("Usage: KeyboardSlave left right bottom top IP Port Objects\n");
    return 1;
  }
  ortho_left = atof(argv[1]); //-5.0;
  ortho_right = atof(argv[2]); //0.0;
  ortho_bottom = atof(argv[3]); //-5.0;
  ortho_top = atof(argv[4]); //5.0;
  ipAddress = string(argv[5]);
  port = atoi(argv[6]);
  for (int i = 7; i < argc; i++) {
    objectsToTrack.push_back(string(argv[i]));
  }


  for (int i = 0; i < WHITE_KEYS * OCTAVES; i++) {
    whiteKeysPressed[i] = false;
    blackKeysPressed[i] = false;
  }
  //whiteKeysPressed[6] = true;
  //blackKeysPressed[5] = true;

  // OpenGL initialization
  glutInit(&argc, argv); //initialize the toolkit
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  //set display mode
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); //set window size
  glutInitWindowPosition(0, 0); //set window position on screen
  glutCreateWindow("Tracked Keyboard"); //open the screen window

  glutDisplayFunc(display);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT4);


  //socket stuff
  slen=sizeof(si_other);
  so_broadcast = 1;

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("ERROR socket");
    exit(1);
  }

  setsockopt(s, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port);
  if (inet_aton(ipAddress.c_str(), &si_other.sin_addr) == 0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  if (pthread_create(&recieverThread, NULL, reciever, NULL) != 0) {
    perror("Can't start thread, terminating\n");
    return 1;
  }

  // go
  glutMainLoop();

  return 0;
}
