.386
.model flat,C
.code
extern buf:dword
extern DELTAC:dword
extern CMIN:dword
extern cvals:dword
R equ 2
SHVAL equ 13
MULVAL equ (1 SHL SHVAL)

_juliaintasm proc C uses ebx esi edi ,a_x_re:dword, a_x_im:dword ,c_re:dword,c_im:dword ,d_re:dword ,d_im:dword ,b:dword, mulc:dword ,x:dword,y:dword

	local x_re0,x0:dword
	mov edi,b
	mov eax,a_x_re
	mov x_re0,eax
ly:
	mov eax,x_re0
	mov a_x_re,eax
	mov eax,x
	mov x0,eax
lx:
	mov ebx,a_x_re ;x_re
	mov esi,a_x_im ;x_im
	mov ecx,DELTAC
lc:
	mov eax,ebx
	mov edx,esi
	imul eax,ebx
	imul edx,esi
	imul esi,ebx
	mov ebx,eax
	add eax,edx
	cmp eax,R*R*MULVAL*MULVAL
	jg elc
	shl esi,1
	sar esi,SHVAL
	sub ebx,edx
	add esi,c_im
	sar ebx,SHVAL
	add ebx,c_re
	dec ecx
	jnz lc
	
elc:
	dec ecx
	mov ebx,a_x_re
	imul ecx,mulc
	shr ecx,SHVAL
	add ecx,CMIN
	add ebx,d_re
	and ecx,255
	mov eax,cvals[ecx*4]
	mov a_x_re,ebx
	stosd
	dec x0
	jnz lx

	mov eax,a_x_im
	add eax,d_im
	mov a_x_im,eax
	dec y
	jnz ly
	ret

_juliaintasm endp

end

rem /
 int x,y=SCRHEIGHT,c;
 int x_re,x_im,c_re,c_im,d_re,d_im,x_re2,x_re0,a_x_re,a_x_im;
 int mulc=(int)((float)(CMAX-CMIN)/DELTAC*MULVAL);
 x_re=(int)(fx_re*MULVAL);
 x_im=(int)(fx_im*MULVAL);
 c_re=(int)(fc_re*MULVAL);
 c_im=(int)(fc_im*MULVAL);
 d_re=(int)(fd_re*MULVAL);
 d_im=(int)(fd_im*MULVAL);
 x_re0=x_re;
 a_x_im=x_im;
 unsigned int *b;
 b=(unsigned int *)buf;
 while (y--)
 {
  a_x_re=x_re0;
  x=SCRWIDTH;
  while (x--)
  {
   x_re=a_x_re;
   x_im=a_x_im;
   c=DELTAC;
   while ((c--)&&(x_re*x_re+x_im*x_im<(int)(R*R*MULVAL*MULVAL)))
   {
    x_re2=((x_re*x_re-x_im*x_im)>>SHVAL)+c_re;
    x_im=((2*x_re*x_im)>>SHVAL)+c_im;
    x_re=x_re2;
   }
   c=(c*mulc>>SHVAL)+CMIN;
   *b=cvals[c&255];
   b++;
   a_x_re+=d_re;
  }
  a_x_im+=d_im;
 }/