if not exist res.rc goto norc
\masm32\bin\rc res.rc
\masm32\bin\cvtres /machine:ix86 res.res
:norc
\masm32\bin\ml -c -coff -I c:\masm32\wmylibs -Fl a.asm |more
if not exist res.obj goto :noresobj
\masm32\bin\link /MAP /SUBSYSTEM:WINDOWS a.obj res.obj
goto endp
:noresobj
\masm32\bin\link /MAP /SUBSYSTEM:WINDOWS a.obj
:endp
