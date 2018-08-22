//engine.h - generowanie obrazu, ³adowanie mapy

//====================================
//stale
//====================================

//maksymalny rozmiar tablicy w funkcji
#define STACKTABMAX 30000

//stala okreslajaca widocznosc
#define VISVAL -1

//odleglosc od plaszczyzny rzutowania
#define PROJECTDIST ((float)SCRWIDTH/640)*256

//minimalna wspolrzedna y punktu
#define MINDIST 0.1

//====================================
//typy danych
//====================================

#ifdef DOUBLE
typedef double Float;
#define sscanft1 "%lf %lf %lf %lf"
#define sscanft2 "%lf %lf"
#else
typedef float Float;
#define sscanft1 "%f %f %f %f"
#define sscanft2 "%f %f"
#endif

//====================================
//Funkcje matematyczne
//====================================

#include "emath.h"

//====================================
//struktury i funkcje
//====================================

//lista

#define MAXLIST 10000

typedef struct _TList
{
 int a,prev,next;
} TList;

TList list[MAXLIST];
int listwsk=1,listfirst,listlast;

//dodaj do listy przed elementem w(jesli w==0 to na koniec)
//zwraca adres nowego elementu
int List_Add(int w,int a)
{
 if (w==0)
 {
  list[listwsk].prev=listlast;
  if (listlast!=0)
  list[listlast].next=listwsk;
  else listfirst=listwsk;
  listlast=listwsk;
 }
 else
 {
  if (list[w].prev!=0)
  list[list[w].prev].next=listwsk;
  else listfirst=listwsk;
  list[listwsk].prev=list[w].prev;
  list[w].prev=listwsk;
 }
 list[listwsk].next=w;
 list[listwsk].a=a;
 listwsk++;
 return listwsk-1;
}

//usuwa element w i zwraca dana wartosc w liscie
int List_Del(int w)
{
 if (w==0) return -1;
 int prev=list[w].prev;
 if (list[w].prev!=0)
 {
  list[list[w].prev].next=list[w].next;
  list[w].prev=0;
 }
 else listfirst=list[w].next;
 if (list[w].next==0) listlast=prev;
 else
 {
  list[list[w].next].prev=prev;
  list[w].next=0;
 }
 return list[w].a;
}


//====================================

//sterta (heap) gdzie pierwszy element jest najmniejszy

#define MAXHEAP 10000

//[0]-wartosc kluczowa, [1]-dane
int heap[MAXHEAP][2];
int heapwsk;

//dodaje element do stery
void Heap_Add(int a,int d)
{
 int i;
 heapwsk++;
 i=heapwsk;
 while ((i>1)&&(a<=heap[i/2][0]))
 {
  heap[i][0]=heap[i/2][0];
  heap[i][1]=heap[i/2][1];
  i=i/2;
 }
 heap[i][0]=a;
 heap[i][1]=d;
}

//pobiera element ze szczytu sterty
int Heap_Get()
{
 if (heapwsk==0) return -1;
 int r=heap[1][1];
 int pom,pom2;
 heap[1][0]=heap[heapwsk][0];
 heap[1][1]=heap[heapwsk][1];
 heapwsk--;
 int i=1;
 while (1)
 {
  pom=2*i;
  if (pom>heapwsk) break;
  if (pom+1<=heapwsk)
   if (heap[pom][0]>heap[pom+1][0]) pom++;
  if (heap[i][0]<=heap[pom][0]) break;
  pom2=heap[pom][0];
  heap[pom][0]=heap[i][0];
  heap[i][0]=pom2;
  pom2=heap[pom][1];
  heap[pom][1]=heap[i][1];
  heap[i][1]=pom2;
  i=pom;
 }
 
 return r;
}

//====================================

//rysowanie lini w buforze

