//cmenu.c

#define AddItem_Sys AddItemWin32
#define DelItem_Sys DelItemWin32

void CMenu::AddItem(int parentname,char *name,int id)
{
 if (root==0)
 {
  root=new CMENU_ITEM;
  root->name=name;
  root->id=id;
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
  
  HMENU m=(HMENU)i->sysref;
  
  i->next=new CMENU_ITEM;
  i->next->prev=i;
  i=i->next;
  i->name=name;
  i->id=id;
  i->child=0;
  i->next=0;
  AddItem_Sys(m,i);
 }
 else
 {
  //find item with parentname
  CMENU_ITEM *i;
  i=Find((char*)parentname);
  if (i==0) return;
  
  HMENU m=0;
  
  //find last item
  if (i->child)
  {
   i=i->child;
   while (i->next)
   {
    i=i->next;
   }
   m=(HMENU)i->sysref;
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
  i->child=0;
  i->next=0;
  AddItem_Sys(m,i);
 }
}

/*
void CMenu::InsertItem(int prevname,char *name,int id)
{
 CMENU_ITEM *i,*n;
 i=Find((char*)prevname);
 if (i==0)
 {
  i=root;
  root=new CMENU_ITEM;
  root->name=name;
  root->id=id;
  root->child=0;
  root->next=i;
  root->prev=0;
  i->prev=root;
  AddItem_Sys((HMENU)i->sysref,root,i->id);
 }
 else
 {
  n=new CMENU_ITEM;
  n->name=name;
  n->id=id;
  n->child=0;
  n->next=i;
  n->prev=i->prev;
  if (i->prev)
  {
   if (i->prev->child==i)
   {
    i->prev->child=n;
   }
   else
   i->prev->next=n;
  }
  i->prev=n;
  AddItem_Sys((HMENU)i->sysref,n,i->id);
 }
}

void CMenu::DelItem(int name)
{
 CMENU_ITEM *i,*n;
 i=Find((char*)name);
 if (!i) return;
 DelItem_Sys(i);
 if (i==root)
 {
  delete root;
  if (i->next)
  {
   root=i->next;
   root->prev=0;
  }
  else
  root=0;
 }
 else
 {
  n=0;
  if (i->prev)
  {
   if (i->prev->child==i)
   {
    i->prev->child=i->next;
   }
   else
   i->prev->next=i->next;
   n=i->prev;
  }
  if (i->next) i->next->prev=n;
  delete i;
 }
}
*/

CMENU_ITEM *CMenu::Find(char *name)
{
 if (name==0) return 0;
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

HMENU CMenu::AddItemWin32(HMENU m,CMENU_ITEM *i,int pos)
{
 if (!i) return 0;
 if (!syscreated) return 0;
 HMENU m2=0;
 MENUITEMINFO mii;
 memset(&mii,0,sizeof(MENUITEMINFO));
 mii.cbSize=sizeof(MENUITEMINFO);
 mii.fMask=MIIM_ID|MIIM_TYPE|MIIM_DATA;
 mii.fType=MFT_STRING;
 mii.dwTypeData=i->name;
 mii.cch=strlen(i->name);
 mii.wID=i->id;
  
 if (mii.cch==3)
 {
  if ((i->name[0]=='-')&&(i->name[1]=='-')&&(i->name[2]=='-'))
  {
   mii.fType&=~MFT_STRING;
   mii.fType|=MFT_SEPARATOR;
  }
 }

 if ((i->child)&&(m!=0))
 {
  m2=CreatePopupMenu();
  mii.fMask|=MIIM_SUBMENU;
  mii.hSubMenu=m2;
  i->sysrefchild=(int)m2;
 }
 if (m==0)
 {
  if (i->prev)
  {
   m=CreatePopupMenu();
   i->prev->sysrefchild=(int)m;
   SetItemInfo(i->prev->id,Parent);
  }
 }
 int mode=1;
 if (pos>=0) mode=0; 
 
 InsertMenuItem(m,pos,mode,&mii);
 
 i->sysref=(int)m;
 return m2;
}

/*
void CMenu::DelItemWin32(CMENU_ITEM *i)
{
 RemoveMenu((HMENU)i->sysref,i->id,MF_BYCOMMAND);
}
*/

HMENU CMenu::SetWin32(HWND w)
{
 CMENU_ITEM *i,*istack[256];
 int sl=-1;
 if (!root) return 0;
 istack[++sl]=root;
 
 syscreated=1;

 HMENU hmenu0;

 hmenu0=CreateMenu();
 
 HMENU hstack[256];
 int hsl=-1;
 hstack[++hsl]=hmenu0;
 while (sl>=0)
 {
  i=istack[sl--];
  if ((int)i==-1) {hsl--; continue;}

  if (i->next) istack[++sl]=i->next; else istack[++sl]=(CMENU_ITEM*)-1;
  if (i->child) istack[++sl]=i->child;
  
  HMENU m2=AddItemWin32(hstack[hsl],i);
  if (m2)
  {
   hstack[++hsl]=m2;
  }

 }
 if (w)
 {
  SetMenu(w,hmenu0);
 }
 return hmenu0;
}

void CMenu::SetItemInfo(int name,int flag)
{
 CMENU_ITEM *i=Find((char*)name);
 if (i==0) return;
 MENUITEMINFO mii;
 memset(&mii,0,sizeof(MENUITEMINFO));
 mii.cbSize=sizeof(MENUITEMINFO);
 mii.fMask=MIIM_STATE;
 GetMenuItemInfo((HMENU)i->sysref,i->id,0,&mii);
 if (flag==Checked)
 {
  mii.fState|=MFS_CHECKED;
  mii.fState&=~MFS_UNCHECKED;
 }
 else
 if (flag==Unchecked)
 {
  mii.fState|=MFS_UNCHECKED;
  mii.fState&=~MFS_CHECKED;
 }
 else
 if (flag==Disabled)
 {
  mii.fState|=MFS_GRAYED;
  mii.fState&=~MFS_ENABLED;
 }
 else
 if (flag==Enabled)
 {
  mii.fState|=MFS_ENABLED;
  mii.fState&=~MFS_GRAYED;
 }
 else
 mii.fState|=(flag==Default)*MFS_DEFAULT;
 
 if (flag==Parent)
 {
  mii.fMask|=MIIM_SUBMENU;
  mii.hSubMenu=(HMENU)i->sysrefchild;
 }
 
 if ((flag==Disabled)||(flag==Enabled))
 {
  if (!i->child)
  SetMenuItemInfo((HMENU)i->sysref,i->id,0,&mii);
  i=i->child;
  while (i)
  {
   SetMenuItemInfo((HMENU)i->sysref,i->id,0,&mii);
   i=i->next;
  }
 }
 else
 SetMenuItemInfo((HMENU)i->sysref,i->id,0,&mii);
}
