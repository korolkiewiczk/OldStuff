//hash.h

int str2hash(char *s)
{
 const int SHIFT=7;
 const int P=524287;
 int n=strlen(s);
 int i,hs;
 hs=s[0]-33;
 for (i=1; i<n; i++)
  hs=((hs<<SHIFT)+s[i]-33)%P;
 return hs;
}