void Line(int x1,int y1,int x2,int y2,unsigned int col)
{
 //przytnij linie
 if ((((x1<0)&&(x2<0))||((y1<0)&&(y2<0)))||
 (((x1>=SCRWIDTH)&&(x2>=SCRWIDTH))||((y1>=SCRHEIGHT)&&(y2>=SCRHEIGHT)))) return;
 
 if ((x1<0)||(x2<0)||(y1<0)||(y2<0)||
 (x1>=SCRWIDTH)||(x2>=SCRWIDTH)||(y1>=SCRHEIGHT)||(y2>=SCRHEIGHT))
 { 
  if (x1==x2)
  {
   if (y1<0) y1=0;
   if (y2<0) y2=0;
   if (y1>=SCRHEIGHT) y1=SCRHEIGHT-1;
   if (y2>=SCRHEIGHT) y2=SCRHEIGHT-1;
  }
  else
  if (y1==y2)
  {
   if (x1<0) x1=0;
   if (x2<0) x2=0;
   if (x1>=SCRWIDTH) x1=SCRWIDTH-1;
   if (x2>=SCRWIDTH) x2=SCRWIDTH-1;
  }
  else
  {
   Float a,b;
   a=(Float)(y2-y1)/(Float)(x1-x2);
   b=-(Float)y1-a*(Float)x1;
   int left,right,top,bottom;
   left=(int)round(b);
   right=(int)round(a*(SCRWIDTH-1)+b);
   top=(int)round(-b/a);
   bottom=(int)round((-(SCRHEIGHT-1)-b)/a);
   int ok=0;
   
   if ((left<=0)&&(left>=-SCRHEIGHT+1))
   {
    if (x1<0)
    {
     x1=0;
     y1=-left;
    }
    else
    if (x2<0)
    {
     x2=0;
     y2=-left;
    }
    ok=1;
   }
   if ((right<=0)&&(right>=-SCRHEIGHT+1))
   {
    if (x1>=SCRWIDTH)
    {
     x1=SCRWIDTH-1;
     y1=-right;
    }
    else
    if (x2>=SCRWIDTH)
    {
     x2=SCRWIDTH-1;
     y2=-right;
    }
    ok=1;
   }
   if ((top>=0)&&(top<SCRWIDTH))
   {
    if (y1<0)
    {
     x1=top;
     y1=0;
    }
    else
    if (y2<0)
    {
     x2=top;
     y2=0;
    }
    ok=1;
   }
   if ((bottom>=0)&&(bottom<SCRWIDTH))
   {
    if (y1>=SCRHEIGHT)
    {
     x1=bottom;
     y1=SCRHEIGHT-1;
    }
    else
    if (y2>=SCRHEIGHT)
    {
     x2=bottom;
     y2=SCRHEIGHT-1;
    }
    ok=1;
   }
   if (ok==0) return;
  }
 }
 
 int sx=x2-x1,sy=y2-y1;
 int dx1,dy1,dx2,dy2;
 if (sx>0) dx1=1; else if (sx<0) dx1=-1; else dx1=0;
 if (sy>0) dy1=1; else if (sy<0) dy1=-1; else dy1=0;
 int a=abs(sx),b=abs(sy);
 if (a>b)
 {
  dx2=dx1; dy2=0;
 }
 else
 {
  dx2=0; dy2=dy1;
  int pom=a; a=b; b=pom;
 }
 int s=a/2;
 int i;
 for (i=0; i<=a; i++)
 {
  SetBuf32(x1,y1,col);
  s+=b;
  if (s>=a)
  {
   s-=a;
   x1+=dx1;
   y1+=dy1;
  }
  else
  {
   x1+=dx2;
   y1+=dy2;
  }
 }
}

//====================================

//Punkt

typedef struct _TPoint
{
 Float x,y;
} TPoint;

//====================================

//bufor rzutowania

#define MAXPROJECTEDPOINTS 100000

typedef struct _TPB
{
 int x,h1,h2;
} TPB;

TPB pb[MAXPROJECTEDPOINTS];
int pbwsk;


//====================================

//Mapa

/*
  Opis:
  Mapa jest suma punktow podzielonych na sektory. 
  W sektorze dwa kolejne punkty stanowia sciane, kazdy jest
  polaczony ze soba. Pierwszy i ostatni tez sa polaczone.
  Pierwsza linia zawiera kolejno: punkt poczatkowy, kat poczatkowy
  i wysokosc scian
  Sektory rozdzielamy znakiem '#'.
  Kazda para punktow jest w kolejnych liniach. Nie ma rozdzialu miedzy liniami.
*/

typedef struct _TSector
{
 int npoints;
 TPoint *sectorpoints;
 struct _TSector *next;
} TSector;

typedef struct _TMap
{
 TPoint viewpoint;
 Float viewangle;
 Float wallheight;
 TSector *mapsectors;
} TMap;

