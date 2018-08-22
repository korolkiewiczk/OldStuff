comment ++ BUILD Version: 0004    ; Increment this if a change has global effects

Copyright (c) 1985-95, Microsoft Corporation

Module Name:

    glu.inc

Abstract:

    Procedure declarations, ant definitions and macros for the OpenGL
    Utility Library.

;--

ifndef __glu_h__
ifndef __GLU_H__

__glu_h__ = 0
__GLU_H__ = 0

include gl.inc

comment 
** Copyright 1991-1993, Silicon Graphics, Inc.
** All Rights Reserved.
** 
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of Silicon Graphics, Inc.
** 
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.


comment 
** Return the error string associated with a particular error code.
** This will return 0 for an invalid error code.
**
** The generic function prototype that can be compiled for ANSI or Unicode
** is defined as follows:
**
** LPCTSTR  gluErrorStringWIN (GLenum errCode);


gluErrorString  proto ,:GLenum

gluErrorUnicodeStringEXT  proto ,:GLenum

gluGetString  proto ,:GLenum

gluOrtho2D  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble

gluPerspective  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble

gluPickMatrix  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLint

gluLookAt  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble

gluProject  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLint ,:dword ,:dword ,:dword

gluUnProject  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLint ,:dword ,:dword ,:dword


gluScaleImage  proto


gluBuild1DMipmaps  proto

gluBuild2DMipmaps  proto

ifdef __cplusplus

GLUnurbs struct GLUnurbs ends
GLUquadric struct GLUquadric ends
GLUtesselator struct GLUtesselator ends

comment  backwards compatibility: 
GLUnurbsObj typedef GLUnurbs
GLUquadricObj typedef GLUquadric
GLUtesselatorObj typedef GLUtesselator
GLUtriangulatorObj typedef GLUtesselator

else

GLUnurbs typedef GLUnurbs
GLUquadric typedef GLUquadric
GLUtesselator typedef GLUtesselator

comment  backwards compatibility: 
GLUnurbsObj typedef GLUnurbs
GLUquadricObj typedef GLUquadric
GLUtesselatorObj typedef GLUtesselator
GLUtriangulatorObj typedef GLUtesselator

endif


gluNewQuadric  proto
gluDeleteQuadric  proto ,:dword

gluQuadricNormals  proto ,:dword ,:GLenum

gluQuadricTexture  proto ,:dword ,:GLboolean

gluQuadricOrientation  proto ,:dword ,:GLenum

gluQuadricDrawStyle  proto ,:dword ,:GLenum

gluCylinder  proto ,:dword ,:GLdouble ,:GLdouble ,:GLdouble ,:GLint ,:GLint

gluDisk  proto ,:dword ,:GLdouble ,:GLdouble ,:GLint ,:GLint

gluPartialDisk  proto ,:dword ,:GLdouble ,:GLdouble ,:GLint ,:GLint ,:GLdouble ,:GLdouble

gluSphere  proto ,:dword ,:GLdouble ,:GLint ,:GLint

gluQuadricCallback  proto

gluNewTess proto

gluDeleteTess proto ,:dword

gluTessBeginPolygon proto

gluTessBeginContour proto ,:dword

gluTessVertex proto

gluTessEndContour proto ,:dword

gluTessEndPolygon proto ,:dword

gluTessProperty proto ,:dword ,:GLenum ,:GLdouble
 
gluTessNormal proto ,:dword ,:GLdouble ,:GLdouble ,:GLdouble

gluTessCallback proto

gluGetTessProperty proto ,:dword ,:GLenum ,:dword
 
gluNewNurbsRenderer  proto

gluDeleteNurbsRenderer  proto ,:dword

gluBeginSurface  proto ,:dword

gluBeginCurve  proto ,:dword

gluEndCurve  proto ,:dword

gluEndSurface  proto ,:dword

gluBeginTrim  proto ,:dword

gluEndTrim  proto ,:dword

gluPwlCurve  proto ,:dword ,:GLint ,:dword ,:GLint ,:GLenum

gluNurbsCurve  proto ,:dword ,:GLint ,:dword ,:GLint ,:dword ,:GLint ,:GLenum

