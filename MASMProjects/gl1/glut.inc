ifndef __glut_h__
__glut_h__ = 0

comment  Copyright (c) Mark J. Kilgard, 1994, 1995, 1996, 1998. 

comment  This program is freely distributable without licensing fees  and is
   provided without guarantee or warrantee expressed or  implied. This
;   program is -not- in the public domain. 
wchar_t typedef word

include gl.inc
include glu.inc

comment *
 GLUT API revision history:
 
 GLUT_API_VERSION is updated to reflect incompatible GLUT
 API changes (interface changes, semantic changes, deletions,
 or additions).
 
 GLUT_API_VERSION=1  First public release of GLUT.  11/29/94

 GLUT_API_VERSION=2  Added support for OpenGL/GLX multisampling,
 extension.  Supports new input devices like tablet, dial and button
 box, and Spaceball.  Easy to query OpenGL extensions.

 GLUT_API_VERSION=3  glutMenuStatus added.

 GLUT_API_VERSION=4  glutInitDisplayString, glutWarpPointer,
 glutBitmapLength, glutStrokeLength, glutWindowStatusFunc, dynamic
 video resize subAPI, glutPostWindowRedisplay, glutKeyboardUpFunc,
 glutSpecialUpFunc, glutIgnoreKeyRepeat, glutSetKeyRepeat,
 glutJoystickFunc, glutForceJoystickFunc (NOT FINALIZED not ).
*
ifndef GLUT_API_VERSION
GLUT_API_VERSION = 3
endif

comment *
 GLUT implementation revision history:
 
 GLUT_XLIB_IMPLEMENTATION is updated to reflect both GLUT
 API revisions and implementation revisions (ie, bug fixes).

 GLUT_XLIB_IMPLEMENTATION=1  mjk's first public release of
 GLUT Xlib-based implementation.  11/29/94

 GLUT_XLIB_IMPLEMENTATION=2  mjk's second public release of
 GLUT Xlib-based implementation providing GLUT version 2 
 interfaces.

 GLUT_XLIB_IMPLEMENTATION=3  mjk's GLUT 2.2 images. 4/17/95

 GLUT_XLIB_IMPLEMENTATION=4  mjk's GLUT 2.3 images. 6/?/95

 GLUT_XLIB_IMPLEMENTATION=5  mjk's GLUT 3.0 images. 10/?/95

 GLUT_XLIB_IMPLEMENTATION=7  mjk's GLUT 3.1+ with glutWarpPoitner.  7/24/96

 GLUT_XLIB_IMPLEMENTATION=8  mjk's GLUT 3.1+ with glutWarpPoitner
 and video resize.  1/3/97

 GLUT_XLIB_IMPLEMENTATION=9 mjk's GLUT 3.4 release with early GLUT 4 routines.

 GLUT_XLIB_IMPLEMENTATION=11 Mesa 2.5's GLUT 3.6 release.

 GLUT_XLIB_IMPLEMENTATION=12 mjk's GLUT 3.6 release with early GLUT 4 routines + signal handling.

 GLUT_XLIB_IMPLEMENTATION=13 mjk's GLUT 3.7 release with GameGLUT support.
*
ifndef GLUT_XLIB_IMPLEMENTATION
GLUT_XLIB_IMPLEMENTATION = 13
endif

comment  Display mode bit masks. 
GLUT_RGB = 0
GLUT_RGBA = GLUT_RGB
GLUT_INDEX = 1
GLUT_SINGLE = 0
GLUT_DOUBLE = 2
GLUT_ACCUM = 4
GLUT_ALPHA = 8
GLUT_DEPTH = 16
GLUT_STENCIL = 32
;#if (GLUT_API_VERSION = 2)
GLUT_MULTISAMPLE = 128
GLUT_STEREO = 256

;#if (GLUT_API_VERSION = 3)
GLUT_LUMINANCE = 512


comment  Mouse buttons. 
GLUT_LEFT_BUTTON = 0
GLUT_MIDDLE_BUTTON = 1
GLUT_RIGHT_BUTTON = 2

