.model small,C
.data
curx dw 0
cury dw 0
textattr db 7
;
text1 db "Hello world",0
.stack 512
.code
.386

writestr proc s:word
  mov si,s
  mov ax,0b800h
  mov es,ax
  mov ax,cury
  mov bx,160
  mul bx
  mov di,curx
  shl di,1
  add di,ax
  .while 1
    mov al,[si]
    .break .if al==0
    mov ah,textattr
    mov es:[di],ax
    inc di
    inc di
    inc si
  .endw
  ret
writestr endp

begin:
      mov ax,seg _DATA
      mov ds,ax
      mov textattr,0
      mov curx,0
      mov cury,0
      .while cury < 16
      .while curx < 80
        invoke writestr,offset text1
        add curx,11
        inc textattr
      .endw
      mov curx,0
      inc cury
      .endw
      .exit
end begin
