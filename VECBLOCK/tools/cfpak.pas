program cfpak;

uses strings;

const

flistn='FILELIST.TXT';
maxfn=1024;
maxf=1024;
maxb=8*1024*1024;
sign:array [0..1] of char = ('C','F');

type

tcfhead = record
                fn:array[0..maxfn-1] of char;
                fnd:word;
                fp:longint;
end;

var
tf:file of char;
f:file;
pf:file;
pfn:string;
fn:array[0..maxfn-1] of char;
fnd:word;
fs,fp:longint;
cfhead:array[0..maxf-1] of tcfhead;
i,j,hsize:longint;
fbuf:array[0..maxb-1] of byte;

procedure readl;
var
c:char;
begin
        fnd:=0;
        while(not eof(tf)) do
        begin
                read(tf,c);
                if (c=#13) or (c=#10) then begin read(tf,c); break; end;
                cfhead[i].fn[fnd]:=c;
                inc (fnd);
        end;
        cfhead[i].fn[fnd]:=#0;
        writeln(i+1,':',cfhead[i].fn);
end;

begin
        write ('Podaj nazwe pliku cf:');
        readln (pfn);
        assign (pf,pfn);
        {$I-}
        rewrite (pf,1);
        {$I+}
        if ioresult<>0 then exit;
        assign (tf,flistn);
        {$I-}
        reset (tf,1);
        {$I+}
        if ioresult<>0 then exit;
        fp:=0;
        i:=0;
        hsize:=0;
        while not eof(tf) do
        begin
                readl;
                assign (f,cfhead[i].fn);
                {$I-}
                reset (f,1);
                {$I+}
                if ioresult<>0 then begin writeln('Brak:',cfhead[i].fn); exit; end;
                fs:=filesize(f);
                cfhead[i].fnd:=fnd;
                cfhead[i].fp:=fp;
                hsize:=hsize+fnd+5;
                fp:=fp+fs;
                close (f);
                inc (i);
        end;
        close(tf);
        blockwrite(pf,sign,2);
        blockwrite(pf,i,4);
        hsize:=hsize+6;
        for j:=0 to i-1 do
        begin
                blockwrite (pf,cfhead[j].fn,cfhead[j].fnd+1);
                cfhead[j].fp:=cfhead[j].fp+hsize;
                blockwrite (pf,cfhead[j].fp,4);
        end;
        for j:=0 to i-1 do
        begin
                assign (f,cfhead[j].fn);
                {$I-}
                reset (f,1);
                {$I+}
                if ioresult<>0 then begin writeln('Brak:',cfhead[j].fn); exit; end;
                blockread (f,fbuf,filesize(f));
                blockwrite (pf,fbuf,filesize(f));
                close(f);
        end;
        close (pf);
end.
