.model small
.386
.stack
.data
napis db "Hello world",13,10,"$"
.code
start:
      mov ax,seg _DATA
      mov ds,ax
      mov eax,00000900h
      mov dx,offset napis
      int 21h
      mov ax,0700h
      int 21h
      mov ah,4ch
      int 21h
end start