comment  Mouse button  state. 
GLUT_DOWN = 0
GLUT_UP = 1

;#if (GLUT_API_VERSION = 2)
comment  function keys 
GLUT_KEY_F1 = 1
GLUT_KEY_F2 = 2
GLUT_KEY_F3 = 3
GLUT_KEY_F4 = 4
GLUT_KEY_F5 = 5
GLUT_KEY_F6 = 6
GLUT_KEY_F7 = 7
GLUT_KEY_F8 = 8
GLUT_KEY_F9 = 9
GLUT_KEY_F10 = 10
GLUT_KEY_F11 = 11
GLUT_KEY_F12 = 12
comment  directional keys 
GLUT_KEY_LEFT = 100
GLUT_KEY_UP = 101
GLUT_KEY_RIGHT = 102
GLUT_KEY_DOWN = 103
GLUT_KEY_PAGE_UP = 104
GLUT_KEY_PAGE_DOWN = 105
GLUT_KEY_HOME = 106
GLUT_KEY_END = 107
GLUT_KEY_INSERT = 108


comment  Entry/exit  state. 
GLUT_LEFT = 0
GLUT_ENTERED = 1

comment  Menu usage  state. 
GLUT_MENU_NOT_IN_USE = 0
GLUT_MENU_IN_USE = 1

comment  Visibility  state. 
GLUT_NOT_VISIBLE = 0
GLUT_VISIBLE = 1

comment  Window status  state. 
GLUT_HIDDEN = 0
GLUT_FULLY_RETAINED = 1
GLUT_PARTIALLY_RETAINED = 2
GLUT_FULLY_COVERED = 3

comment  Color index component selection values. 
GLUT_RED = 0
GLUT_GREEN = 1
GLUT_BLUE = 2

comment  Layers for use. 
GLUT_NORMAL = 0
GLUT_OVERLAY = 1

comment  glutGet parameters. 
GLUT_WINDOW_X = 100
GLUT_WINDOW_Y = 101
GLUT_WINDOW_WIDTH = 102
GLUT_WINDOW_HEIGHT = 103
GLUT_WINDOW_BUFFER_SIZE = 104
GLUT_WINDOW_STENCIL_SIZE = 105
GLUT_WINDOW_DEPTH_SIZE = 106
GLUT_WINDOW_RED_SIZE = 107
GLUT_WINDOW_GREEN_SIZE = 108
GLUT_WINDOW_BLUE_SIZE = 109
GLUT_WINDOW_ALPHA_SIZE = 110
GLUT_WINDOW_ACCUM_RED_SIZE = 111
GLUT_WINDOW_ACCUM_GREEN_SIZE = 112
GLUT_WINDOW_ACCUM_BLUE_SIZE = 113
GLUT_WINDOW_ACCUM_ALPHA_SIZE = 114
GLUT_WINDOW_DOUBLEBUFFER = 115
GLUT_WINDOW_RGBA = 116
GLUT_WINDOW_PARENT = 117
GLUT_WINDOW_NUM_CHILDREN = 118
GLUT_WINDOW_COLORMAP_SIZE = 119
;#if (GLUT_API_VERSION = 2)
GLUT_WINDOW_NUM_SAMPLES = 120
GLUT_WINDOW_STEREO = 121

;#if (GLUT_API_VERSION = 3)
GLUT_WINDOW_CURSOR = 122

GLUT_SCREEN_WIDTH = 200
GLUT_SCREEN_HEIGHT = 201
GLUT_SCREEN_WIDTH_MM = 202
GLUT_SCREEN_HEIGHT_MM = 203
GLUT_MENU_NUM_ITEMS = 300
GLUT_DISPLAY_MODE_POSSIBLE = 400
GLUT_INIT_WINDOW_X = 500
GLUT_INIT_WINDOW_Y = 501
GLUT_INIT_WINDOW_WIDTH = 502
GLUT_INIT_WINDOW_HEIGHT = 503
GLUT_INIT_DISPLAY_MODE = 504
;#if (GLUT_API_VERSION = 2)
GLUT_ELAPSED_TIME = 700

