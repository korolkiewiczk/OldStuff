const
pvnil = $80000000;
type
tslist = ^slist;
slist = record
             prev,next : tslist;
             name : pchar;
             val : longint;
end;

procedure createlist (var list : tslist);
begin
     new (list);
     list^.prev := nil;
     list^.next := nil;
     list^.name := nil;
     list^.val := 0;
end;

procedure list_find (var list : tslist; pname : pchar);
begin
     while (1=1) do
     begin
          if strcomp (list^.name,pname) = 0 then exit
          else
          begin
               list := list^.next;
               if list = nil then exit;
          end;
     end;
end;

procedure list_getval (var oval : longint; list : tslist; pname : pchar);
var
pl : tslist;
begin
     pl := list;
     list_find (pl,pname);
     if pl <> nil then
     oval := pl^.val
     else
     oval := pvnil;
end;

procedure list_gotoend (var list : tslist);
begin
     while (1=1) do
     begin
          if list^.next = nil then exit
          else
          list := list^.next;
     end;
end;

procedure list_additem (var list : tslist; pname : pchar; pval : longint);
var
plist,pplist : tslist;
begin
     {0 elem}
     if list^.name=nil then
     begin
          list^.name := pname;
          list^.val := pval;
          exit;
     end;
     {}
     plist := list;
     {}
     list_gotoend (plist);
     new (plist^.next);
     pplist := plist^.next;
     pplist^.prev := plist;
     pplist^.next := nil;
     pplist^.name := pname;
     pplist^.val := pval;
end;