#ifndef _ANIMATIONS_H

#define _ANIMATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

  extern void transition_vide(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void fondui(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void fondud(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void animationsInit(void);

  extern void buddhaBrot(int state);
  extern void mandalBrot(int state);
  extern void circle(int state);
  extern void circle_anim(int state);
  extern void tree(int state);
  extern void credits(int state);




#ifdef __cplusplus
}
#endif

#endif
