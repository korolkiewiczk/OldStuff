.model small
.list
.code

begin:
      .startup
      mov ah,07h
      int 21h
      mov ah,09h
      .if al==27        
        mov dx,offset n1
      .elseif ((al >= 'a')&&(al <= 'z')) || ((al >= 'A')&&(al <= 'Z')) || ((al >= '0')&&(al <= '9'))
        mov dx,offset n3
        mov byte ptr n3,al
      .else    
        mov dx,offset n2       
      .endif
      int 21h
      .exit

.data
n1 db 'esc$'
n2 db 'other$'
n3 db ' $'
.stack 100

end
