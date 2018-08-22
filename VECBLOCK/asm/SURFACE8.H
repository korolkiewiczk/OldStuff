#ifndef __SURFACE8_H
#define __SURFACE8_H
//surface8.h dla surface8.inc by Kamil Korolkiewicz 2005/2006

//rysuje 8 bitowa powierzchnie na innej
extern int DrawSurface8(TRect *,TRect *,TSurf *,TSurf *);

//rysuje 8 bitowa powierzchnie na innej w skali
extern int DrawSurface8S(TRect *,TRect *,TSurf *,TSurf *);

//rysuje 8 bitowa przezroczysta powierzchnie na innej
extern int DrawSurface8C(TRect *,TRect *,TSurf *,TSurf *);

//rysuje 8 bitowa przezroczysta powierzchnie na innej w skali
extern int DrawSurface8CS(TRect *,TRect *,TSurf *,TSurf *);

//rysuje 8 bitowa powierzchnie na innej z cieniowaniem
extern int DrawSurface8F(TRect *,TRect *,TSurf *,TSurf *,int);

//rysuje 8 bitowa powierzchnie na innej w skali z cieniowaniem
extern int DrawSurface8FS(TRect *,TRect *,TSurf *,TSurf *,int);

//rysuje 8 bitowa powierzchnie na innej z cieniowaniem i przezroczystoscia
extern int DrawSurface8T(TRect *,TRect *,TSurf *,TSurf *,int,int);

//rysuje 8 bitowa powierzchnie na innej w skali z cieniowaniem i przezroczystoscia
extern int DrawSurface8TS(TRect *,TRect *,TSurf *,TSurf *,int,int);

#endif
