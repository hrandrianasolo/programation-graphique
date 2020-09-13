#include <assert.h>
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4dp.h>
#include "audioHelper.h"

typedef struct circle_t circle_t;
struct circle_t {
  float posX, posY, rayon;
};

static void quit(void);
static void init(int, int, float);
static void draw(void);

static GLuint  _screen = 0;
static GLfloat _basses = 0;

static GLuint  _begin  = 0, _end = 0;
static GLfloat myRand  = 0;
static GLuint  _state  = 0;

static GLfloat    _posX      = 0, _posY = 0, _rayon = 0;
const  GLuint     _nbrCircle = 61;
static circle_t * _tabCircle = NULL;
static int        _circleId = 0;
static int        _circleIdReverse = 60;
static int        _lastCirclerayon = 1, _lastCirclerayon2 = 1;
static int        endAnim = 0;

static void quit(void) {
  if(_screen) {
    gl4dpSetScreen(_screen);
    gl4dpDeleteScreen();
    _screen = 0;
  }

  if(_tabCircle) {
    free(_tabCircle);
    _tabCircle = NULL;
  }
}

void init(int weight, int height, float rayon) {
  _posX = weight/2;
  _posY = height/2;
  _rayon = rayon;

  if(_tabCircle) {
    free(_tabCircle);
    _tabCircle = NULL;
  }

  _tabCircle = malloc(_nbrCircle * sizeof * _tabCircle);
  assert(_tabCircle);
}

void circle_fractale(float posX, float posY, float rayon){

  gl4dpSetColor(RGB(255,255,255));
  gl4dpFilledCircle(posX, posY, rayon);
  _tabCircle[_circleId].posX = posX;
  _tabCircle[_circleId].posY = posY;
  _tabCircle[_circleId].rayon = rayon;
  ++_circleId;

  gl4dpSetColor(RGB(102, 0, 255));
  gl4dpFilledCircle(posX+rayon, posY, rayon/2);
  _tabCircle[_circleId].posX = posX+rayon;
  _tabCircle[_circleId].posY = posY;
  _tabCircle[_circleId].rayon = rayon / 2;
  ++_circleId;

  gl4dpSetColor(RGB(102, 153, 255));
  gl4dpFilledCircle(posX-rayon, posY, rayon/2);
  _tabCircle[_circleId].posX = posX-rayon;
  _tabCircle[_circleId].posY = posY;
  _tabCircle[_circleId].rayon = rayon / 2;
  ++_circleId;
}

void draw(){
  _begin = SDL_GetTicks();

  myRand = _rayon;

  if(_basses >= 0.3 && (_begin - _end) > 400){
    if(_state%4){
      _state++;
      return;      
    }
    if(_rayon/2 >= 0.2){
      circle_fractale(_posX+myRand, _posY, myRand);
      printf(" _circleId %d \n", _circleId);
      circle_fractale(_posX-myRand, _posY, myRand);
      printf("fin  _circleId %d \n", _circleId);          
      _rayon = _rayon/2;    
      _end = _begin;
      _state++;
      return;
    }
  }

  if(_basses > 1  && (_begin - _end) > 10){
    if(_circleIdReverse != 5){
      if(gl4dpGetPixel(_tabCircle[_circleIdReverse].posX, _tabCircle[_circleIdReverse].posY) != RGB(0,0,0)){
        gl4dpSetColor(RGB(0,0,0));
      }
      else{
        gl4dpSetColor(RGB(255,255,255));
      }
      gl4dpFilledCircle(_tabCircle[_circleIdReverse].posX, _tabCircle[_circleIdReverse].posY, _tabCircle[_circleIdReverse].rayon);
      --_circleIdReverse;	
      _end = _begin;
      return;
    }	
    else{
      endAnim = 1;    
    }
  }
  if(endAnim == 1){
    gl4dpSetColor(RGB(255,200,0));
    gl4dpFilledCircle(gl4dpGetWidth()/2, gl4dpGetHeight()/2, _lastCirclerayon);
    _lastCirclerayon += 3;

    gl4dpSetColor(RGB(0,0,0));
    gl4dpFilledCircle(gl4dpGetWidth()/2, gl4dpGetHeight()/2, _lastCirclerayon2);
    _lastCirclerayon2 += 2;
    if(_lastCirclerayon < 50){     
      ++endAnim;
      return;
    }
  }
}

void circle(int state) {
  switch(state) {
    case GL4DH_INIT:
      _screen = gl4dpInitScreen();
      init(gl4dpGetWidth(), gl4dpGetHeight(), 350);
      return;
    case GL4DH_FREE:
      quit();
      gl4dpSetScreen(_screen);
      gl4dpDeleteScreen();
      return;
    case GL4DH_UPDATE_WITH_AUDIO:
      _basses = ahGetAudioStreamFreq();
      return;
    default:
      gl4dpSetScreen(_screen);
      draw();
      gl4dpUpdateScreen(NULL);
      return;

   }
}