//wczytuje mape z pliku tekstowego ze skala
void Map_Load(char *fn,TMap *map,Float s)
{
 Float pt[STACKTABMAX][2];
 int i,j;
 char line[256];
 FILE *f;
 f=fopen(fn,"rt");
 if (f==0) return;

 //wczytaj polozenie poczatkowe kamery
 fgets(line,256,f);
 sscanf(line,sscanft1,&map->viewpoint.x,&map->viewpoint.y,&map->viewangle,&map->wallheight);
 map->viewpoint.x*=s;
 map->viewpoint.y*=s;
 
 //alokuj sektory
 TSector *wsks,*poms;
 wsks=0;
 i=0;
 while (!(feof(f)))
 {
  fgets(line,256,f);
  if (line[0]=='#') 
  {
   poms=(TSector*)malloc(sizeof(TSector));
   poms->next=0;
   if (wsks==0)
   {
    map->mapsectors=poms;
   }
   else
   {
    wsks->npoints=j;
    wsks->sectorpoints=(TPoint*)malloc(sizeof(TPoint)*j);
    wsks->next=poms;
   }
   wsks=poms;
   j=0;
  }
  else
  {
   sscanf(line,sscanft2,&pt[i][0],&pt[i][1]);
   i++; j++;
  }
 }
 //ostatni sektor
 wsks->npoints=j;
 wsks->sectorpoints=(TPoint*)malloc(sizeof(TPoint)*j);
 
 fclose(f);
 
 //ustaw punkty w sektorach
 wsks=map->mapsectors;
 i=0;
 while (wsks)
 {
  for (j=0; j<wsks->npoints; j++)
  {
   wsks->sectorpoints[j].x=pt[i][0]*s;
   wsks->sectorpoints[j].y=pt[i][1]*s;
   i++;
  }
  wsks=wsks->next;
 }
}

//====================================

//Renderowanie mapy

