#include <assert.h>
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4dp.h>
#include "audioHelper.h"

typedef struct line_t line_t;
struct line_t {
  float beginX, beginY, endX, endY;
  short R, G, B;
};

typedef struct cirlce_t cirlce_t;
struct cirlce_t {
  float posX, posY, rayon;
  short R, G, B;
};

static void quit(void);
static void init(int, int, int);
static void draw(void);

static GLuint   _screen    = 0;
static GLfloat  _basses    = 0;

static GLuint   _timeBegin      = 0, _timeEnd = 0;
static GLuint   _state     = 0;

static line_t * _tabLine   = NULL;
static cirlce_t *_tabFruit = NULL;
static short *  _visitedId = NULL;

static GLfloat  _posX      = 0, _posY = 0;
static GLfloat  _angle     = 0, _angleRotation = 0, _lineMinSize = 0.5;
static GLfloat  _length    = 0, _itter = 0.5;
static short    _nbrLine   = 0;

static short    _bDraw     = 0, _eDraw = 255;
static int      _nbrFruit  = 0;
static const Uint32 _steril = 0x0;



static void quit(void) {
  if(_screen) {
    gl4dpSetScreen(_screen);
    gl4dpDeleteScreen();
    _screen = 0;
  }

  if(_tabLine) {
    free(_tabLine);
    _tabLine = NULL;
  }

  if(_tabFruit) {
    free(_tabFruit);
    _tabFruit = NULL;
  }

  if(_visitedId) {
    free(_visitedId);
    _visitedId = NULL;
  }
}

void init(int weight, int height, int length) {
  _posX = gl4dpGetWidth()/2;
  _posY = gl4dpGetHeight()*0.30;
  _length = length;  
  _angle = M_PI/2;
  _angleRotation = M_PI/8;

  if(_tabLine) {
    free(_tabLine);
    _tabLine = NULL;
  }

  if(_tabFruit) {
    free(_tabFruit);
    _tabFruit = NULL;
  }

  if(_visitedId) {
    free(_visitedId);
    _visitedId = NULL;
  }

  _tabLine = malloc(255 * sizeof * _tabLine);
  assert(_tabLine);

  _tabFruit = malloc(20 * sizeof * _tabFruit);
  assert(_tabFruit);

  _visitedId = malloc(255 * sizeof * _visitedId);
  assert(_visitedId);

  for(int i = 0; i < 255; i++){
    _visitedId[i] = -1;
  }
}

void tree_fractale(float posX, float posY, int len, float minSize, float angle, float angle_rotation){
  float newLen;
  float newX, newY;
  float newXLeft, newYLeft,newAngleLeft;
  float newXRight, newYRight, newAngleRight;

  newX = posX - len * cos(angle);
  newY = posY - len * sin(angle);
  
  newLen = len * 0.75;

  if(newLen > _length*0.314){
    _tabLine[_nbrLine].R = 100;
    _tabLine[_nbrLine].G = 30;
    _tabLine[_nbrLine].B = 22;  
  }
  else{
    _tabLine[_nbrLine].R = 30;
    _tabLine[_nbrLine].G = 132;
    _tabLine[_nbrLine].B = 73;
  }

  _tabLine[_nbrLine].beginX = posX; 
  _tabLine[_nbrLine].beginY = posY;
  _tabLine[_nbrLine].endX = newX;
  _tabLine[_nbrLine].endY = newY;

  ++_nbrLine;

  if(minSize <= 0.1){
    minSize = 0.1;
  }
  if(newLen < _length*minSize){
    return;
  }  
  else{
    newAngleLeft = angle + angle_rotation;
    newAngleRight = angle - angle_rotation;
    
    newXLeft = posX + newLen * cos(newAngleLeft);
    newYLeft = posY + newLen * sin(newAngleLeft);
    tree_fractale( newXLeft, newYLeft, newLen, minSize, newAngleLeft, angle_rotation);

    newXRight = posX + newLen * cos(newAngleRight);
    newYRight = posY + newLen * sin(newAngleRight);
    tree_fractale( newXRight, newYRight, newLen, minSize, newAngleRight, angle_rotation);
    
  }
}

int checkIsTerminate(){
  for(int i = 0; i < _nbrFruit ; ++i){
    if(_tabFruit[i].posY > (_tabFruit[i].rayon+10)){
      return 0;
    }
  }
  return 1;
}