;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 13)
GLUT_WINDOW_FORMAT_ID = 123


;#if (GLUT_API_VERSION = 2)
comment  glutDeviceGet parameters. 
GLUT_HAS_KEYBOARD = 600
GLUT_HAS_MOUSE = 601
GLUT_HAS_SPACEBALL = 602
GLUT_HAS_DIAL_AND_BUTTON_BOX = 603
GLUT_HAS_TABLET = 604
GLUT_NUM_MOUSE_BUTTONS = 605
GLUT_NUM_SPACEBALL_BUTTONS = 606
GLUT_NUM_BUTTON_BOX_BUTTONS = 607
GLUT_NUM_DIALS = 608
GLUT_NUM_TABLET_BUTTONS = 609

;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 13)
GLUT_DEVICE_IGNORE_KEY_REPEAT = 610
GLUT_DEVICE_KEY_REPEAT = 611
GLUT_HAS_JOYSTICK = 612
GLUT_OWNS_JOYSTICK = 613
GLUT_JOYSTICK_BUTTONS = 614
GLUT_JOYSTICK_AXES = 615
GLUT_JOYSTICK_POLL_RATE = 616


;#if (GLUT_API_VERSION = 3)
comment  glutLayerGet parameters. 
GLUT_OVERLAY_POSSIBLE = 800
GLUT_LAYER_IN_USE = 801
GLUT_HAS_OVERLAY = 802
GLUT_TRANSPARENT_INDEX = 803
GLUT_NORMAL_DAMAGED = 804
GLUT_OVERLAY_DAMAGED = 805

;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 9)
comment  glutVideoResizeGet parameters. 
GLUT_VIDEO_RESIZE_POSSIBLE = 900
GLUT_VIDEO_RESIZE_IN_USE = 901
GLUT_VIDEO_RESIZE_X_DELTA = 902
GLUT_VIDEO_RESIZE_Y_DELTA = 903
GLUT_VIDEO_RESIZE_WIDTH_DELTA = 904
GLUT_VIDEO_RESIZE_HEIGHT_DELTA = 905
GLUT_VIDEO_RESIZE_X = 906
GLUT_VIDEO_RESIZE_Y = 907
GLUT_VIDEO_RESIZE_WIDTH = 908
GLUT_VIDEO_RESIZE_HEIGHT = 909


comment  glutUseLayer parameters. 
GLUT_NORMAL = 0
GLUT_OVERLAY = 1

comment  glutGetModifiers return mask. 
GLUT_ACTIVE_SHIFT = 1
GLUT_ACTIVE_CTRL = 2
GLUT_ACTIVE_ALT = 4

comment  glutSetCursor parameters. 
comment  Basic arrows. 
GLUT_CURSOR_RIGHT_ARROW = 0
GLUT_CURSOR_LEFT_ARROW = 1
comment  Symbolic cursor shapes. 
GLUT_CURSOR_INFO = 2
GLUT_CURSOR_DESTROY = 3
GLUT_CURSOR_HELP = 4
GLUT_CURSOR_CYCLE = 5
GLUT_CURSOR_SPRAY = 6
GLUT_CURSOR_WAIT = 7
GLUT_CURSOR_TEXT = 8
GLUT_CURSOR_CROSSHAIR = 9
comment  Directional cursors. 
GLUT_CURSOR_UP_DOWN = 10
GLUT_CURSOR_LEFT_RIGHT = 11
comment  Sizing cursors. 
GLUT_CURSOR_TOP_SIDE = 12
GLUT_CURSOR_BOTTOM_SIDE = 13
GLUT_CURSOR_LEFT_SIDE = 14
GLUT_CURSOR_RIGHT_SIDE = 15
GLUT_CURSOR_TOP_LEFT_CORNER = 16
GLUT_CURSOR_TOP_RIGHT_CORNER = 17
GLUT_CURSOR_BOTTOM_RIGHT_CORNER = 18
GLUT_CURSOR_BOTTOM_LEFT_CORNER = 19
comment  Inherit from parent window. 
GLUT_CURSOR_INHERIT = 100
comment  Blank cursor. 
GLUT_CURSOR_NONE = 101
comment  Fullscreen crosshair (if available). 
GLUT_CURSOR_FULL_CROSSHAIR = 102


