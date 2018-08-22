.386
kod segment use16
assume cs:kod,ds:dane,ss:stoss
start:
      mov ax,seg dane
      mov ds,ax
      ;
      mov ax,0900h
      mov dx,offset nap
      int 21h
      ;
      mov ax,0900h
      mov dx,offset nap+20
      int 21h      
      mov ax,4c00h
      int 21h
kod ends

dane segment use16
  nap db "Nazywam sie:",13,10,"Kamil Korolkiewicz",13,10,"$"
dane ends

stoss segment use16 stack
db 1024 dup (?)
stoss ends

end start
