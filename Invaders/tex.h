//tex.h
#ifndef TEX_H
#define TEX_H

#define MAXMIPMAPS 16

class CTex
{
private:
 int size;
 int shift;
 unsigned int *b[MAXMIPMAPS];
 int bind;
 
public:
 CTex() {size=shift=0; b[0]=0; bind=0;}
 ~CTex();
 int Load(char *);
 void BuildMipMaps();
 void SetAlpha(unsigned int);
 void Bind();
 float Coord(float a) {return a/(float)size;}
};

int LoadImage(char *,unsigned int *&,int &,int &);

#endif