;void  
gluNurbsSurface proto ,:dword ,:GLint ,:dword ,:GLint ,:dword ,:GLint ,:GLint ,:dword ,:GLint ,:GLint ,:GLenum

;void  
gluLoadSamplingMatrices  proto ,:dword ,:GLfloat ,:GLfloat ,:GLint

;void  
gluNurbsProperty  proto ,:dword ,:GLenum ,:GLfloat

;void  
gluGetNurbsProperty  proto ,:dword ,:GLenum ,:dword

;void  
gluNurbsCallback  proto


comment ***           Callback function prototypes    ***

comment  gluQuadricCallback 
;typedef void (* GLUquadricErrorProc) (GLenum);

comment  gluTessCallback 
;typedef void (* GLUtessBeginProc)        (GLenum);
;typedef void (* GLUtessEdgeFlagProc)     (GLboolean);
;typedef void (* GLUtessVertexProc)       (void *);
;typedef void (* GLUtessEndProc)          (void);
;typedef void (* GLUtessErrorProc)        (GLenum);
;typedef void (* GLUtessCombineProc)      (GLdouble[3],
;                                                  void*[4], 
;                                                  GLfloat[4],
;                                                  void** );
;typedef void (* GLUtessBeginDataProc)    (GLenum, void *);
;typedef void (* GLUtessEdgeFlagDataProc) (GLboolean, void *);
;typedef void (* GLUtessVertexDataProc)   (void *, void *);
;typedef void (* GLUtessEndDataProc)      (void *);
;typedef void (* GLUtessErrorDataProc)    (GLenum, void *);
;typedef void (* GLUtessCombineDataProc)  (GLdouble[3],
;                                                  void*[4], 
;                                                  GLfloat[4],
;                                                  void**,
;                                                  void* );

comment  gluNurbsCallback 
;typedef void (* GLUnurbsErrorProc)   (GLenum);


comment ***           Generic ants               ***

comment  Version 
GLU_VERSION_1_1 = 1
GLU_VERSION_1_2 = 1

comment  Errors: (return value 0 = no error) 
GLU_INVALID_ENUM = 100900
GLU_INVALID_VALUE = 100901
GLU_OUT_OF_MEMORY = 100902
GLU_INCOMPATIBLE_GL_VERSION = 100903

comment  StringName 
GLU_VERSION = 100800
GLU_EXTENSIONS = 100801

comment  Boolean 
GLU_TRUE = GL_TRUE
GLU_FALSE = GL_FALSE


comment ***           Quadric ants               ***

comment  QuadricNormal 
GLU_SMOOTH = 100000
GLU_FLAT = 100001
GLU_NONE = 100002

comment  QuadricDrawStyle 
GLU_POINT = 100010
GLU_LINE = 100011
GLU_FILL = 100012
GLU_SILHOUETTE = 100013

comment  QuadricOrientation 
GLU_OUTSIDE = 100020
GLU_INSIDE = 100021

comment  Callback types: 
comment       GLU_ERROR               100103 


comment ***           Tesselation ants           ***



_GLU_TESS_MAX_COORD = 1

comment  TessProperty 
GLU_TESS_WINDING_RULE = 100140
GLU_TESS_BOUNDARY_ONLY = 100141
GLU_TESS_TOLERANCE = 100142

comment  TessWinding 
GLU_TESS_WINDING_ODD = 100130
GLU_TESS_WINDING_NONZERO = 100131
GLU_TESS_WINDING_POSITIVE = 100132
GLU_TESS_WINDING_NEGATIVE = 100133
GLU_TESS_WINDING_ABS_GEQ_TWO = 100134

comment  TessCallback 
GLU_TESS_BEGIN = 100100 
GLU_TESS_VERTEX = 100101
GLU_TESS_END = 100102
GLU_TESS_ERROR = 100103
GLU_TESS_EDGE_FLAG = 100104

comment  TessError 
GLU_TESS_ERROR1 = 100151
GLU_TESS_ERROR2 = 100152
GLU_TESS_ERROR3 = 100153
GLU_TESS_ERROR4 = 100154
GLU_TESS_ERROR5 = 100155
GLU_TESS_ERROR6 = 100156
GLU_TESS_ERROR7 = 100157
GLU_TESS_ERROR8 = 100158

