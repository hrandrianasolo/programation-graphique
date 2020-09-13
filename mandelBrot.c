#include <assert.h>
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4dp.h>
#include "audioHelper.h"

static void quit(void);
static void init(int, int);
static void draw(void);

static GLuint _screen;
static GLuint _height, _width, max = 15;
static GLuint _stateXPos = 0, _stateMax = 0;
static float XMIN = -2;
static float XMAX = 0.5;
static float YMIN = -1.25;
static float YMAX = 1.25;

static float xPos = 0;
static float yPos = 0;

static void quit(void) {
  if(_screen) {
    gl4dpSetScreen(_screen);
    gl4dpDeleteScreen();
    _screen = 0;
  }


}

void init(int weight, int height) {
  _height = height;
  _width =  weight;
}
/*

Fonctions pour créer un ensemble de Mendelbrot

*/

void mendel(int max, float xPos, float yPos){

  float xReel, yReel;
  float  cX, cY, tmp_x, tmp_y;
  int itter = 0;

  for(int yRepere = 0; yRepere < _height; ++yRepere){
    for(int xRepere = 0; xRepere < _width; ++xRepere){
      cX = (xRepere * (XMAX - XMIN) / _width + XMIN);
      cY = (yRepere * (YMIN - YMAX) / _height + YMAX);
      xReel = xPos;
      yReel = yPos;
      itter = 0;

      for(; xReel * xReel + yReel * yReel <= 4 && itter < max; ++itter){
        tmp_x = xReel;
        tmp_y = yReel;
        xReel = tmp_x * tmp_x - tmp_y * tmp_y + cX;
        yReel = 2 * tmp_x * tmp_y + cY;

      }

      if (itter == max){
        gl4dpSetColor(RGB(0,0,0));
        gl4dpPutPixel(xRepere, yRepere);
      }
      else{
        gl4dpSetColor(RGB((5 * itter) % 256,(10 * itter) % 256, (18 * itter) % 256));
        gl4dpPutPixel(xRepere, yRepere);
      
      }
    }
  }
}


void draw(){


  mendel(max, xPos, yPos);
  if(max == 35){
    _stateMax = 0;
  }
  if(max == 3){
    _stateMax = 1;   
  }

  if(_stateMax == 0){
    --max;
  }
  if(_stateMax == 1){
    ++max;
  }

  if(_stateXPos == 0){
    xPos += 0.01;
    if(xPos > 2){
      ++_stateXPos;
    }
    return;
  }
  if(_stateXPos == 1){
    xPos -= 0.01;
    if(xPos < 0){
      ++_stateXPos;
    }
    return;
  }

  if(_stateXPos == 2){
    yPos += 0.01;
    xPos += 0.01;
    if(xPos > 1.6){
      _stateXPos++;
    }
    return;
  }

  if(_stateXPos == 3){
    xPos -= 0.01;
    yPos -= 0.01;
    if(xPos < 0){
      _stateXPos =0;
    }
    return;
  }
}

void mandalBrot(int state) {
  switch(state) {
    case GL4DH_INIT:
      _screen = gl4dpInitScreen();
      init(gl4dpGetWidth(), gl4dpGetHeight());
      return;
    case GL4DH_FREE:
      quit();
      gl4dpSetScreen(_screen);
      gl4dpDeleteScreen();
      return;
    case GL4DH_UPDATE_WITH_AUDIO:
      //_basses = ahGetAudioStreamFreq();
      return;
    default:
      gl4dpSetScreen(_screen);
      draw();
      gl4dpUpdateScreen(NULL);
      return;

   }
}