//tworzy bufor punktow rzutowania na podstawie mapy
void Create_Project_Buf(TMap *map)
{
 //tablica pomocnicza przechowujaca widoczne sciany (pary punktow)
 TPoint vtab[STACKTABMAX];
 int vtabwsk=0;
 //tablica pomocnicza przechowujaca rzutowane punkty 
 //(kolejno x1,h1,x2,h2 dla sciany oraz adres do komorki w vtab i czy uzyte)
 int ptab[STACKTABMAX][6];
 int ptabwsk=0;
 
 memset(&list,0,listwsk*sizeof(TList));
 memset(&heap,0,heapwsk*sizeof(int));
 listwsk=1; listfirst=0; listlast=0;
 heapwsk=0;
 
 int i,j;
 
 TSector *wsks;
 //odrzuc sciany odwrocone tylem i bedace z tylu kamery
 Float va=Get_a_From_angle(-map->viewangle);
 Float vb=Get_b(va,map->viewpoint.x,map->viewpoint.y);
 Float z;
 int anmod360=(int)map->viewangle+90;
 if (map->viewangle<0) anmod360--;
 anmod360=intMod(anmod360,360);
 if (anmod360<180) z=1; else z=-1;
 wsks=map->mapsectors;
 while (wsks)
 {
  Float x1,y1,x2,y2;
  x1=wsks->sectorpoints[0].x;
  y1=wsks->sectorpoints[0].y;
  for (i=1; i<=wsks->npoints; i++)
  {
   int ok=0;
   x2=wsks->sectorpoints[i%wsks->npoints].x;
   y2=wsks->sectorpoints[i%wsks->npoints].y;
   
   //najpierw sprawdz czy sciana jest za kamera

   if (va==INF)
   {
    if ((x1*z<map->viewpoint.x*z)||(x2*z<map->viewpoint.x*z)) ok=1;
   }
   else
   if ((Get_b(va,x1,y1)*z>vb*z)||(Get_b(va,x2,y2)*z>vb*z))
   //nie jest za kamera
   {
    ok=1;
   }
   
   //sprawdz iloczyn skalarny wektora kamery i sciany
   if (ok==1)
   {
    if (ScalarProduct(y2-y1,x1-x2,x1-map->viewpoint.x,y1-map->viewpoint.y)*VISVAL>0) ok=2;
   }
   
   //dodaj sciane
   if (ok==2) 
   {
    vtab[vtabwsk++]=wsks->sectorpoints[i-1];
    vtab[vtabwsk++]=wsks->sectorpoints[i%wsks->npoints];
   }
   
   
   x1=x2;
   y1=y2;
  }
  wsks=wsks->next;
 }
 
 //transformuj i rzutuj kolejne punkty i dodawaj rzuty do sterty
 for (i=0; i<vtabwsk; i+=2)
 {
  int x1,h1,x2,h2;
  //transformacja
  TransformPoint(vtab[i].x,vtab[i].y,map->viewpoint.x,map->viewpoint.y,map->viewangle,
                 &vtab[i].x,&vtab[i].y);
  TransformPoint(vtab[i+1].x,vtab[i+1].y,map->viewpoint.x,map->viewpoint.y,map->viewangle,
                 &vtab[i+1].x,&vtab[i+1].y);
  
  //przytnij sciane
  if ((vtab[i].y<MINDIST)&&(vtab[i+1].y<MINDIST)) continue;
  if (vtab[i].y<MINDIST)
  {
   vtab[i].x=CutLine(vtab[i].x,vtab[i].y,vtab[i+1].x,vtab[i+1].y,MINDIST);
   vtab[i].y=MINDIST;
  }
  if (vtab[i+1].y<MINDIST)
  {
   vtab[i+1].x=CutLine(vtab[i].x,vtab[i].y,vtab[i+1].x,vtab[i+1].y,MINDIST);
   vtab[i+1].y=MINDIST;
  }
  
  ProjectPoint(vtab[i].x,vtab[i].y,map->wallheight,PROJECTDIST,SCRWIDTH,SCRHEIGHT,&x1,&h1);
  ProjectPoint(vtab[i+1].x,vtab[i+1].y,map->wallheight,PROJECTDIST,SCRWIDTH,SCRHEIGHT,&x2,&h2);
  if (((x1<0)&&(x2<0))||((x1>=SCRWIDTH)&&(x2>=SCRWIDTH))) continue;
  ptab[ptabwsk][0]=x1;
  ptab[ptabwsk][1]=h1;
  ptab[ptabwsk][2]=x2;
  ptab[ptabwsk][3]=h2;
  ptab[ptabwsk][4]=i;
  ptab[ptabwsk][5]=0;
  
  Heap_Add(x1,ptabwsk);
  Heap_Add(x2,ptabwsk);
  
  ptabwsk++;
 }
 
 //tworz tablice pb
 pbwsk=0;
 pb[0].x=1000000000;
 while (1)
 {
  int ad=1;
  j=Heap_Get();
  if (j==-1) break;
  
  //szukaj kolejnych elementow w liscie i sprawdzaj widocznosc
  if (listfirst==0)
  //gdy lista jest pusta
  {
   if (pbwsk>0)
   if (ptab[j][0]==pb[pbwsk-1].x) pbwsk--;
   pb[pbwsk].x=ptab[j][0];
   pb[pbwsk].h1=ptab[j][1];
   pbwsk++;
   ptab[j][5]=List_Add(0,j);
  }
  else
  {
   i=listfirst;
   if (ptab[j][5]!=0) while ((i!=0)&&(list[i].a!=j)) i=list[i].next;
   while (i!=0)
   {
    Float xp,yp;
    if (list[i].a==j)
    //gdy zamykamy sciane
    {
     if (i==listfirst)
     {
      pb[pbwsk].x=ptab[j][2];
      pb[pbwsk].h2=ptab[j][3];
      i=list[i].next;
      if (i!=0)
      {
       int wsk=ptab[list[i].a][4];
       CheckLinesCrossing(vtab[wsk].x,vtab[wsk].y,vtab[wsk+1].x,vtab[wsk+1].y,
                          vtab[ptab[j][4]+1].x,vtab[ptab[j][4]+1].y,&xp,&yp);
       int x,h;
       ProjectPoint(xp,yp,map->wallheight,PROJECTDIST,SCRWIDTH,SCRHEIGHT,&x,&h);
       pb[pbwsk].h1=h;
      } else pb[pbwsk].h1=0;
      pbwsk++;
     }
     ad=0;
    }
    else
    //sprawdz czy punkt jest za/przed sciana
    {
     int wsk=ptab[list[i].a][4];
     int k;
     k=CheckLinesCrossing(vtab[wsk].x,vtab[wsk].y,vtab[wsk+1].x,vtab[wsk+1].y,
                          vtab[ptab[j][4]].x,vtab[ptab[j][4]].y,&xp,&yp);
     if (k==1)
     {
      if (i==listfirst)
      {
       if (pbwsk>0)
       if (ptab[j][0]==pb[pbwsk-1].x) pbwsk--;
       else
       {
        int x,h;
        ProjectPoint(xp,yp,map->wallheight,PROJECTDIST,SCRWIDTH,SCRHEIGHT,&x,&h);
        pb[pbwsk].h2=h;
       }
       pb[pbwsk].x=ptab[j][0];
       pb[pbwsk].h1=ptab[j][1];
       pbwsk++;
      }
      break;
     }
    }
    i=list[i].next;
   }
   if (ad)
   {
    ptab[j][5]=List_Add(i,j);
   }
   else
   List_Del(ptab[j][5]);
  }
 }
}

