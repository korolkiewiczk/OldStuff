#ifndef __VB_VR_H
#define __VB_VR_H
/*
------------------------------------------------
 VECBLOCK COPYRIGHT (C) KAMIL KOROLKIEWICZ 2006
 Plik: VB_VR.H
 Opis: Obsluga nazw zmiennych - plik naglowkowy
------------------------------------------------
*/

//==========================================

#define VR_MAXCHARS 37
#define VR_VOID 0
#define VR_INT 1
#define VR_FLOAT 2
#define VR_DOUBLE 3

//==========================================

typedef struct _TVarRef
{
 int type;
 void *ref;
} TVarRef;

typedef struct _TVarRefTab
{
 struct _TVarRefTab *namechar[VR_MAXCHARS];
 struct _TVarRefTab *prevchar[VR_MAXCHARS];
 struct _TVarRef *v;
} TVarRefTab;


int InitVR ();
int VR_CIndex (char c);
int VR_New (char *,int,void *);
int VR_Delete(char *);
TVarRef *VR_FindName (char *);
int VR_Set_f (char *,float);
int VR_Set_d (char *,double);
int VR_Set_i (char *,int);
int VR_Set (char *,void*,int);
float VR_Get_f (char *);
double VR_Get_d (char *);
int VR_Get_i (char *);
int VR_Get (char *,void *,int);

#endif
