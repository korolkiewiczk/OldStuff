#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
  FUNKCJE
*/

float mandelbrot(float x,float y,int rep)
{
 float z_re=0,z_im=0;
 int i=-1;
 while ((i<rep)&&(z_re*z_re+z_im*z_im<4))
 {
  float pom=z_re;
  z_re=z_re*z_re-z_im*z_im+x;
  z_im=2*pom*z_im+y;
  i++;
 }
 return (float)i/(float)rep;
}

float julia(float x,float y,float c_re,float c_im,int rep)
{
 float z_re=x,z_im=y;
 int i=0;
 while ((i<rep)&&(z_re*z_re+z_im*z_im<4))
 {
  float pom=z_re;
  z_re=z_re*z_re-z_im*z_im+c_re;
  z_im=2*pom*z_im+c_im;
  i++;
 }
 return (float)i/(float)rep;
}

float bs(float x,float y,int rep)
{
 float z_re=0,z_im=0;
 int i=-1;
 while ((i<rep)&&(z_re*z_re+z_im*z_im<4))
 {
  float re=fabs(z_re),im=fabs(z_im);
  z_re=re*re-im*im+x;
  z_im=2*re*im+y;
  i++;
 }
 return (float)i/(float)rep;
}

/*
  BIBLIOTEKA FUNKCJI
*/

ELEMENT fractal_lib[]=
{
 {"fractal_mandelbrot",(void*)mandelbrot,VAL_FLOAT,3,VAL_INT*16,0},
 {"fractal_julia",(void*)julia,VAL_FLOAT,5,VAL_INT*256,0},
 {"fractal_bs",(void*)bs,VAL_FLOAT,3,VAL_INT*16,0}
};