comment  GLUT initialization sub-API. 
glutInit proto  ,:dword ,:dword
glutInitDisplayMode proto  ,:dword
;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 9)
glutInitDisplayString proto  ,:dword

glutInitWindowPosition proto  ,:dword ,:dword
glutInitWindowSize proto  ,:dword ,:dword
glutMainLoop proto 

comment  GLUT window sub-API. 
glutCreateWindow proto  ,:dword
glutCreateSubWindow proto  ,:dword ,:dword ,:dword ,:dword ,:dword
glutDestroyWindow proto  ,:dword
glutPostRedisplay proto 
;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 11)
glutPostWindowRedisplay proto  ,:dword

glutSwapBuffers proto 
glutGetWindow proto 
glutSetWindow proto  ,:dword
glutSetWindowTitle proto  ,:dword
glutSetIconTitle proto  ,:dword
glutPositionWindow proto  ,:dword ,:dword
glutReshapeWindow proto  ,:dword ,:dword
glutPopWindow proto 
glutPushWindow proto 
glutIconifyWindow proto 
glutShowWindow proto 
glutHideWindow proto 
;#if (GLUT_API_VERSION = 3)
glutFullScreen proto 
glutSetCursor proto  ,:dword
;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 9)
glutWarpPointer proto  ,:dword ,:dword


comment  GLUT overlay sub-API. 
glutEstablishOverlay proto 
glutRemoveOverlay proto 
glutUseLayer proto  ,:GLenum
glutPostOverlayRedisplay proto 
;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 11)
glutPostWindowOverlayRedisplay proto  ,:dword

glutShowOverlay proto 
glutHideOverlay proto 


comment  GLUT menu sub-API. 
glutCreateMenu proto  ,:dword
glutDestroyMenu proto  ,:dword
glutGetMenu proto 
glutSetMenu proto  ,:dword
glutAddMenuEntry proto  ,:dword ,:dword
glutAddSubMenu proto  ,:dword ,:dword
glutChangeToMenuEntry proto  ,:dword ,:dword ,:dword
glutChangeToSubMenu proto  ,:dword ,:dword ,:dword
glutRemoveMenuItem proto  ,:dword
glutAttachMenu proto  ,:dword
glutDetachMenu proto  ,:dword

comment  GLUT window callback sub-API. 
glutDisplayFunc proto  ,:dword
glutReshapeFunc proto  ,:dword
glutKeyboardFunc proto  ,:dword
glutMouseFunc proto  ,:dword
glutMotionFunc proto  ,:dword
glutPassiveMotionFunc proto  ,:dword
glutEntryFunc proto  ,:dword
glutVisibilityFunc proto  ,:dword
glutIdleFunc proto  ,:dword
glutTimerFunc proto  ,:dword ,:dword
glutMenuStateFunc proto  ,:dword
;#if (GLUT_API_VERSION = 2)
glutSpecialFunc proto  ,:dword
glutSpaceballMotionFunc proto  ,:dword
glutSpaceballRotateFunc proto  ,:dword
glutSpaceballButtonFunc proto  ,:dword
glutButtonBoxFunc proto  ,:dword
glutDialsFunc proto  ,:dword
glutTabletMotionFunc proto  ,:dword
glutTabletButtonFunc proto  ,:dword
;#if (GLUT_API_VERSION = 3)
glutMenuStatusFunc proto  ,:dword
glutOverlayDisplayFunc proto  ,:dword
;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 9)
glutWindowStatusFunc proto  ,:dword

;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 13)
glutKeyboardUpFunc proto  ,:dword
glutSpecialUpFunc proto  ,:dword
glutJoystickFunc proto  ,:dword