//rysuje bufor projekcji za pomaca lini
void Render_Project_Buf_Line(int col)
{
 int i,prevx,prevh1,x=0,h1=0,h2;
 for (i=0; i<pbwsk; i++)
 {
  prevx=x;
  prevh1=h1;
  x=pb[i].x;
  h1=pb[i].h1;
  h2=pb[i].h2;
  Line(x,(SCRHEIGHT-MAX(h1,h2))/2,x,(SCRHEIGHT+MAX(h1,h2))/2,col);
  if (h2!=0)
  {
   Line(prevx,(SCRHEIGHT-prevh1)/2,x,(SCRHEIGHT-h2)/2,col);
   Line(prevx,(SCRHEIGHT+prevh1)/2,x,(SCRHEIGHT+h2)/2,col);
  }
 }
}

//sprawdza czy mozna przesunac bez kolizyjnie kamere o wektor [vx,vy] (jesli 0 to tak)
//zwraca tez do vx i vy mozliwy wektor przesuniecia
//mind okresla minimalna odleglosc od sciany
//jesli w==1 to y=ax+b, jesli w==2 to x=ay+b
int CheckCollision(TMap *map,Float *vx,Float *vy,Float mind)
{
 int i;
 //wsp. lini granicznej
 Float la,lb; int lw=0;
 int r=0;
 TSector *wsks;
 wsks=map->mapsectors;
 while (wsks)
 {
  Float x1,y1,x2,y2;
  //dla sciany przesunietej
  Float xs1,ys1,xs2,ys2;
  //wektor prostopadly
  Float px,py;
  x1=wsks->sectorpoints[0].x;
  y1=wsks->sectorpoints[0].y;
  for (i=1; i<=wsks->npoints; i++)
  {
   x2=wsks->sectorpoints[i%wsks->npoints].x;
   y2=wsks->sectorpoints[i%wsks->npoints].y;
   px=y2-y1;
   py=x1-x2;
   if (ScalarProduct(px,py,*vx,*vy)*VISVAL>0)
   {
    Float l=sqrt(px*px+py*py);
    px=mind/l*px;
    py=mind/l*py;
    
    //przesun sciane o [px,py]
    xs1=x1+px;
    ys1=y1+py;
    xs2=x2+px;
    ys2=y2+py;
    
    if (CheckSegmentsCrossing(xs1,ys1,xs2,ys2,
    -*vx+map->viewpoint.x,-*vy+map->viewpoint.y,map->viewpoint.x+*vx,map->viewpoint.y+*vy))
    {
     //znajdz p. przeciecia lini prostopadlej przech. przez koniec wekt. pr.
     Float a,b,bp;
     Float xp,yp;
     int w;
     if (fabs(xs1-xs2)>=fabs(ys1-ys2))
     {
      a=(ys1-ys2)/(xs1-xs2);
      b=ys1-a*xs1;
      bp=map->viewpoint.x+*vx+a*(map->viewpoint.y+*vy);
      xp=(bp-a*b)/(1+a*a);
      yp=a*xp+b;
      w=1;
     }
     else
     {
      a=(xs1-xs2)/(ys1-ys2);
      b=xs1-a*ys1;
      bp=map->viewpoint.y+*vy+a*(map->viewpoint.x+*vx);
      xp=(b+a*bp)/(1+a*a);
      yp=-a*xp+bp;
      w=2;
     }
     
     *vx=xp-map->viewpoint.x;
     *vy=yp-map->viewpoint.y;
     
     /*Float a2,b2;
     int w2;
     if (fabs(*vx)>=fabs(*vy)) 
     {
      a2=*vy/(*vx);
      b2=map->viewpoint.y-a2*(map->viewpoint.x);
      w2=1;
     }
     else
     {
      a2=*vx/(*vy);
      b2=map->viewpoint.x-a2*(map->viewpoint.y);
      w2=2;
     }
     
     if (lw==1)
     {
      if (w2==1)
      {
       xp=(lb-b2)/(a2-la);
       yp=a2*xp+b2;
      }
      else
      {
       xp=(a2*lb+b2)/(1-la*a2);
       yp=la*xp+lb;
      }
     }
     else
     {
      if (w2==1)
      {
       xp=(la*b2+lb)/(1-la*a2);
       yp=a2*xp+b2;
      }
      else
      {
       yp=(lb-b2)/(a2-la);
       xp=a2*yp+b2;
      }
     }
     
     int ok=0;
     if ((xp>=x1)&&(xp<=x2)) ok=1;
     else
     if ((xp<=x1)&&(xp>=x2)) ok=1;
 
     if (ok==1)
     {
     if ((yp>=y1)&&(yp<=y2)) ok=2;
     else
     if ((yp<=y1)&&(yp>=y2)) ok=2;
     }
     
     if (ok==2)
     {
      if (lw==1)
      {
       if (w==1)
       {
        xp=(lb-b)/(a-la);
        yp=a*xp+b;
       }
       else
       {
        xp=(a*lb+b)/(1-la*a);
        yp=la*xp+lb;
       }
      }
      else
      {
       if (w==1)
       {
        xp=(la*b+lb)/(1-la*a);
        yp=a*xp+b;
       }
       else
       {
        yp=(lb-b)/(a-la);
        xp=a*yp+b;
       }
      }
      *vx=xp-map->viewpoint.x;
      *vy=yp-map->viewpoint.y;
     }
     la=a;
     lb=b;
     lw=w;*/
     r=1;
    }
    else
    //przeciecie z okregiem przy wierzcholku
    {
     Float a,b,bp,delta,x,y;
     if (*vx==0) a=INF;
     else
     a=*vy/(*vx);
     b=map->viewpoint.y-a*(map->viewpoint.x);
     x=x1;
     y=y1;
     delta=4*pow(a*b-a*y-x,2)-4*(a*a+1)*(x*x+y*y+b*b-mind*mind-2*b*y);
     if (delta<0)
     {
      x=x2;
      y=y2;
      delta=4*pow(a*b-a*y-x,2)-4*(a*a+1)*(x*x+y*y+b*b-mind*mind-2*b*y);
     }
     if (delta>0)
     {
      Float xc1,yc1,xc2,yc2,xc,yc;
      xc1=(-2*(a*b-a*y-x)-sqrt(delta))/(2*(a*a+1));
      yc1=a*xc1+b;
      xc2=(-2*(a*b-a*y-x)+sqrt(delta))/(2*(a*a+1));
      yc2=a*xc2+b;
      if 
      ((xc1-map->viewpoint.x)*(xc1-map->viewpoint.x)+(yc1-map->viewpoint.y)*(yc1-map->viewpoint.y)
      <
      (xc2-map->viewpoint.x)*(xc2-map->viewpoint.x)+(yc2-map->viewpoint.y)*(yc2-map->viewpoint.y))
      {xc=xc1; yc=yc1;} else {xc=xc2; yc=yc2;}
      
      if ((xc-map->viewpoint.x)*(xc-map->viewpoint.x)+(yc-map->viewpoint.y)*(yc-map->viewpoint.y)
          <(*vx**vx+*vy**vy))
      {
       Float sg;
       if (yc<y) sg=-1; else sg=1;
       
       xc-=x;
       yc-=y;
       if (mind*mind<=xc*xc) a=INF;
       else
       a=-sg*xc/sqrt(mind*mind-xc*xc);
       xc+=x;
       yc+=y;
       b=yc-a*xc;
       bp=map->viewpoint.x+*vx+a*(map->viewpoint.y+*vy);
       Float xp,yp;
       xp=(bp-a*b)/(1+a*a);
       yp=a*xp+b;
       *vx=xp-map->viewpoint.x;
       *vy=yp-map->viewpoint.y;
       r=1;
      }
     }
    }
   }
   x1=x2;
   y1=y2;
  }
  wsks=wsks->next;
 }
 return r;
}

//przesuwa kamere z predkoscia v. Jesli check=1 to sprawdz kolizje
//Jesli byla ruch to zwraca 1;
int MoveViewPoint(TMap *map,Float v,int check)
{
 Float arad=RAD(map->viewangle);
 Float vx=v*sin(arad),vy=v*cos(arad);
 if (check==1) check=CheckCollision(map,&vx,&vy,2);
 map->viewpoint.x+=vx;
 map->viewpoint.y+=vy;
 return check==0;
}
