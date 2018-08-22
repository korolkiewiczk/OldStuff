//cmenu.h

#define MENU_CH(a) (((a)>0)? CMenu::Checked:CMenu::Unchecked)
#define MENU_ONOFF(a) (((a)>0)? CMenu::Enabled:CMenu::Disabled)

typedef struct _CMENU_ITEM
{
 char               *name;
 int                id;
 int 				flags;
 struct _CMENU_ITEM *child;
 struct _CMENU_ITEM *next,*prev;
} CMENU_ITEM;

class CMenu
{
 private:
 CMENU_ITEM *root;
 
 public:
 CMenu() {root=0;}
 //if parentname==NULL then adds it to root
 void AddItem(int parentname,char *name,int id); //parentname or id
 //void InsertItem(int prevname,char *name,int id); //prevname or id
 //void DelItem(int name); //name or id
 CMENU_ITEM *Find(char *name);
 CMENU_ITEM *Root() {return root;}
 
 //depends on system
 enum
 {
  Checked=1,
  Unchecked=2,
  Disabled=4,
  Enabled=8
 };
 
 void   SetItemInfo(int name,int flag);
 void   SetS60(int id,CEikMenuPane* m);
};