comment  GLUT color index sub-API. 
glutSetColor proto  ,:dword ,:GLfloat ,:GLfloat ,:GLfloat
glutGetColor proto  ,:dword ,:dword
glutCopyColormap proto  ,:dword

comment  GLUT state retrieval sub-API. 
glutGet proto  ,:GLenum
glutDeviceGet proto  ,:GLenum
;#if (GLUT_API_VERSION = 2)
comment  GLUT extension support sub-API 
glutExtensionSupported proto  ,:dword

;#if (GLUT_API_VERSION = 3)
glutGetModifiers proto 
glutLayerGet proto  ,:GLenum


comment  GLUT font sub-API 
glutBitmapCharacter proto  ,:dword ,:dword
glutBitmapWidth proto  ,:dword ,:dword
glutStrokeCharacter proto  ,:dword ,:dword
glutStrokeWidth proto  ,:dword ,:dword
;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 9)
glutBitmapLength proto  ,:dword ,:dword
glutStrokeLength proto  ,:dword ,:dword


comment  GLUT pre-built models sub-API 
glutWireSphere proto  ,:GLdouble ,:GLint ,:GLint
glutSolidSphere proto  ,:GLdouble ,:GLint ,:GLint
glutWireCone proto  ,:GLdouble ,:GLdouble ,:GLint ,:GLint
glutSolidCone proto  ,:GLdouble ,:GLdouble ,:GLint ,:GLint
glutWireCube proto  ,:GLdouble
glutSolidCube proto  ,:GLdouble
glutWireTorus proto  ,:GLdouble ,:GLdouble ,:GLint ,:GLint
glutSolidTorus proto  ,:GLdouble ,:GLdouble ,:GLint ,:GLint
glutWireDodecahedron proto 
glutSolidDodecahedron proto 
glutWireTeapot proto  ,:GLdouble
glutSolidTeapot proto  ,:GLdouble
glutWireOctahedron proto 
glutSolidOctahedron proto 
glutWireTetrahedron proto 
glutSolidTetrahedron proto 
glutWireIcosahedron proto 
glutSolidIcosahedron proto 

;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 9)
comment  GLUT video resize sub-API. 
glutVideoResizeGet proto  ,:GLenum
glutSetupVideoResizing proto 
glutStopVideoResizing proto 
glutVideoResize proto  ,:dword ,:dword ,:dword ,:dword
glutVideoPan proto  ,:dword ,:dword ,:dword ,:dword

comment  GLUT debugging sub-API. 
glutReportErrors proto 


;#if (GLUT_API_VERSION = 4  or  GLUT_XLIB_IMPLEMENTATION = 13)
comment  GLUT device control sub-API. 
comment  glutSetKeyRepeat modes. 
GLUT_KEY_REPEAT_OFF = 0
GLUT_KEY_REPEAT_ON = 1
GLUT_KEY_REPEAT_DEFAULT = 2

comment  Joystick button masks. 
GLUT_JOYSTICK_BUTTON_A = 1
GLUT_JOYSTICK_BUTTON_B = 2
GLUT_JOYSTICK_BUTTON_C = 4
GLUT_JOYSTICK_BUTTON_D = 8

glutIgnoreKeyRepeat proto  ,:dword
glutSetKeyRepeat proto  ,:dword
glutForceJoystickFunc proto 

comment  GLUT game mode sub-API. 
comment  glutGameModeGet. 
GLUT_GAME_MODE_ACTIVE = 0
GLUT_GAME_MODE_POSSIBLE = 1
GLUT_GAME_MODE_WIDTH = 2
GLUT_GAME_MODE_HEIGHT = 3
GLUT_GAME_MODE_PIXEL_DEPTH = 4
GLUT_GAME_MODE_REFRESH_RATE = 5
GLUT_GAME_MODE_DISPLAY_CHANGED = 6

glutGameModeString proto  ,:dword
glutEnterGameMode proto 
glutLeaveGameMode proto 
glutGameModeGet proto  ,:GLenum

endif

