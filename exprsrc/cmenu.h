//cmenu.h

#define MENU_CH(a) (((a)>0)? CMenu::Checked:CMenu::Unchecked)
#define MENU_ONOFF(a) (((a)>0)? CMenu::Enabled:CMenu::Disabled)

typedef struct _CMENU_ITEM
{
 char               *name;
 int                id;
 int                sysref,sysrefchild; //private
 struct _CMENU_ITEM *child;
 struct _CMENU_ITEM *next,*prev;
} CMENU_ITEM;

class CMenu
{
 private:
 CMENU_ITEM *root;
 int syscreated;
 
 public:
 CMenu() {root=0;}
 //if parentname==NULL then adds it to root
 void AddItem(int parentname,char *name,int id); //parentname or id
 //void InsertItem(int prevname,char *name,int id); //prevname or id
 //void DelItem(int name); //name or id
 CMENU_ITEM *Find(char *name);
 
 //depends on system
 enum
 {
  Checked,
  Unchecked,
  Disabled,
  Enabled,
  Default,
  Parent
 };
 void   SetItemInfo(int name,int flag);
 HMENU  AddItemWin32(HMENU m,CMENU_ITEM *i,int pos=-1);
 //void DelItemWin32(CMENU_ITEM *i);
 HMENU  SetWin32(HWND w);
};
