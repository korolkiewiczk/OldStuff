.386

public main

stub32 segment use32 public
assume cs:stub32,ds:stub32

main:
      mov ah,07h
      int 21h
      mov ah,09h
      .if al==27        
        mov edx,offset n1
      .elseif ((al >= 'a')&&(al <= 'z')) || ((al >= 'A')&&(al <= 'Z')) || ((al >= '0')&&(al <= '9'))
        mov edx,offset n3
        mov byte ptr n3,al
      .else    
        mov edx,offset n2       
      .endif
      int 21h
      mov ax,4c00h
      int 21h

;dane
n1 db 'esc$'
n2 db 'other$'
n3 db ' $'

stub32 ends

end
