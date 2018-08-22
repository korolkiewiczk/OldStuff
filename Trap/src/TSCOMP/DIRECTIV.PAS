const
maxdir = 7;
directives : array [0..maxdir-1] of pchar = ('script','endscript','var','endvar','code','wordsize','const');
script=0;
endscript=1;
_var=2;
endvar=3;
code=4;
wordsize=5;
_const=6;