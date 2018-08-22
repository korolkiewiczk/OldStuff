const
maxproc = 256;
maxarg = 10;
type
tproc = record
              name : pchar;
              code : byte;
              args : byte;
              arg : array [0..maxarg-1] of byte;
end;
tprocedures = array [0..maxproc-1] of tproc;
