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
void circle_fractale(float, float, float);

static GLuint  _screen = 0;

static GLfloat    _posX      = 0, _posY = 0, _rayon = 0;
static GLuint   _timeBegin      = 0, _timeEnd = 0;
static  GLuint     _nbrCircle = 200, _state = 0;
static circle_t * _tabCircle = NULL;
static int _circleId = 0;
static const Uint32 _steril = 0x0;

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
  _posY = height/1.8;
  _rayon = rayon;

  if(_tabCircle) {
    free(_tabCircle);
    _tabCircle = NULL;
  }

  _tabCircle = malloc(_nbrCircle * sizeof * _tabCircle);
  assert(_tabCircle);
 
}

void circle_draw(float posX, float posY, float rayon, int affichageMode){
  
  gl4dpSetColor(RGB(rand()%255, rand()%255, rand()%255));
  gl4dpFilledCircle(posX, posY, rayon);
  
  if(_circleId == (_nbrCircle-1)){
    _tabCircle[_circleId].posX = posX;
    _tabCircle[_circleId].posY = posY;
    _tabCircle[_circleId].rayon = rayon;
    ++_circleId;
    return;
  }
  if(rayon/2 > 5){
    if(affichageMode == 1){
      circle_draw(posX+rayon, posY, rayon/2, affichageMode);
      circle_draw(posX-rayon, posY, rayon/2, affichageMode);
    }
    if(affichageMode == 2){
      circle_draw(posX+rayon, posY, rayon/2, affichageMode);
      circle_draw(posX, posY-rayon, rayon/2, affichageMode);
    }
    if(affichageMode == 3){
      circle_draw(posX-rayon, posY, rayon/2, affichageMode);
      circle_draw(posX, posY-rayon, rayon/2, affichageMode);
    }
    if(affichageMode == 4){
      circle_draw(posX+rayon, posY, rayon/2, affichageMode);
      circle_draw(posX-rayon, posY, rayon/2, affichageMode);
      circle_draw(posX, posY-rayon, rayon/2, affichageMode);
    }
  }
}

void draw(){
  _timeBegin = SDL_GetTicks();

  if(_state == 0){
    if(_timeBegin - _timeEnd > 4000){
      gl4dpClearScreenWith(_steril);
      circle_draw(_posX, _posY, _rayon, _state+1);
      _state++;
      _timeEnd = _timeBegin;
      return;
    }
  }

  if(_state == 1){
    if(_timeBegin - _timeEnd > 4000){
      gl4dpClearScreenWith(_steril);
      circle_draw(_posX, _posY, _rayon, _state+1);
      _state++;
      _timeEnd = _timeBegin;
      return;
    }
  }

  if(_state == 2){
    if(_timeBegin - _timeEnd > 4000){
      gl4dpClearScreenWith(_steril);
      circle_draw(_posX, _posY, _rayon, _state+1);
      _state++;
      _timeEnd = _timeBegin;
      return;
    }
  }

  if(_state == 3){
    if(_timeBegin - _timeEnd > 4000){
      gl4dpClearScreenWith(_steril);
      circle_draw(_posX, _posY, _rayon, _state+1);
      _state++;
      _timeEnd = _timeBegin;
      return;
    }
  }

  if(_state == 4){
    gl4dpClearScreenWith(_steril);
    circle_draw(_posX, _posY, _rayon, _state);
    return;
  }

}

void circle_anim(int state) {
  switch(state) {
    case GL4DH_INIT:
      _screen = gl4dpInitScreen();
      init(gl4dpGetWidth(), gl4dpGetHeight(), 200);
      return;
    case GL4DH_FREE:
      quit();
      gl4dpSetScreen(_screen);
      gl4dpDeleteScreen();
      return;
    case GL4DH_UPDATE_WITH_AUDIO:
      return;
    default:
      gl4dpSetScreen(_screen);
      draw();
      gl4dpUpdateScreen(NULL);
      return;

   }
}
