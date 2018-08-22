//cmenu.c

void CMenu::AddItem(int parentname,char *name,int id)
{
 char bf[256];
 int l=0,k=0;
 if ((name[0]=='-')&&(name[1]=='-')&&(name[2]=='-')) return;
 while (name[l]>=32) if (name[l]!='&') bf[k++]=name[l++]; else l++;
 bf[k]=0;
 char *b=new char[k+1];
 memcpy(b,bf,k+1);
 name=b;
 if (root==0)
 {
  root=new CMENU_ITEM;
  root->name=name;
  root->id=id;
  root->flags=Unchecked|Enabled;
  root->child=0;
  root->next=0;
  root->prev=0;
 }
 else
 if (parentname==0)
 {
  //find last item
  CMENU_ITEM *i=root;
  while (i->next)
  {
   i=i->next;
  }
  
  i->next=new CMENU_ITEM;
  i->next->prev=i;
  i=i->next;
  i->name=name;
  i->id=id;
  i->flags=Unchecked|Enabled;
  i->child=0;
  i->next=0;
 }
 else
 {
  //find item with parentname
  CMENU_ITEM *i;
  i=Find((char*)parentname);
  if (i==0) return;
  
  //find last item
  if (i->child)
  {
   i=i->child;
   while (i->next)
   {
    i=i->next;
   }
   i->next=new CMENU_ITEM;
   i->next->prev=i;
   i=i->next;
  }
  else
  {
   i->child=new CMENU_ITEM;
   i->child->prev=i;
   i=i->child;
  }
  i->name=name;
  i->id=id;
  i->flags=Unchecked|Enabled;
  i->child=0;
  i->next=0;
 }
}


CMENU_ITEM *CMenu::Find(char *name)
{
 if (name==0) return root;
 CMENU_ITEM *i,*istack[256];
 int sl=-1; 
 istack[++sl]=root;
 while (sl>=0)
 {
  i=istack[sl];
  if ((int)name<0xffff)
  {
   if (i->id==(int)name) break;
  }
  else
  {
   if (strcmp(i->name,name)==0) break;
  }
  sl--;
  if (i->next) istack[++sl]=i->next;
  if (i->child) istack[++sl]=i->child;
 }
 if (sl<0) return 0;
 return i;
}


void CMenu::SetS60(int id,CEikMenuPane* m)
{
 
 CMENU_ITEM *i=CMenu::Find((char *)id);
 if (!i) return;
 CEikMenuPaneItem::SData data;
 TBuf<256> b;
 
 if (id==0)
 {
  CMENU_ITEM *i,*istack[256];
  int sl=-1; 
  istack[++sl]=root;
  while (sl>=0)
  {
   i=istack[sl];
   sl--;
   if (i->next) istack[++sl]=i->next;
   if (i->child)
   {
    istack[++sl]=i->child;
    
    str2desc(i->name,b);
    data.iText=b;
    data.iCommandId=i->id;
    data.iCascadeId=0;
    switch (i->id)
    {
     case MENU_FILE: data.iCascadeId=R_FILE_MENU; break;
   	 case MENU_CONSOLE: data.iCascadeId=R_CON_MENU; break;
   	 case MENU_GRAPH:  data.iCascadeId=R_GRAPH_MENU; break;
     case MENU_SOUND: data.iCascadeId=R_SOUND_MENU; break;
     case MENU_HELP: data.iCascadeId=R_HELP_MENU; break;
     case MENU_FILE_WINDOW: data.iCascadeId=R_FILE_WINDOW_MENU; break;
     case MENU_FILE_DCOLOR: data.iCascadeId=R_FILE_COLOR_MENU; break;
     case MENU_CONSOLE_FONTSIZE: data.iCascadeId=R_CON_FONTSIZE_MENU; break;
     case MENU_GRAPH_DMODE: data.iCascadeId=R_GRAPH_DRAWINGMODE_MENU; break;
     case MENU_GRAPH1: data.iCascadeId=R_GRAPH_3DOPTIONS_MENU; break;
     case MENU_GRAPH2: data.iCascadeId=R_GRAPH_PARAMETRICFUNCTIONOPTIONS_MENU; break;
     case MENU_GRAPH3: data.iCascadeId=R_GRAPH_3DOPTIONS_RENDERER_MENU; break;
     case MENU_GRAPH4: data.iCascadeId=R_GRAPH_PHYSICSMODEL_MENU; break;
    }
    data.iFlags=EEikMenuItemCheckBox;
    data.iExtraText=KNullDesC;
  
    m->AddMenuItemL(data);
  
    if (i->flags&Disabled)
    {
     m->SetItemDimmed(i->id,ETrue);
    }
    else
    if (i->flags&Enabled)
    {
     m->SetItemDimmed(i->id,EFalse);
    }
   }
  }
  return;
 }
 
 i=i->child;
 
 while (i)
 {
  
  if (i->child)
  {
   i=i->next;
   continue;
  }
  
  str2desc(i->name,b);
  data.iText=b;
  data.iCommandId=i->id;
  data.iCascadeId=0;
  data.iFlags=EEikMenuItemCheckBox;
  data.iExtraText=KNullDesC;
  
  m->AddMenuItemL(data);
  
 
  if (i->flags&Disabled)
  {
   m->SetItemDimmed(i->id,ETrue);
  }
  else
  if (i->flags&Enabled)
  {
   m->SetItemDimmed(i->id,EFalse);
  }
  
  if (i->flags&Checked)
  {
   m->SetItemButtonState(i->id,EEikMenuItemSymbolOn);
  }
  else
  if (i->flags&Unchecked)
  {
   m->SetItemButtonState(i->id,EEikMenuItemSymbolIndeterminate);
  }
  
  i=i->next;
 }
}

void CMenu::SetItemInfo(int name,int flag)
{
 CMENU_ITEM *i=Find((char*)name);
 if (i==0) return;
 i->flags=flag;
}