GLU_TESS_MISSING_BEGIN_POLYGON = GLU_TESS_ERROR1
GLU_TESS_MISSING_BEGIN_CONTOUR = GLU_TESS_ERROR2
GLU_TESS_MISSING_END_POLYGON = GLU_TESS_ERROR3
GLU_TESS_MISSING_END_CONTOUR = GLU_TESS_ERROR4
GLU_TESS_COORD_TOO_LARGE = GLU_TESS_ERROR5
GLU_TESS_NEED_COMBINE_ = GLU_TESS_ERROR6

comment ***           NURBS ants                 ***

comment  NurbsProperty 
GLU_AUTO_LOAD_MATRIX = 100200
GLU_CULLING = 100201
GLU_SAMPLING_TOLERANCE = 100203
GLU_DISPLAY_MODE = 100204
GLU_PARAMETRIC_TOLERANCE = 100202
GLU_SAMPLING_METHOD = 100205
GLU_U_STEP = 100206
GLU_V_STEP = 100207

comment  NurbsSampling 
GLU_PATH_LENGTH = 100215
GLU_PARAMETRIC_ERROR = 100216
GLU_DOMAIN_DISTANCE = 100217


comment  NurbsTrim 
GLU_MAP1_TRIM_2 = 100210
GLU_MAP1_TRIM_3 = 100211

comment  NurbsDisplay 
comment       GLU_FILL                100012 
GLU_OUTLINE_POLYGON = 100240
GLU_OUTLINE_PATCH = 100241

comment  NurbsCallback 
comment       GLU_ERROR               100103 

comment  NurbsErrors 
GLU_NURBS_ERROR1 = 100251
GLU_NURBS_ERROR2 = 100252
GLU_NURBS_ERROR3 = 100253
GLU_NURBS_ERROR4 = 100254
GLU_NURBS_ERROR5 = 100255
GLU_NURBS_ERROR6 = 100256
GLU_NURBS_ERROR7 = 100257
GLU_NURBS_ERROR8 = 100258
GLU_NURBS_ERROR9 = 100259
GLU_NURBS_ERROR10 = 100260
GLU_NURBS_ERROR11 = 100261
GLU_NURBS_ERROR12 = 100262
GLU_NURBS_ERROR13 = 100263
GLU_NURBS_ERROR14 = 100264
GLU_NURBS_ERROR15 = 100265
GLU_NURBS_ERROR16 = 100266
GLU_NURBS_ERROR17 = 100267
GLU_NURBS_ERROR18 = 100268
GLU_NURBS_ERROR19 = 100269
GLU_NURBS_ERROR20 = 100270
GLU_NURBS_ERROR21 = 100271
GLU_NURBS_ERROR22 = 100272
GLU_NURBS_ERROR23 = 100273
GLU_NURBS_ERROR24 = 100274
GLU_NURBS_ERROR25 = 100275
GLU_NURBS_ERROR26 = 100276
GLU_NURBS_ERROR27 = 100277
GLU_NURBS_ERROR28 = 100278
GLU_NURBS_ERROR29 = 100279
GLU_NURBS_ERROR30 = 100280
GLU_NURBS_ERROR31 = 100281
GLU_NURBS_ERROR32 = 100282
GLU_NURBS_ERROR33 = 100283
GLU_NURBS_ERROR34 = 100284
GLU_NURBS_ERROR35 = 100285
GLU_NURBS_ERROR36 = 100286
GLU_NURBS_ERROR37 = 100287

comment ***           Backwards compatibility for old tesselator           ***

gluBeginPolygon proto ,:dword

gluNextContour proto ,:dword ,:GLenum

gluEndPolygon proto ,:dword

comment  Contours types -- obsolete not  
GLU_CW = 100120
GLU_CCW = 100121
GLU_INTERIOR = 100122
GLU_EXTERIOR = 100123
GLU_UNKNOWN = 100124

comment  Names without TESS_ prefix 
GLU_BEGIN = GLU_TESS_BEGIN
GLU_VERTEX = GLU_TESS_VERTEX
GLU_END = GLU_TESS_END
GLU_ERROR = GLU_TESS_ERROR
GLU_EDGE_FLAG = GLU_TESS_EDGE_FLAG

endif
endif