void draw(){
  _timeBegin = SDL_GetTicks();
  
  /* init _tabLine */
  if(_state == 0 && _nbrLine < 255){
    tree_fractale( _posX, _posY, _length, _lineMinSize, _angle, _angleRotation);
    ++_state;
    return;
  }

  /* Draw every lines with data in _tabLine */
  if(_state == 1){
    if(_bDraw <= 127){
      gl4dpSetColor(RGB(_tabLine[_bDraw].R, _tabLine[_bDraw].G, _tabLine[_bDraw].B));
      gl4dpLine(_tabLine[_bDraw].beginX, _tabLine[_bDraw].beginY, _tabLine[_bDraw].endX, _tabLine[_bDraw].endY);
      ++_bDraw;
    }
    if(_eDraw > 127){
      gl4dpSetColor(RGB(_tabLine[_eDraw].R, _tabLine[_eDraw].G, _tabLine[_eDraw].B));
      gl4dpLine(_tabLine[_eDraw].beginX, _tabLine[_eDraw].beginY, _tabLine[_eDraw].endX, _tabLine[_eDraw].endY);
      --_eDraw;
        return;
    }
    else {
      ++_state;
      return;
    }
  }

  // Tree growing
  if(_state == 2){
    
    if(_length <= 200 && (_timeBegin - _timeEnd > 50)){
      gl4dpClearScreenWith(_steril);
      for(int i = 0; i < _nbrLine; ++i){
        gl4dpSetColor(RGB(_tabLine[i].R, _tabLine[i].G, _tabLine[i].B));
        gl4dpLine(_tabLine[i].beginX, _tabLine[i].beginY, _tabLine[i].endX, _tabLine[i].endY);
      }
      _nbrLine = 0;
      _length += 1;
      _lineMinSize -= 0.0025;
      tree_fractale( _posX, _posY, _length, _lineMinSize, _angle, _angleRotation);
      _timeEnd = _timeBegin;
      return;
    }
    else if(_length >= 200 && (_timeBegin - _timeEnd > 50)){
      ++_state;
      _timeEnd = _timeBegin;
      return;

    }
  }

  /* Draw fruit with random pos  */
  if(_state == 3){
    int id = rand()%255;
    if(((id > 10 && id < 127) || (id > 140 && id < 255)) && (_timeBegin - _timeEnd > 100)){
      if(_nbrFruit < 20){
        _tabFruit[_nbrFruit].posX = _tabLine[id].endX;
        _tabFruit[_nbrFruit].posY =  _tabLine[id].endY;
        _tabFruit[_nbrFruit].rayon = 2;

        _tabFruit[_nbrFruit].R = 255;
        _tabFruit[_nbrFruit].G = 0;
        _tabFruit[_nbrFruit].B = 0;
        
        gl4dpSetColor(RGB(255, 0, 0));
        gl4dpFilledCircle( _tabFruit[_nbrFruit].posX, _tabFruit[_nbrFruit].posY, _tabFruit[_nbrFruit].rayon);
        ++_nbrFruit;
        _timeEnd = _timeBegin;
        return;
      }
      else{
        ++_state;
        _timeEnd = _timeBegin;
        return;
      }    
    }
  }

  // Frute fall
  if(_state == 4){
    if(checkIsTerminate() == 0){
      if(_timeBegin - _timeEnd > 20){
        gl4dpClearScreenWith(_steril);
        for(int i = 0; i < _nbrLine; ++i){
          gl4dpSetColor(RGB(_tabLine[i].R, _tabLine[i].G, _tabLine[i].B));
          gl4dpLine(_tabLine[i].beginX, _tabLine[i].beginY, _tabLine[i].endX, _tabLine[i].endY);
        }


        for(int i = 0; i < _nbrFruit; ++i){
          if(_tabFruit[i].rayon < 8){
            _tabFruit[i].rayon += 1;
            
          }
          else{
            if(_tabFruit[i].posY > _tabFruit[i].rayon + 10){
              _tabFruit[i].posY -=_itter;
            }
          }
          gl4dpSetColor(RGB(_tabFruit[i].R, _tabFruit[i].G, _tabFruit[i].B));
          gl4dpFilledCircle( _tabFruit[i].posX, _tabFruit[i].posY, _tabFruit[i].rayon);
        }

        _itter += 0.1;
        if(_itter >= 7){
          _itter = 6;
        }
        _timeEnd = _timeBegin;
        return;
      }
    }
    else{
      if(_timeBegin - _timeEnd > 2000){
        ++_state;
        return;
      }
      
    }
  }

  if(_state == 5){
    gl4dpClearScreenWith(_steril);
  }
}

void tree(int state) {
  switch(state) {
    case GL4DH_INIT:
      _screen = gl4dpInitScreen();
      init(gl4dpGetWidth(), gl4dpGetHeight(), 20);
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
