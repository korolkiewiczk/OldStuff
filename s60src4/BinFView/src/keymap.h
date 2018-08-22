#define K_SHF                  18
#define K_LSK                  164
#define K_RSK                  165
#define K_ESC                  165
#define K_CALL                 196
#define K_C                    1
#define K_BKSPACE              1
#define K_UP                   16
#define K_DOWN                 17
#define K_LEFT                 14
#define K_RIGHT                15
#define K_ENTER                167
#define K_0                    48
#define K_1                    49
#define K_2                    50
#define K_3                    51
#define K_4                    52
#define K_5                    53
#define K_6                    54
#define K_7                    55
#define K_8                    56
#define K_9                    57
#define K_ASTERISK             42
#define K_CHECK                127 //#



/*#define K_UP                    72
#define K_DOWN                  80
#define K_LEFT                  75
#define K_RIGHT                 77
#define K_BKSPACE               14
#define K_DEL                   83
#define K_HOME                  71
#define K_END                   79
#define K_LSHF                  42
#define K_CTRL                  29
#define K_PGUP                  73
#define K_PGDOWN                81
#define K_TAB                   15
#define K_ESC                    1
#define K_ENTER                 28
#define K_SPACE                 57
#define K_F1                    59
#define K_F2                    60
#define K_F3                    61
#define K_F4                    62
#define K_F5                    63
#define K_F6                    64
#define K_F7                    65
#define K_F8                    66
#define K_F9                    67
#define K_F10                   68
#define K_Q                     16
#define K_W                     17
#define K_E                     18
#define K_R                     19
#define K_T                     20
#define K_Y                     21
#define K_U                     22
#define K_I                     23
#define K_O                     24
#define K_P                     25
#define K_A                     30
#define K_S                     31
#define K_D                     32
#define K_F                     33
#define K_G                     34
#define K_H                     35
#define K_J                     36
#define K_K                     37
#define K_L                     38
#define K_Z                     44
#define K_X                     45
#define K_C                     46
#define K_V                     47
#define K_B                     48
#define K_N                     49
#define K_M                     50
#define K_0                     11
#define K_1                     2
#define K_2                     3
#define K_3                     4
#define K_4                     5
#define K_5                     6
#define K_6                     7
#define K_7                     8
#define K_8                     9
#define K_9                     10
#define K_0                     11
#define K_MINUS                 12
#define K_EQU                   13
#define K_LSBRACKET             26
#define K_RSBRACKET             27
#define K_SEMICOLON             39
#define K_APOSTROPHE            40
#define K_SLASH                 53
#define K_BACKSLASH             43
#define K_TILDE                 41
#define K_EXT0                  112
#define K_EXT1                  113
#define K_EXT2                  114
#define K_EXT3                  115
#define K_EXT4                  116
#define K_EXT5                  117
#define K_EXT6                  118
#define K_EXT7                  119
#define K_EXT8                  120*/

#define KEYMAPLEN 55

unsigned char keymap[KEYMAPLEN][3]=
{
 { 2,'1','!'},{ 3,'2','@'},{ 4,'3','#'},{ 5,'4','$'},{ 6,'5','%'},{ 7,'6','^'},
 { 8,'7','&'},{ 9,'8','*'},{10,'9','('},{11,'0',')'},{12,'-','_'},{13,'=','+'},
 {14,  8,  8},{15,'\t','\t'},{16,'q','Q'},{17,'w','W'},{18,'e','E'},{19,'r','R'},
 {20,'t','T'},{21,'y','Y'},{22,'u','U'},{23,'i','I'},{24,'o','O'},{25,'p','P'},
 {26,'[','{'},{27,']','}'},{28,'\n','\n'},{30,'a','A'},{31,'s','S'},{32,'d','D'},
 {33,'f','F'},{34,'g','G'},{35,'h','H'},{36,'j','J'},{37,'k','K'},{38,'l','L'},
 {39,';',':'},{40,39,'"'},{41,'`','~'},{43,'\\','|'},{44,'z','Z'},{45,'x','X'},
 {46,'c','C'},
 {47,'v','V'},{48,'b','B'},{49,'n','N'},{50,'m','M'},{51,',','<'},{52,'.','>'},
 {53,'/','?'},{55,'*','*'},{57,' ',' '},{74,'-','-'},{78,'+','+'},{83,127,127}
};
