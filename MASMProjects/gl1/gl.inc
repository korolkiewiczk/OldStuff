comment ++ BUILD Version: 0004    ; Increment this if a change has global effects

;Copyright (c) 1985-96, Microsoft Corporation

;Module Name:

;    gl.inc

;Abstract:

;    Procedure declarations, ant definitions and macros for the OpenGL
;    component.

;--

ifndef __gl_h_
ifndef __GL_H__

__gl_h_ = 0
__GL_H__ = 0

comment 
** Copyright 1996 Silicon Graphics, Inc.
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

GLenum typedef dword
GLboolean typedef byte
GLbitfield typedef dword
GLbyte typedef byte
GLshort typedef word
GLint typedef dword
GLsizei typedef dword
GLubyte typedef byte
GLushort typedef word
GLuint typedef dword
GLfloat typedef real4
GLclampf typedef real4
GLdouble typedef qword
GLclampd typedef qword
GLvoid typedef dword

comment ***********************************************************

comment  Version 
GL_VERSION_1_1 = 1

comment  AccumOp 
GL_ACCUM = 00100h
GL_LOAD = 00101h
GL_RETURN = 00102h
GL_MULT = 00103h
GL_ADD = 00104h

comment  AlphaFunction 
GL_NEVER = 00200h
GL_LESS = 00201h
GL_EQUAL = 00202h
GL_LEQUAL = 00203h
GL_GREATER = 00204h
GL_NOTEQUAL = 00205h
GL_GEQUAL = 00206h
GL_ALWAYS = 00207h

comment  AttribMask 
GL_CURRENT_BIT = 000000001h
GL_POINT_BIT = 000000002h
GL_LINE_BIT = 000000004h
GL_POLYGON_BIT = 000000008h
GL_POLYGON_STIPPLE_BIT = 000000010h
GL_PIXEL_MODE_BIT = 000000020h
GL_LIGHTING_BIT = 000000040h
GL_FOG_BIT = 000000080h
GL_DEPTH_BUFFER_BIT = 000000100h
GL_ACCUM_BUFFER_BIT = 000000200h
GL_STENCIL_BUFFER_BIT = 000000400h
GL_VIEWPORT_BIT = 000000800h
GL_TRANSFORM_BIT = 000001000h
GL_ENABLE_BIT = 000002000h
GL_COLOR_BUFFER_BIT = 000004000h
GL_HINT_BIT = 000008000h
GL_EVAL_BIT = 000010000h
GL_LIST_BIT = 000020000h
GL_TEXTURE_BIT = 000040000h
GL_SCISSOR_BIT = 000080000h
GL_ALL_ATTRIB_BITS = 0000FFFFFh

comment  BeginMode 
GL_POINTS = 00000h
GL_LINES = 00001h
GL_LINE_LOOP = 00002h
GL_LINE_STRIP = 00003h
GL_TRIANGLES = 00004h
GL_TRIANGLE_STRIP = 00005h
GL_TRIANGLE_FAN = 00006h
GL_QUADS = 00007h
GL_QUAD_STRIP = 00008h
GL_POLYGON = 00009h

comment  BlendingFactorDest 
GL_ZERO = 0
GL_ONE = 1
GL_SRC_COLOR = 00300h
GL_ONE_MINUS_SRC_COLOR = 00301h
GL_SRC_ALPHA = 00302h
GL_ONE_MINUS_SRC_ALPHA = 00303h
GL_DST_ALPHA = 00304h
GL_ONE_MINUS_DST_ALPHA = 00305h

comment  BlendingFactorSrc 
comment       GL_ZERO 
comment       GL_ONE 
GL_DST_COLOR = 00306h
GL_ONE_MINUS_DST_COLOR = 00307h
GL_SRC_ALPHA_SATURATE = 00308h
comment       GL_SRC_ALPHA 
comment       GL_ONE_MINUS_SRC_ALPHA 
comment       GL_DST_ALPHA 
comment       GL_ONE_MINUS_DST_ALPHA 

comment  Boolean 
GL_TRUE = 1
GL_FALSE = 0

comment  ClearBufferMask 
comment       GL_COLOR_BUFFER_BIT 
comment       GL_ACCUM_BUFFER_BIT 
comment       GL_STENCIL_BUFFER_BIT 
comment       GL_DEPTH_BUFFER_BIT 

comment  ClientArrayType 
comment       GL_VERTEX_ARRAY 
comment       GL_NORMAL_ARRAY 
comment       GL_COLOR_ARRAY 
comment       GL_INDEX_ARRAY 
comment       GL_TEXTURE_COORD_ARRAY 
comment       GL_EDGE_FLAG_ARRAY 

comment  ClipPlaneName 
GL_CLIP_PLANE0 = 03000h
GL_CLIP_PLANE1 = 03001h
GL_CLIP_PLANE2 = 03002h
GL_CLIP_PLANE3 = 03003h
GL_CLIP_PLANE4 = 03004h
GL_CLIP_PLANE5 = 03005h

comment  ColorMaterialFace 
comment       GL_FRONT 
comment       GL_BACK 
comment       GL_FRONT_AND_BACK 

comment  ColorMaterialParameter 
comment       GL_AMBIENT 
comment       GL_DIFFUSE 
comment       GL_SPECULAR 
comment       GL_EMISSION 
comment       GL_AMBIENT_AND_DIFFUSE 

comment  ColorPointerType 
comment       GL_BYTE 
comment       GL_UNSIGNED_BYTE 
comment       GL_SHORT 
comment       GL_UNSIGNED_SHORT 
comment       GL_INT 
comment       GL_UNSIGNED_INT 
comment       GL_FLOAT 
comment       GL_DOUBLE 

comment  CullFaceMode 
comment       GL_FRONT 
comment       GL_BACK 
comment       GL_FRONT_AND_BACK 

comment  DataType 
GL_BYTE = 01400h
GL_UNSIGNED_BYTE = 01401h
GL_SHORT = 01402h
GL_UNSIGNED_SHORT = 01403h
GL_INT = 01404h
GL_UNSIGNED_INT = 01405h
GL_FLOAT = 01406h
GL_2_BYTES = 01407h
GL_3_BYTES = 01408h
GL_4_BYTES = 01409h
GL_DOUBLE = 0140Ah

comment  DepthFunction 
comment       GL_NEVER 
comment       GL_LESS 
comment       GL_EQUAL 
comment       GL_LEQUAL 
comment       GL_GREATER 
comment       GL_NOTEQUAL 
comment       GL_GEQUAL 
comment       GL_ALWAYS 

comment  DrawBufferMode 
GL_NONE = 0
GL_FRONT_LEFT = 00400h
GL_FRONT_RIGHT = 00401h
GL_BACK_LEFT = 00402h
GL_BACK_RIGHT = 00403h
GL_FRONT = 00404h
GL_BACK = 00405h
GL_LEFT = 00406h
GL_RIGHT = 00407h
GL_FRONT_AND_BACK = 00408h
GL_AUX0 = 00409h
GL_AUX1 = 0040Ah
GL_AUX2 = 0040Bh
GL_AUX3 = 0040Ch

comment  Enable 
comment       GL_FOG 
comment       GL_LIGHTING 
comment       GL_TEXTURE_1D 
comment       GL_TEXTURE_2D 
comment       GL_LINE_STIPPLE 
comment       GL_POLYGON_STIPPLE 
comment       GL_CULL_FACE 
comment       GL_ALPHA_TEST 
comment       GL_BLEND 
comment       GL_INDEX_LOGIC_OP 
comment       GL_COLOR_LOGIC_OP 
comment       GL_DITHER 
comment       GL_STENCIL_TEST 
comment       GL_DEPTH_TEST 
comment       GL_CLIP_PLANE0 
comment       GL_CLIP_PLANE1 
comment       GL_CLIP_PLANE2 
comment       GL_CLIP_PLANE3 
comment       GL_CLIP_PLANE4 
comment       GL_CLIP_PLANE5 
comment       GL_LIGHT0 
comment       GL_LIGHT1 
comment       GL_LIGHT2 
comment       GL_LIGHT3 
comment       GL_LIGHT4 
comment       GL_LIGHT5 
comment       GL_LIGHT6 
comment       GL_LIGHT7 
comment       GL_TEXTURE_GEN_S 
comment       GL_TEXTURE_GEN_T 
comment       GL_TEXTURE_GEN_R 
comment       GL_TEXTURE_GEN_Q 
comment       GL_MAP1_VERTEX_3 
comment       GL_MAP1_VERTEX_4 
comment       GL_MAP1_COLOR_4 
comment       GL_MAP1_INDEX 
comment       GL_MAP1_NORMAL 
comment       GL_MAP1_TEXTURE_COORD_1 
comment       GL_MAP1_TEXTURE_COORD_2 
comment       GL_MAP1_TEXTURE_COORD_3 
comment       GL_MAP1_TEXTURE_COORD_4 
comment       GL_MAP2_VERTEX_3 
comment       GL_MAP2_VERTEX_4 
comment       GL_MAP2_COLOR_4 
comment       GL_MAP2_INDEX 
comment       GL_MAP2_NORMAL 
comment       GL_MAP2_TEXTURE_COORD_1 
comment       GL_MAP2_TEXTURE_COORD_2 
comment       GL_MAP2_TEXTURE_COORD_3 
comment       GL_MAP2_TEXTURE_COORD_4 
comment       GL_POINT_SMOOTH 
comment       GL_LINE_SMOOTH 
comment       GL_POLYGON_SMOOTH 
comment       GL_SCISSOR_TEST 
comment       GL_COLOR_MATERIAL 
comment       GL_NORMALIZE 
comment       GL_AUTO_NORMAL 
comment       GL_VERTEX_ARRAY 
comment       GL_NORMAL_ARRAY 
comment       GL_COLOR_ARRAY 
comment       GL_INDEX_ARRAY 
comment       GL_TEXTURE_COORD_ARRAY 
comment       GL_EDGE_FLAG_ARRAY 
comment       GL_POLYGON_OFFSET_POINT 
comment       GL_POLYGON_OFFSET_LINE 
comment       GL_POLYGON_OFFSET_FILL 

comment  ErrorCode 
GL_NO_ERROR = 0
GL_INVALID_ENUM = 00500h
GL_INVALID_VALUE = 00501h
GL_INVALID_OPERATION = 00502h
GL_STACK_OVERFLOW = 00503h
GL_STACK_UNDERFLOW = 00504h
GL_OUT_OF_MEMORY = 00505h

comment  FeedBackMode 
GL_2D = 00600h
GL_3D = 00601h
GL_3D_COLOR = 00602h
GL_3D_COLOR_TEXTURE = 00603h
GL_4D_COLOR_TEXTURE = 00604h

comment  FeedBackToken 
GL_PASS_THROUGH_TOKEN = 00700h
GL_POINT_TOKEN = 00701h
GL_LINE_TOKEN = 00702h
GL_POLYGON_TOKEN = 00703h
GL_BITMAP_TOKEN = 00704h
GL_DRAW_PIXEL_TOKEN = 00705h
GL_COPY_PIXEL_TOKEN = 00706h
GL_LINE_RESET_TOKEN = 00707h

comment  FogMode 
comment       GL_LINEAR 
GL_EXP = 00800h
GL_EXP2 = 00801h


comment  FogParameter 
comment       GL_FOG_COLOR 
comment       GL_FOG_DENSITY 
comment       GL_FOG_END 
comment       GL_FOG_INDEX 
comment       GL_FOG_MODE 
comment       GL_FOG_START 

comment  FrontFaceDirection 
GL_CW = 00900h
GL_CCW = 00901h

comment  GetMapTarget 
GL_COEFF = 00A00h
GL_ORDER = 00A01h
GL_DOMAIN = 00A02h

comment  GetPixelMap 
comment       GL_PIXEL_MAP_I_TO_I 
comment       GL_PIXEL_MAP_S_TO_S 
comment       GL_PIXEL_MAP_I_TO_R 
comment       GL_PIXEL_MAP_I_TO_G 
comment       GL_PIXEL_MAP_I_TO_B 
comment       GL_PIXEL_MAP_I_TO_A 
comment       GL_PIXEL_MAP_R_TO_R 
comment       GL_PIXEL_MAP_G_TO_G 
comment       GL_PIXEL_MAP_B_TO_B 
comment       GL_PIXEL_MAP_A_TO_A 

comment  GetPointerTarget 
comment       GL_VERTEX_ARRAY_POINTER 
comment       GL_NORMAL_ARRAY_POINTER 
comment       GL_COLOR_ARRAY_POINTER 
comment       GL_INDEX_ARRAY_POINTER 
comment       GL_TEXTURE_COORD_ARRAY_POINTER 
comment       GL_EDGE_FLAG_ARRAY_POINTER 

comment  GetTarget 
GL_CURRENT_COLOR = 00B00h
GL_CURRENT_INDEX = 00B01h
GL_CURRENT_NORMAL = 00B02h
GL_CURRENT_TEXTURE_COORDS = 00B03h
GL_CURRENT_RASTER_COLOR = 00B04h
GL_CURRENT_RASTER_INDEX = 00B05h
GL_CURRENT_RASTER_TEXTURE_COORDS = 00B06h
GL_CURRENT_RASTER_POSITION = 00B07h
GL_CURRENT_RASTER_POSITION_VALID = 00B08h
GL_CURRENT_RASTER_DISTANCE = 00B09h
GL_POINT_SMOOTH = 00B10h
GL_POINT_SIZE = 00B11h
GL_POINT_SIZE_RANGE = 00B12h
GL_POINT_SIZE_GRANULARITY = 00B13h
GL_LINE_SMOOTH = 00B20h
GL_LINE_WIDTH = 00B21h
GL_LINE_WIDTH_RANGE = 00B22h
GL_LINE_WIDTH_GRANULARITY = 00B23h
GL_LINE_STIPPLE = 00B24h
GL_LINE_STIPPLE_PATTERN = 00B25h
GL_LINE_STIPPLE_REPEAT = 00B26h
GL_LIST_MODE = 00B30h
GL_MAX_LIST_NESTING = 00B31h
GL_LIST_BASE = 00B32h
GL_LIST_INDEX = 00B33h
GL_POLYGON_MODE = 00B40h
GL_POLYGON_SMOOTH = 00B41h
GL_POLYGON_STIPPLE = 00B42h
GL_EDGE_FLAG = 00B43h
GL_CULL_FACE = 00B44h
GL_CULL_FACE_MODE = 00B45h
GL_FRONT_FACE = 00B46h
GL_LIGHTING = 00B50h
GL_LIGHT_MODEL_LOCAL_VIEWER = 00B51h
GL_LIGHT_MODEL_TWO_SIDE = 00B52h
GL_LIGHT_MODEL_AMBIENT = 00B53h
GL_SHADE_MODEL = 00B54h
GL_COLOR_MATERIAL_FACE = 00B55h
GL_COLOR_MATERIAL_PARAMETER = 00B56h
GL_COLOR_MATERIAL = 00B57h
GL_FOG = 00B60h
GL_FOG_INDEX = 00B61h
GL_FOG_DENSITY = 00B62h
GL_FOG_START = 00B63h
GL_FOG_END = 00B64h
GL_FOG_MODE = 00B65h
GL_FOG_COLOR = 00B66h
GL_DEPTH_RANGE = 00B70h
GL_DEPTH_TEST = 00B71h
GL_DEPTH_WRITEMASK = 00B72h
GL_DEPTH_CLEAR_VALUE = 00B73h
GL_DEPTH_FUNC = 00B74h
GL_ACCUM_CLEAR_VALUE = 00B80h
GL_STENCIL_TEST = 00B90h
GL_STENCIL_CLEAR_VALUE = 00B91h
GL_STENCIL_FUNC = 00B92h
GL_STENCIL_VALUE_MASK = 00B93h
GL_STENCIL_FAIL = 00B94h
GL_STENCIL_PASS_DEPTH_FAIL = 00B95h
GL_STENCIL_PASS_DEPTH_PASS = 00B96h
GL_STENCIL_REF = 00B97h
GL_STENCIL_WRITEMASK = 00B98h
GL_MATRIX_MODE = 00BA0h
GL_NORMALIZE = 00BA1h
GL_VIEWPORT = 00BA2h
GL_MODELVIEW_STACK_DEPTH = 00BA3h
GL_PROJECTION_STACK_DEPTH = 00BA4h
GL_TEXTURE_STACK_DEPTH = 00BA5h
GL_MODELVIEW_MATRIX = 00BA6h
GL_PROJECTION_MATRIX = 00BA7h
GL_TEXTURE_MATRIX = 00BA8h
GL_ATTRIB_STACK_DEPTH = 00BB0h
GL_CLIENT_ATTRIB_STACK_DEPTH = 00BB1h
GL_ALPHA_TEST = 00BC0h
GL_ALPHA_TEST_FUNC = 00BC1h
GL_ALPHA_TEST_REF = 00BC2h
GL_DITHER = 00BD0h
GL_BLEND_DST = 00BE0h
GL_BLEND_SRC = 00BE1h
GL_BLEND = 00BE2h
GL_LOGIC_OP_MODE = 00BF0h
GL_INDEX_LOGIC_OP = 00BF1h
GL_COLOR_LOGIC_OP = 00BF2h
GL_AUX_BUFFERS = 00C00h
GL_DRAW_BUFFER = 00C01h
GL_READ_BUFFER = 00C02h
GL_SCISSOR_BOX = 00C10h
GL_SCISSOR_TEST = 00C11h
GL_INDEX_CLEAR_VALUE = 00C20h
GL_INDEX_WRITEMASK = 00C21h
GL_COLOR_CLEAR_VALUE = 00C22h
GL_COLOR_WRITEMASK = 00C23h
GL_INDEX_MODE = 00C30h
GL_RGBA_MODE = 00C31h
GL_DOUBLEBUFFER = 00C32h
GL_STEREO = 00C33h
GL_RENDER_MODE = 00C40h
GL_PERSPECTIVE_CORRECTION_HINT = 00C50h
GL_POINT_SMOOTH_HINT = 00C51h
GL_LINE_SMOOTH_HINT = 00C52h
GL_POLYGON_SMOOTH_HINT = 00C53h
GL_FOG_HINT = 00C54h
GL_TEXTURE_GEN_S = 00C60h
GL_TEXTURE_GEN_T = 00C61h
GL_TEXTURE_GEN_R = 00C62h
GL_TEXTURE_GEN_Q = 00C63h
GL_PIXEL_MAP_I_TO_I = 00C70h
GL_PIXEL_MAP_S_TO_S = 00C71h
GL_PIXEL_MAP_I_TO_R = 00C72h
GL_PIXEL_MAP_I_TO_G = 00C73h
GL_PIXEL_MAP_I_TO_B = 00C74h
GL_PIXEL_MAP_I_TO_A = 00C75h
GL_PIXEL_MAP_R_TO_R = 00C76h
GL_PIXEL_MAP_G_TO_G = 00C77h
GL_PIXEL_MAP_B_TO_B = 00C78h
GL_PIXEL_MAP_A_TO_A = 00C79h
GL_PIXEL_MAP_I_TO_I_SIZE = 00CB0h
GL_PIXEL_MAP_S_TO_S_SIZE = 00CB1h
GL_PIXEL_MAP_I_TO_R_SIZE = 00CB2h
GL_PIXEL_MAP_I_TO_G_SIZE = 00CB3h
GL_PIXEL_MAP_I_TO_B_SIZE = 00CB4h
GL_PIXEL_MAP_I_TO_A_SIZE = 00CB5h
GL_PIXEL_MAP_R_TO_R_SIZE = 00CB6h
GL_PIXEL_MAP_G_TO_G_SIZE = 00CB7h
GL_PIXEL_MAP_B_TO_B_SIZE = 00CB8h
GL_PIXEL_MAP_A_TO_A_SIZE = 00CB9h
GL_UNPACK_SWAP_BYTES = 00CF0h
GL_UNPACK_LSB_FIRST = 00CF1h
GL_UNPACK_ROW_LENGTH = 00CF2h
GL_UNPACK_SKIP_ROWS = 00CF3h
GL_UNPACK_SKIP_PIXELS = 00CF4h
GL_UNPACK_ALIGNMENT = 00CF5h
GL_PACK_SWAP_BYTES = 00D00h
GL_PACK_LSB_FIRST = 00D01h
GL_PACK_ROW_LENGTH = 00D02h
GL_PACK_SKIP_ROWS = 00D03h
GL_PACK_SKIP_PIXELS = 00D04h
GL_PACK_ALIGNMENT = 00D05h
GL_MAP_COLOR = 00D10h
GL_MAP_STENCIL = 00D11h
GL_INDEX_SHIFT = 00D12h
GL_INDEX_OFFSET = 00D13h
GL_RED_SCALE = 00D14h
GL_RED_BIAS = 00D15h
GL_ZOOM_X = 00D16h
GL_ZOOM_Y = 00D17h
GL_GREEN_SCALE = 00D18h
GL_GREEN_BIAS = 00D19h
GL_BLUE_SCALE = 00D1Ah
GL_BLUE_BIAS = 00D1Bh
GL_ALPHA_SCALE = 00D1Ch
GL_ALPHA_BIAS = 00D1Dh
GL_DEPTH_SCALE = 00D1Eh
GL_DEPTH_BIAS = 00D1Fh
GL_MAX_EVAL_ORDER = 00D30h
GL_MAX_LIGHTS = 00D31h
GL_MAX_CLIP_PLANES = 00D32h
GL_MAX_TEXTURE_SIZE = 00D33h
GL_MAX_PIXEL_MAP_TABLE = 00D34h
GL_MAX_ATTRIB_STACK_DEPTH = 00D35h
GL_MAX_MODELVIEW_STACK_DEPTH = 00D36h
GL_MAX_NAME_STACK_DEPTH = 00D37h
GL_MAX_PROJECTION_STACK_DEPTH = 00D38h
GL_MAX_TEXTURE_STACK_DEPTH = 00D39h
GL_MAX_VIEWPORT_DIMS = 00D3Ah
GL_MAX_CLIENT_ATTRIB_STACK_DEPTH = 00D3Bh
GL_SUBPIXEL_BITS = 00D50h
GL_INDEX_BITS = 00D51h
GL_RED_BITS = 00D52h
GL_GREEN_BITS = 00D53h
GL_BLUE_BITS = 00D54h
GL_ALPHA_BITS = 00D55h
GL_DEPTH_BITS = 00D56h
GL_STENCIL_BITS = 00D57h
GL_ACCUM_RED_BITS = 00D58h
GL_ACCUM_GREEN_BITS = 00D59h
GL_ACCUM_BLUE_BITS = 00D5Ah
GL_ACCUM_ALPHA_BITS = 00D5Bh
GL_NAME_STACK_DEPTH = 00D70h
GL_AUTO_NORMAL = 00D80h
GL_MAP1_COLOR_4 = 00D90h
GL_MAP1_INDEX = 00D91h
GL_MAP1_NORMAL = 00D92h
GL_MAP1_TEXTURE_COORD_1 = 00D93h
GL_MAP1_TEXTURE_COORD_2 = 00D94h
GL_MAP1_TEXTURE_COORD_3 = 00D95h
GL_MAP1_TEXTURE_COORD_4 = 00D96h
GL_MAP1_VERTEX_3 = 00D97h
GL_MAP1_VERTEX_4 = 00D98h
GL_MAP2_COLOR_4 = 00DB0h
GL_MAP2_INDEX = 00DB1h
GL_MAP2_NORMAL = 00DB2h
GL_MAP2_TEXTURE_COORD_1 = 00DB3h
GL_MAP2_TEXTURE_COORD_2 = 00DB4h
GL_MAP2_TEXTURE_COORD_3 = 00DB5h
GL_MAP2_TEXTURE_COORD_4 = 00DB6h
GL_MAP2_VERTEX_3 = 00DB7h
GL_MAP2_VERTEX_4 = 00DB8h
GL_MAP1_GRID_DOMAIN = 00DD0h
GL_MAP1_GRID_SEGMENTS = 00DD1h
GL_MAP2_GRID_DOMAIN = 00DD2h
GL_MAP2_GRID_SEGMENTS = 00DD3h
GL_TEXTURE_1D = 00DE0h
GL_TEXTURE_2D = 00DE1h
GL_FEEDBACK_BUFFER_POINTER = 00DF0h
GL_FEEDBACK_BUFFER_SIZE = 00DF1h
GL_FEEDBACK_BUFFER_TYPE = 00DF2h
GL_SELECTION_BUFFER_POINTER = 00DF3h
GL_SELECTION_BUFFER_SIZE = 00DF4h
comment       GL_TEXTURE_BINDING_1D 
comment       GL_TEXTURE_BINDING_2D 
comment       GL_VERTEX_ARRAY 
comment       GL_NORMAL_ARRAY 
comment       GL_COLOR_ARRAY 
comment       GL_INDEX_ARRAY 
comment       GL_TEXTURE_COORD_ARRAY 
comment       GL_EDGE_FLAG_ARRAY 
comment       GL_VERTEX_ARRAY_SIZE 
comment       GL_VERTEX_ARRAY_TYPE 
comment       GL_VERTEX_ARRAY_STRIDE 
comment       GL_NORMAL_ARRAY_TYPE 
comment       GL_NORMAL_ARRAY_STRIDE 
comment       GL_COLOR_ARRAY_SIZE 
comment       GL_COLOR_ARRAY_TYPE 
comment       GL_COLOR_ARRAY_STRIDE 
comment       GL_INDEX_ARRAY_TYPE 
comment       GL_INDEX_ARRAY_STRIDE 
comment       GL_TEXTURE_COORD_ARRAY_SIZE 
comment       GL_TEXTURE_COORD_ARRAY_TYPE 
comment       GL_TEXTURE_COORD_ARRAY_STRIDE 
comment       GL_EDGE_FLAG_ARRAY_STRIDE 
comment       GL_POLYGON_OFFSET_FACTOR 
comment       GL_POLYGON_OFFSET_UNITS 

comment  GetTextureParameter 
comment       GL_TEXTURE_MAG_FILTER 
comment       GL_TEXTURE_MIN_FILTER 
comment       GL_TEXTURE_WRAP_S 
comment       GL_TEXTURE_WRAP_T 
GL_TEXTURE_WIDTH = 01000h
GL_TEXTURE_HEIGHT = 01001h
GL_TEXTURE_INTERNAL_FORMAT = 01003h
GL_TEXTURE_BORDER_COLOR = 01004h
GL_TEXTURE_BORDER = 01005h
comment       GL_TEXTURE_RED_SIZE 
comment       GL_TEXTURE_GREEN_SIZE 
comment       GL_TEXTURE_BLUE_SIZE 
comment       GL_TEXTURE_ALPHA_SIZE 
comment       GL_TEXTURE_LUMINANCE_SIZE 
comment       GL_TEXTURE_INTENSITY_SIZE 
comment       GL_TEXTURE_PRIORITY 
comment       GL_TEXTURE_RESIDENT 

comment  HintMode 
GL_DONT_CARE = 01100h
GL_FASTEST = 01101h
GL_NICEST = 01102h

comment  HintTarget 
comment       GL_PERSPECTIVE_CORRECTION_HINT 
comment       GL_POINT_SMOOTH_HINT 
comment       GL_LINE_SMOOTH_HINT 
comment       GL_POLYGON_SMOOTH_HINT 
comment       GL_FOG_HINT 
comment       GL_PHONG_HINT 

comment  IndexPointerType 
comment       GL_SHORT 
comment       GL_INT 
comment       GL_FLOAT 
comment       GL_DOUBLE 

comment  LightModelParameter 
comment       GL_LIGHT_MODEL_AMBIENT 
comment       GL_LIGHT_MODEL_LOCAL_VIEWER 
comment       GL_LIGHT_MODEL_TWO_SIDE 

comment  LightName 
GL_LIGHT0 = 04000h
GL_LIGHT1 = 04001h
GL_LIGHT2 = 04002h
GL_LIGHT3 = 04003h
GL_LIGHT4 = 04004h
GL_LIGHT5 = 04005h
GL_LIGHT6 = 04006h
GL_LIGHT7 = 04007h

comment  LightParameter 
GL_AMBIENT = 01200h
GL_DIFFUSE = 01201h
GL_SPECULAR = 01202h
GL_POSITION = 01203h
GL_SPOT_DIRECTION = 01204h
GL_SPOT_EXPONENT = 01205h
GL_SPOT_CUTOFF = 01206h
GL_CONSTANT_ATTENUATION = 01207h
GL_LINEAR_ATTENUATION = 01208h
GL_QUADRATIC_ATTENUATION = 01209h

comment  InterleavedArrays 
comment       GL_V2F 
comment       GL_V3F 
comment       GL_C4UB_V2F 
comment       GL_C4UB_V3F 
comment       GL_C3F_V3F 
comment       GL_N3F_V3F 
comment       GL_C4F_N3F_V3F 
comment       GL_T2F_V3F 
comment       GL_T4F_V4F 
comment       GL_T2F_C4UB_V3F 
comment       GL_T2F_C3F_V3F 
comment       GL_T2F_N3F_V3F 
comment       GL_T2F_C4F_N3F_V3F 
comment       GL_T4F_C4F_N3F_V4F 

comment  ListMode 
GL_COMPILE = 01300h
GL_COMPILE_AND_EXECUTE = 01301h

comment  ListNameType 
comment       GL_BYTE 
comment       GL_UNSIGNED_BYTE 
comment       GL_SHORT 
comment       GL_UNSIGNED_SHORT 
comment       GL_INT 
comment       GL_UNSIGNED_INT 
comment       GL_FLOAT 
comment       GL_2_BYTES 
comment       GL_3_BYTES 
comment       GL_4_BYTES 

comment  LogicOp 
GL_CLEAR = 01500h
GL_AND = 01501h
GL_AND_REVERSE = 01502h
GL_COPY = 01503h
GL_AND_INVERTED = 01504h
GL_NOOP = 01505h
GL_XOR = 01506h
GL_OR = 01507h
GL_NOR = 01508h
GL_EQUIV = 01509h
GL_INVERT = 0150Ah
GL_OR_REVERSE = 0150Bh
GL_COPY_INVERTED = 0150Ch
GL_OR_INVERTED = 0150Dh
GL_NAND = 0150Eh
GL_SET = 0150Fh

comment  MapTarget 
comment       GL_MAP1_COLOR_4 
comment       GL_MAP1_INDEX 
comment       GL_MAP1_NORMAL 
comment       GL_MAP1_TEXTURE_COORD_1 
comment       GL_MAP1_TEXTURE_COORD_2 
comment       GL_MAP1_TEXTURE_COORD_3 
comment       GL_MAP1_TEXTURE_COORD_4 
comment       GL_MAP1_VERTEX_3 
comment       GL_MAP1_VERTEX_4 
comment       GL_MAP2_COLOR_4 
comment       GL_MAP2_INDEX 
comment       GL_MAP2_NORMAL 
comment       GL_MAP2_TEXTURE_COORD_1 
comment       GL_MAP2_TEXTURE_COORD_2 
comment       GL_MAP2_TEXTURE_COORD_3 
comment       GL_MAP2_TEXTURE_COORD_4 
comment       GL_MAP2_VERTEX_3 
comment       GL_MAP2_VERTEX_4 

comment  MaterialFace 
comment       GL_FRONT 
comment       GL_BACK 
comment       GL_FRONT_AND_BACK 

comment  MaterialParameter 
GL_EMISSION = 01600h
GL_SHININESS = 01601h
GL_AMBIENT_AND_DIFFUSE = 01602h
GL_COLOR_INDEXES = 01603h
comment       GL_AMBIENT 
comment       GL_DIFFUSE 
comment       GL_SPECULAR 

comment  MatrixMode 
GL_MODELVIEW = 01700h
GL_PROJECTION = 01701h
GL_TEXTURE = 01702h

comment  MeshMode1 
comment       GL_POINT 
comment       GL_LINE 

comment  MeshMode2 
comment       GL_POINT 
comment       GL_LINE 
comment       GL_FILL 

comment  NormalPointerType 
comment       GL_BYTE 
comment       GL_SHORT 
comment       GL_INT 
comment       GL_FLOAT 
comment       GL_DOUBLE 

comment  PixelCopyType 
GL_COLOR = 01800h
GL_DEPTH = 01801h
GL_STENCIL = 01802h

comment  PixelFormat 
GL_COLOR_INDEX = 01900h
GL_STENCIL_INDEX = 01901h
GL_DEPTH_COMPONENT = 01902h
GL_RED = 01903h
GL_GREEN = 01904h
GL_BLUE = 01905h
GL_ALPHA = 01906h
GL_RGB = 01907h
GL_RGBA = 01908h
GL_LUMINANCE = 01909h
GL_LUMINANCE_ALPHA = 0190Ah

comment  PixelMap 
comment       GL_PIXEL_MAP_I_TO_I 
comment       GL_PIXEL_MAP_S_TO_S 
comment       GL_PIXEL_MAP_I_TO_R 
comment       GL_PIXEL_MAP_I_TO_G 
comment       GL_PIXEL_MAP_I_TO_B 
comment       GL_PIXEL_MAP_I_TO_A 
comment       GL_PIXEL_MAP_R_TO_R 
comment       GL_PIXEL_MAP_G_TO_G 
comment       GL_PIXEL_MAP_B_TO_B 
comment       GL_PIXEL_MAP_A_TO_A 

comment  PixelStore 
comment       GL_UNPACK_SWAP_BYTES 
comment       GL_UNPACK_LSB_FIRST 
comment       GL_UNPACK_ROW_LENGTH 
comment       GL_UNPACK_SKIP_ROWS 
comment       GL_UNPACK_SKIP_PIXELS 
comment       GL_UNPACK_ALIGNMENT 
comment       GL_PACK_SWAP_BYTES 
comment       GL_PACK_LSB_FIRST 
comment       GL_PACK_ROW_LENGTH 
comment       GL_PACK_SKIP_ROWS 
comment       GL_PACK_SKIP_PIXELS 
comment       GL_PACK_ALIGNMENT 

comment  PixelTransfer 
comment       GL_MAP_COLOR 
comment       GL_MAP_STENCIL 
comment       GL_INDEX_SHIFT 
comment       GL_INDEX_OFFSET 
comment       GL_RED_SCALE 
comment       GL_RED_BIAS 
comment       GL_GREEN_SCALE 
comment       GL_GREEN_BIAS 
comment       GL_BLUE_SCALE 
comment       GL_BLUE_BIAS 
comment       GL_ALPHA_SCALE 
comment       GL_ALPHA_BIAS 
comment       GL_DEPTH_SCALE 
comment       GL_DEPTH_BIAS 

comment  PixelType 
GL_BITMAP = 01A00h
comment       GL_BYTE 
comment       GL_UNSIGNED_BYTE 
comment       GL_SHORT 
comment       GL_UNSIGNED_SHORT 
comment       GL_INT 
comment       GL_UNSIGNED_INT 
comment       GL_FLOAT 

comment  PolygonMode 
GL_POINT = 01B00h
GL_LINE = 01B01h
GL_FILL = 01B02h

comment  ReadBufferMode 
comment       GL_FRONT_LEFT 
comment       GL_FRONT_RIGHT 
comment       GL_BACK_LEFT 
comment       GL_BACK_RIGHT 
comment       GL_FRONT 
comment       GL_BACK 
comment       GL_LEFT 
comment       GL_RIGHT 
comment       GL_AUX0 
comment       GL_AUX1 
comment       GL_AUX2 
comment       GL_AUX3 

comment  RenderingMode 
GL_RENDER = 01C00h
GL_FEEDBACK = 01C01h
GL_SELECT = 01C02h

comment  ShadingModel 
GL_FLAT = 01D00h
GL_SMOOTH = 01D01h


comment  StencilFunction 
comment       GL_NEVER 
comment       GL_LESS 
comment       GL_EQUAL 
comment       GL_LEQUAL 
comment       GL_GREATER 
comment       GL_NOTEQUAL 
comment       GL_GEQUAL 
comment       GL_ALWAYS 

comment  StencilOp 
comment       GL_ZERO 
GL_KEEP = 01E00h
GL_REPLACE = 01E01h
GL_INCR = 01E02h
GL_DECR = 01E03h
comment       GL_INVERT 

comment  StringName 
GL_VENDOR = 01F00h
GL_RENDERER = 01F01h
GL_VERSION = 01F02h
GL_EXTENSIONS = 01F03h

comment  TextureCoordName 
GL_S = 02000h
GL_T = 02001h
GL_R = 02002h
GL_Q = 02003h

comment  TexCoordPointerType 
comment       GL_SHORT 
comment       GL_INT 
comment       GL_FLOAT 
comment       GL_DOUBLE 

comment  TextureEnvMode 
GL_MODULATE = 02100h
GL_DECAL = 02101h
comment       GL_BLEND 
comment       GL_REPLACE 

comment  TextureEnvParameter 
GL_TEXTURE_ENV_MODE = 02200h
GL_TEXTURE_ENV_COLOR = 02201h

comment  TextureEnvTarget 
GL_TEXTURE_ENV = 02300h

comment  TextureGenMode 
GL_EYE_LINEAR = 02400h
GL_OBJECT_LINEAR = 02401h
GL_SPHERE_MAP = 02402h

comment  TextureGenParameter 
GL_TEXTURE_GEN_MODE = 02500h
GL_OBJECT_PLANE = 02501h
GL_EYE_PLANE = 02502h

comment  TextureMagFilter 
GL_NEAREST = 02600h
GL_LINEAR = 02601h

comment  TextureMinFilter 
comment       GL_NEAREST 
comment       GL_LINEAR 
GL_NEAREST_MIPMAP_NEAREST = 02700h
GL_LINEAR_MIPMAP_NEAREST = 02701h
GL_NEAREST_MIPMAP_LINEAR = 02702h
GL_LINEAR_MIPMAP_LINEAR = 02703h

comment  TextureParameterName 
GL_TEXTURE_MAG_FILTER = 02800h
GL_TEXTURE_MIN_FILTER = 02801h
GL_TEXTURE_WRAP_S = 02802h
GL_TEXTURE_WRAP_T = 02803h
comment       GL_TEXTURE_BORDER_COLOR 
comment       GL_TEXTURE_PRIORITY 

comment  TextureTarget 
comment       GL_TEXTURE_1D 
comment       GL_TEXTURE_2D 
comment       GL_PROXY_TEXTURE_1D 
comment       GL_PROXY_TEXTURE_2D 

comment  TextureWrapMode 
GL_CLAMP = 02900h
GL_REPEAT = 02901h

comment  VertexPointerType 
comment       GL_SHORT 
comment       GL_INT 
comment       GL_FLOAT 
comment       GL_DOUBLE 

comment  ClientAttribMask 
GL_CLIENT_PIXEL_STORE_BIT = 000000001h
GL_CLIENT_VERTEX_ARRAY_BIT = 000000002h
GL_CLIENT_ALL_ATTRIB_BITS = 0FFFFFFFFh

comment  polygon_offset 
GL_POLYGON_OFFSET_FACTOR = 08038h
GL_POLYGON_OFFSET_UNITS = 02A00h
GL_POLYGON_OFFSET_POINT = 02A01h
GL_POLYGON_OFFSET_LINE = 02A02h
GL_POLYGON_OFFSET_FILL = 08037h

comment  texture 
GL_ALPHA4 = 0803Bh
GL_ALPHA8 = 0803Ch
GL_ALPHA12 = 0803Dh
GL_ALPHA16 = 0803Eh
GL_LUMINANCE4 = 0803Fh
GL_LUMINANCE8 = 08040h
GL_LUMINANCE12 = 08041h
GL_LUMINANCE16 = 08042h
GL_LUMINANCE4_ALPHA4 = 08043h
GL_LUMINANCE6_ALPHA2 = 08044h
GL_LUMINANCE8_ALPHA8 = 08045h
GL_LUMINANCE12_ALPHA4 = 08046h
GL_LUMINANCE12_ALPHA12 = 08047h
GL_LUMINANCE16_ALPHA16 = 08048h
GL_INTENSITY = 08049h
GL_INTENSITY4 = 0804Ah
GL_INTENSITY8 = 0804Bh
GL_INTENSITY12 = 0804Ch
GL_INTENSITY16 = 0804Dh
GL_R3_G3_B2 = 02A10h
GL_RGB4 = 0804Fh
GL_RGB5 = 08050h
GL_RGB8 = 08051h
GL_RGB10 = 08052h
GL_RGB12 = 08053h
GL_RGB16 = 08054h
GL_RGBA2 = 08055h
GL_RGBA4 = 08056h
GL_RGB5_A1 = 08057h
GL_RGBA8 = 08058h
GL_RGB10_A2 = 08059h
GL_RGBA12 = 0805Ah
GL_RGBA16 = 0805Bh
GL_TEXTURE_RED_SIZE = 0805Ch
GL_TEXTURE_GREEN_SIZE = 0805Dh
GL_TEXTURE_BLUE_SIZE = 0805Eh
GL_TEXTURE_ALPHA_SIZE = 0805Fh
GL_TEXTURE_LUMINANCE_SIZE = 08060h
GL_TEXTURE_INTENSITY_SIZE = 08061h
GL_PROXY_TEXTURE_1D = 08063h
GL_PROXY_TEXTURE_2D = 08064h

comment  texture_object 
GL_TEXTURE_PRIORITY = 08066h
GL_TEXTURE_RESIDENT = 08067h
GL_TEXTURE_BINDING_1D = 08068h
GL_TEXTURE_BINDING_2D = 08069h

comment  vertex_array 
GL_VERTEX_ARRAY = 08074h
GL_NORMAL_ARRAY = 08075h
GL_COLOR_ARRAY = 08076h
GL_INDEX_ARRAY = 08077h
GL_TEXTURE_COORD_ARRAY = 08078h
GL_EDGE_FLAG_ARRAY = 08079h
GL_VERTEX_ARRAY_SIZE = 0807Ah
GL_VERTEX_ARRAY_TYPE = 0807Bh
GL_VERTEX_ARRAY_STRIDE = 0807Ch
GL_NORMAL_ARRAY_TYPE = 0807Eh
GL_NORMAL_ARRAY_STRIDE = 0807Fh
GL_COLOR_ARRAY_SIZE = 08081h
GL_COLOR_ARRAY_TYPE = 08082h
GL_COLOR_ARRAY_STRIDE = 08083h
GL_INDEX_ARRAY_TYPE = 08085h
GL_INDEX_ARRAY_STRIDE = 08086h
GL_TEXTURE_COORD_ARRAY_SIZE = 08088h
GL_TEXTURE_COORD_ARRAY_TYPE = 08089h
GL_TEXTURE_COORD_ARRAY_STRIDE = 0808Ah
GL_EDGE_FLAG_ARRAY_STRIDE = 0808Ch
GL_VERTEX_ARRAY_POINTER = 0808Eh
GL_NORMAL_ARRAY_POINTER = 0808Fh
GL_COLOR_ARRAY_POINTER = 08090h
GL_INDEX_ARRAY_POINTER = 08091h
GL_TEXTURE_COORD_ARRAY_POINTER = 08092h
GL_EDGE_FLAG_ARRAY_POINTER = 08093h
GL_V2F = 02A20h
GL_V3F = 02A21h
GL_C4UB_V2F = 02A22h
GL_C4UB_V3F = 02A23h
GL_C3F_V3F = 02A24h
GL_N3F_V3F = 02A25h
GL_C4F_N3F_V3F = 02A26h
GL_T2F_V3F = 02A27h
GL_T4F_V4F = 02A28h
GL_T2F_C4UB_V3F = 02A29h
GL_T2F_C3F_V3F = 02A2Ah
GL_T2F_N3F_V3F = 02A2Bh
GL_T2F_C4F_N3F_V3F = 02A2Ch
GL_T4F_C4F_N3F_V4F = 02A2Dh

comment  Extensions 
GL_EXT_vertex_array = 1
GL_EXT_bgra = 1
GL_EXT_paletted_texture = 1
GL_WIN_swap_hint = 1
GL_WIN_draw_range_elements = 1
GL_WIN_phong_shading = 1
GL_WIN_specular_fog = 1

comment  EXT_vertex_array 
GL_VERTEX_ARRAY_EXT = 08074h
GL_NORMAL_ARRAY_EXT = 08075h
GL_COLOR_ARRAY_EXT = 08076h
GL_INDEX_ARRAY_EXT = 08077h
GL_TEXTURE_COORD_ARRAY_EXT = 08078h
GL_EDGE_FLAG_ARRAY_EXT = 08079h
GL_VERTEX_ARRAY_SIZE_EXT = 0807Ah
GL_VERTEX_ARRAY_TYPE_EXT = 0807Bh
GL_VERTEX_ARRAY_STRIDE_EXT = 0807Ch
GL_VERTEX_ARRAY_COUNT_EXT = 0807Dh
GL_NORMAL_ARRAY_TYPE_EXT = 0807Eh
GL_NORMAL_ARRAY_STRIDE_EXT = 0807Fh
GL_NORMAL_ARRAY_COUNT_EXT = 08080h
GL_COLOR_ARRAY_SIZE_EXT = 08081h
GL_COLOR_ARRAY_TYPE_EXT = 08082h
GL_COLOR_ARRAY_STRIDE_EXT = 08083h
GL_COLOR_ARRAY_COUNT_EXT = 08084h
GL_INDEX_ARRAY_TYPE_EXT = 08085h
GL_INDEX_ARRAY_STRIDE_EXT = 08086h
GL_INDEX_ARRAY_COUNT_EXT = 08087h
GL_TEXTURE_COORD_ARRAY_SIZE_EXT = 08088h
GL_TEXTURE_COORD_ARRAY_TYPE_EXT = 08089h
GL_TEXTURE_COORD_ARRAY_STRIDE_EXT = 0808Ah
GL_TEXTURE_COORD_ARRAY_COUNT_EXT = 0808Bh
GL_EDGE_FLAG_ARRAY_STRIDE_EXT = 0808Ch
GL_EDGE_FLAG_ARRAY_COUNT_EXT = 0808Dh
GL_VERTEX_ARRAY_POINTER_EXT = 0808Eh
GL_NORMAL_ARRAY_POINTER_EXT = 0808Fh
GL_COLOR_ARRAY_POINTER_EXT = 08090h
GL_INDEX_ARRAY_POINTER_EXT = 08091h
GL_TEXTURE_COORD_ARRAY_POINTER_EXT = 08092h
GL_EDGE_FLAG_ARRAY_POINTER_EXT = 08093h
GL_DOUBLE_EXT = GL_DOUBLE

comment  EXT_bgra 
GL_BGR_EXT = 080E0h
GL_BGRA_EXT = 080E1h

comment  EXT_paletted_texture 

comment  These must match the GL_COLOR_TABLE_*_SGI enumerants 
GL_COLOR_TABLE_FORMAT_EXT = 080D8h
GL_COLOR_TABLE_WIDTH_EXT = 080D9h
GL_COLOR_TABLE_RED_SIZE_EXT = 080DAh
GL_COLOR_TABLE_GREEN_SIZE_EXT = 080DBh
GL_COLOR_TABLE_BLUE_SIZE_EXT = 080DCh
GL_COLOR_TABLE_ALPHA_SIZE_EXT = 080DDh
GL_COLOR_TABLE_LUMINANCE_SIZE_EXT = 080DEh
GL_COLOR_TABLE_INTENSITY_SIZE_EXT = 080DFh

GL_COLOR_INDEX1_EXT = 080E2h
GL_COLOR_INDEX2_EXT = 080E3h
GL_COLOR_INDEX4_EXT = 080E4h
GL_COLOR_INDEX8_EXT = 080E5h
GL_COLOR_INDEX12_EXT = 080E6h
GL_COLOR_INDEX16_EXT = 080E7h

comment  WIN_draw_range_elements 
GL_MAX_ELEMENTS_VERTICES_WIN = 080E8h
GL_MAX_ELEMENTS_INDICES_WIN = 080E9h

comment  WIN_phong_shading 
GL_PHONG_WIN = 080EAh 
GL_PHONG_HINT_WIN = 080EBh 

comment  WIN_specular_fog 
GL_FOG_SPECULAR_TEXTURE_WIN = 080ECh

comment  For compatibility with OpenGL v1.0 
GL_LOGIC_OP = GL_INDEX_LOGIC_OP
GL_TEXTURE_COMPONENTS = GL_TEXTURE_INTERNAL_FORMAT

comment ***********************************************************

glAccum  proto ,:GLenum ,:GLfloat
glAlphaFunc  proto ,:GLenum ,:GLclampf
glAreTexturesResident  proto ,:GLsizei ,:dword ,:dword
glArrayElement  proto ,:GLint
glBegin  proto ,:GLenum
glBindTexture  proto ,:GLenum ,:GLuint
glBitmap  proto ,:GLsizei ,:GLsizei ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat ,:dword
glBlendFunc  proto ,:GLenum ,:GLenum
glCallList  proto ,:GLuint
glCallLists  proto ,:GLsizei ,:GLenum ,:dword
glClear  proto ,:GLbitfield
glClearAccum  proto ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat
glClearColor  proto ,:GLclampf ,:GLclampf ,:GLclampf ,:GLclampf
glClearDepth  proto ,:GLclampd
glClearIndex  proto ,:GLfloat
glClearStencil  proto ,:GLint
glClipPlane  proto ,:GLenum ,:dword
glColor3b  proto ,:GLbyte ,:GLbyte ,:GLbyte
glColor3bv  proto ,:dword
glColor3d  proto ,:GLdouble ,:GLdouble ,:GLdouble
glColor3dv  proto ,:dword
glColor3f  proto ,:GLfloat ,:GLfloat ,:GLfloat
glColor3fv  proto ,:dword
glColor3i  proto ,:GLint ,:GLint ,:GLint
glColor3iv  proto ,:dword
glColor3s  proto ,:GLshort ,:GLshort ,:GLshort
glColor3sv  proto ,:dword
glColor3ub  proto ,:GLubyte ,:GLubyte ,:GLubyte
glColor3ubv  proto ,:dword
glColor3ui  proto ,:GLuint ,:GLuint ,:GLuint
glColor3uiv  proto ,:dword
glColor3us  proto ,:GLushort ,:GLushort ,:GLushort
glColor3usv  proto ,:dword
glColor4b  proto ,:GLbyte ,:GLbyte ,:GLbyte ,:GLbyte
glColor4bv  proto ,:dword
glColor4d  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glColor4dv  proto ,:dword
glColor4f  proto ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat
glColor4fv  proto ,:dword
glColor4i  proto ,:GLint ,:GLint ,:GLint ,:GLint
glColor4iv  proto ,:dword
glColor4s  proto ,:GLshort ,:GLshort ,:GLshort ,:GLshort
glColor4sv  proto ,:dword
glColor4ub  proto ,:GLubyte ,:GLubyte ,:GLubyte ,:GLubyte
glColor4ubv  proto ,:dword
glColor4ui  proto ,:GLuint ,:GLuint ,:GLuint ,:GLuint
glColor4uiv  proto ,:dword
glColor4us  proto ,:GLushort ,:GLushort ,:GLushort ,:GLushort
glColor4usv  proto ,:dword
glColorMask  proto ,:GLboolean ,:GLboolean ,:GLboolean ,:GLboolean
glColorMaterial  proto ,:GLenum ,:GLenum
glColorPointer  proto ,:GLint ,:GLenum ,:GLsizei ,:dword
glCopyPixels  proto ,:GLint ,:GLint ,:GLsizei ,:GLsizei ,:GLenum
glCopyTexImage1D  proto ,:GLenum ,:GLint ,:GLenum ,:GLint ,:GLint ,:GLsizei ,:GLint
glCopyTexImage2D  proto ,:GLenum ,:GLint ,:GLenum ,:GLint ,:GLint ,:GLsizei ,:GLsizei ,:GLint
glCopyTexSubImage1D  proto ,:GLenum ,:GLint ,:GLint ,:GLint ,:GLint ,:GLsizei
glCopyTexSubImage2D  proto ,:GLenum ,:GLint ,:GLint ,:GLint ,:GLint ,:GLint ,:GLsizei ,:GLsizei
glCullFace  proto ,:GLenum
glDeleteLists  proto ,:GLuint ,:GLsizei
glDeleteTextures  proto ,:GLsizei ,:dword
glDepthFunc  proto ,:GLenum
glDepthMask  proto ,:GLboolean
glDepthRange  proto ,:GLclampd ,:GLclampd
glDisable  proto ,:GLenum
glDisableClientState  proto ,:GLenum
glDrawArrays  proto ,:GLenum ,:GLint ,:GLsizei
glDrawBuffer  proto ,:GLenum
glDrawElements  proto ,:GLenum ,:GLsizei ,:GLenum ,:dword
glDrawPixels  proto ,:GLsizei ,:GLsizei ,:GLenum ,:GLenum ,:dword
glEdgeFlag  proto ,:GLboolean
glEdgeFlagPointer  proto ,:GLsizei ,:dword
glEdgeFlagv  proto ,:dword
glEnable  proto ,:GLenum
glEnableClientState  proto ,:GLenum
glEnd  proto
glEndList  proto
glEvalCoord1d  proto ,:GLdouble
glEvalCoord1dv  proto ,:dword
glEvalCoord1f  proto ,:GLfloat
glEvalCoord1fv  proto ,:dword
glEvalCoord2d  proto ,:GLdouble ,:GLdouble
glEvalCoord2dv  proto ,:dword
glEvalCoord2f  proto ,:GLfloat ,:GLfloat
glEvalCoord2fv  proto ,:dword
glEvalMesh1  proto ,:GLenum ,:GLint ,:GLint
glEvalMesh2  proto ,:GLenum ,:GLint ,:GLint ,:GLint ,:GLint
glEvalPoint1  proto ,:GLint
glEvalPoint2  proto ,:GLint ,:GLint
glFeedbackBuffer  proto ,:GLsizei ,:GLenum ,:dword
glFinish  proto
glFlush  proto
glFogf  proto ,:GLenum ,:GLfloat
glFogfv  proto ,:GLenum ,:dword
glFogi  proto ,:GLenum ,:GLint
glFogiv  proto ,:GLenum ,:dword
glFrontFace  proto ,:GLenum
glFrustum  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glGenLists  proto ,:GLsizei
glGenTextures  proto ,:GLsizei ,:dword
glGetBooleanv  proto ,:GLenum ,:dword
glGetClipPlane  proto ,:GLenum ,:dword
glGetDoublev  proto ,:GLenum ,:dword
glGetError  proto
glGetFloatv  proto ,:GLenum ,:dword
glGetIntegerv  proto ,:GLenum ,:dword
glGetLightfv  proto ,:GLenum ,:GLenum ,:dword
glGetLightiv  proto ,:GLenum ,:GLenum ,:dword
glGetMapdv  proto ,:GLenum ,:GLenum ,:dword
glGetMapfv  proto ,:GLenum ,:GLenum ,:dword
glGetMapiv  proto ,:GLenum ,:GLenum ,:dword
glGetMaterialfv  proto ,:GLenum ,:GLenum ,:dword
glGetMaterialiv  proto ,:GLenum ,:GLenum ,:dword
glGetPixelMapfv  proto ,:GLenum ,:dword
glGetPixelMapuiv  proto ,:GLenum ,:dword
glGetPixelMapusv  proto ,:GLenum ,:dword
glGetPointerv  proto ,:GLenum ,:dword
glGetPolygonStipple  proto ,:dword
  glGetString  proto ,:GLenum
glGetTexEnvfv  proto ,:GLenum ,:GLenum ,:dword
glGetTexEnviv  proto ,:GLenum ,:GLenum ,:dword
glGetTexGendv  proto ,:GLenum ,:GLenum ,:dword
glGetTexGenfv  proto ,:GLenum ,:GLenum ,:dword
glGetTexGeniv  proto ,:GLenum ,:GLenum ,:dword
glGetTexImage  proto ,:GLenum ,:GLint ,:GLenum ,:GLenum ,:dword
glGetTexLevelParameterfv  proto ,:GLenum ,:GLint ,:GLenum ,:dword
glGetTexLevelParameteriv  proto ,:GLenum ,:GLint ,:GLenum ,:dword
glGetTexParameterfv  proto ,:GLenum ,:GLenum ,:dword
glGetTexParameteriv  proto ,:GLenum ,:GLenum ,:dword
glHint  proto ,:GLenum ,:GLenum
glIndexMask  proto ,:GLuint
glIndexPointer  proto ,:GLenum ,:GLsizei ,:dword
glIndexd  proto ,:GLdouble
glIndexdv  proto ,:dword
glIndexf  proto ,:GLfloat
glIndexfv  proto ,:dword
glIndexi  proto ,:GLint
glIndexiv  proto ,:dword
glIndexs  proto ,:GLshort
glIndexsv  proto ,:dword
glIndexub  proto ,:GLubyte
glIndexubv  proto ,:dword
glInitNames  proto
glInterleavedArrays  proto ,:GLenum ,:GLsizei ,:dword
glIsEnabled  proto ,:GLenum
glIsList  proto ,:GLuint
glIsTexture  proto ,:GLuint
glLightModelf  proto ,:GLenum ,:GLfloat
glLightModelfv  proto ,:GLenum ,:dword
glLightModeli  proto ,:GLenum ,:GLint
glLightModeliv  proto ,:GLenum ,:dword
glLightf  proto ,:GLenum ,:GLenum ,:GLfloat
glLightfv  proto ,:GLenum ,:GLenum ,:dword
glLighti  proto ,:GLenum ,:GLenum ,:GLint
glLightiv  proto ,:GLenum ,:GLenum ,:dword
glLineStipple  proto ,:GLint ,:GLushort
glLineWidth  proto ,:GLfloat
glListBase  proto ,:GLuint
glLoadIdentity  proto
glLoadMatrixd  proto ,:dword
glLoadMatrixf  proto ,:dword
glLoadName  proto ,:GLuint
glLogicOp  proto ,:GLenum
glMap1d  proto ,:GLenum ,:GLdouble ,:GLdouble ,:GLint ,:GLint ,:dword
glMap1f  proto ,:GLenum ,:GLfloat ,:GLfloat ,:GLint ,:GLint ,:dword
glMap2d  proto ,:GLenum ,:GLdouble ,:GLdouble ,:GLint ,:GLint ,:GLdouble ,:GLdouble ,:GLint ,:GLint ,:dword
glMap2f  proto ,:GLenum ,:GLfloat ,:GLfloat ,:GLint ,:GLint ,:GLfloat ,:GLfloat ,:GLint ,:GLint ,:dword
glMapGrid1d  proto ,:GLint ,:GLdouble ,:GLdouble
glMapGrid1f  proto ,:GLint ,:GLfloat ,:GLfloat
glMapGrid2d  proto ,:GLint ,:GLdouble ,:GLdouble ,:GLint ,:GLdouble ,:GLdouble
glMapGrid2f  proto ,:GLint ,:GLfloat ,:GLfloat ,:GLint ,:GLfloat ,:GLfloat
glMaterialf  proto ,:GLenum ,:GLenum ,:GLfloat
glMaterialfv  proto ,:GLenum ,:GLenum ,:dword
glMateriali  proto ,:GLenum ,:GLenum ,:GLint
glMaterialiv  proto ,:GLenum ,:GLenum ,:dword
glMatrixMode  proto ,:GLenum
glMultMatrixd  proto ,:dword
glMultMatrixf  proto ,:dword
glNewList  proto ,:GLuint ,:GLenum
glNormal3b  proto ,:GLbyte ,:GLbyte ,:GLbyte
glNormal3bv  proto ,:dword
glNormal3d  proto ,:GLdouble ,:GLdouble ,:GLdouble
glNormal3dv  proto ,:dword
glNormal3f  proto ,:GLfloat ,:GLfloat ,:GLfloat
glNormal3fv  proto ,:dword
glNormal3i  proto ,:GLint ,:GLint ,:GLint
glNormal3iv  proto ,:dword
glNormal3s  proto ,:GLshort ,:GLshort ,:GLshort
glNormal3sv  proto ,:dword
glNormalPointer  proto ,:GLenum ,:GLsizei ,:dword
glOrtho  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glPassThrough  proto ,:GLfloat
glPixelMapfv  proto ,:GLenum ,:GLsizei ,:dword
glPixelMapuiv  proto ,:GLenum ,:GLsizei ,:dword
glPixelMapusv  proto ,:GLenum ,:GLsizei ,:dword
glPixelStoref  proto ,:GLenum ,:GLfloat
glPixelStorei  proto ,:GLenum ,:GLint
glPixelTransferf  proto ,:GLenum ,:GLfloat
glPixelTransferi  proto ,:GLenum ,:GLint
glPixelZoom  proto ,:GLfloat ,:GLfloat
glPointSize  proto ,:GLfloat
glPolygonMode  proto ,:GLenum ,:GLenum
glPolygonOffset  proto ,:GLfloat ,:GLfloat
glPolygonStipple  proto ,:dword
glPopAttrib  proto
glPopClientAttrib  proto
glPopMatrix  proto
glPopName  proto
glPrioritizeTextures  proto ,:GLsizei ,:dword ,:dword
glPushAttrib  proto ,:GLbitfield
glPushClientAttrib  proto ,:GLbitfield
glPushMatrix  proto
glPushName  proto ,:GLuint
glRasterPos2d  proto ,:GLdouble ,:GLdouble
glRasterPos2dv  proto ,:dword
glRasterPos2f  proto ,:GLfloat ,:GLfloat
glRasterPos2fv  proto ,:dword
glRasterPos2i  proto ,:GLint ,:GLint
glRasterPos2iv  proto ,:dword
glRasterPos2s  proto ,:GLshort ,:GLshort
glRasterPos2sv  proto ,:dword
glRasterPos3d  proto ,:GLdouble ,:GLdouble ,:GLdouble
glRasterPos3dv  proto ,:dword
glRasterPos3f  proto ,:GLfloat ,:GLfloat ,:GLfloat
glRasterPos3fv  proto ,:dword
glRasterPos3i  proto ,:GLint ,:GLint ,:GLint
glRasterPos3iv  proto ,:dword
glRasterPos3s  proto ,:GLshort ,:GLshort ,:GLshort
glRasterPos3sv  proto ,:dword
glRasterPos4d  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glRasterPos4dv  proto ,:dword
glRasterPos4f  proto ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat
glRasterPos4fv  proto ,:dword
glRasterPos4i  proto ,:GLint ,:GLint ,:GLint ,:GLint
glRasterPos4iv  proto ,:dword
glRasterPos4s  proto ,:GLshort ,:GLshort ,:GLshort ,:GLshort
glRasterPos4sv  proto ,:dword
glReadBuffer  proto ,:GLenum
glReadPixels  proto ,:GLint ,:GLint ,:GLsizei ,:GLsizei ,:GLenum ,:GLenum ,:dword
glRectd  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glRectdv  proto ,:dword ,:dword
glRectf  proto ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat
glRectfv  proto ,:dword ,:dword
glRecti  proto ,:GLint ,:GLint ,:GLint ,:GLint
glRectiv  proto ,:dword ,:dword
glRects  proto ,:GLshort ,:GLshort ,:GLshort ,:GLshort
glRectsv  proto ,:dword ,:dword
glRenderMode  proto ,:GLenum
glRotated  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glRotatef  proto ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat
glScaled  proto ,:GLdouble ,:GLdouble ,:GLdouble
glScalef  proto ,:GLfloat ,:GLfloat ,:GLfloat
glScissor  proto ,:GLint ,:GLint ,:GLsizei ,:GLsizei
glSelectBuffer  proto ,:GLsizei ,:dword
glShadeModel  proto ,:GLenum
glStencilFunc  proto ,:GLenum ,:GLint ,:GLuint
glStencilMask  proto ,:GLuint
glStencilOp  proto ,:GLenum ,:GLenum ,:GLenum
glTexCoord1d  proto ,:GLdouble
glTexCoord1dv  proto ,:dword
glTexCoord1f  proto ,:GLfloat
glTexCoord1fv  proto ,:dword
glTexCoord1i  proto ,:GLint
glTexCoord1iv  proto ,:dword
glTexCoord1s  proto ,:GLshort
glTexCoord1sv  proto ,:dword
glTexCoord2d  proto ,:GLdouble ,:GLdouble
glTexCoord2dv  proto ,:dword
glTexCoord2f  proto ,:GLfloat ,:GLfloat
glTexCoord2fv  proto ,:dword
glTexCoord2i  proto ,:GLint ,:GLint
glTexCoord2iv  proto ,:dword
glTexCoord2s  proto ,:GLshort ,:GLshort
glTexCoord2sv  proto ,:dword
glTexCoord3d  proto ,:GLdouble ,:GLdouble ,:GLdouble
glTexCoord3dv  proto ,:dword
glTexCoord3f  proto ,:GLfloat ,:GLfloat ,:GLfloat
glTexCoord3fv  proto ,:dword
glTexCoord3i  proto ,:GLint ,:GLint ,:GLint
glTexCoord3iv  proto ,:dword
glTexCoord3s  proto ,:GLshort ,:GLshort ,:GLshort
glTexCoord3sv  proto ,:dword
glTexCoord4d  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glTexCoord4dv  proto ,:dword
glTexCoord4f  proto ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat
glTexCoord4fv  proto ,:dword
glTexCoord4i  proto ,:GLint ,:GLint ,:GLint ,:GLint
glTexCoord4iv  proto ,:dword
glTexCoord4s  proto ,:GLshort ,:GLshort ,:GLshort ,:GLshort
glTexCoord4sv  proto ,:dword
glTexCoordPointer  proto ,:GLint ,:GLenum ,:GLsizei ,:dword
glTexEnvf  proto ,:GLenum ,:GLenum ,:GLfloat
glTexEnvfv  proto ,:GLenum ,:GLenum ,:dword
glTexEnvi  proto ,:GLenum ,:GLenum ,:GLint
glTexEnviv  proto ,:GLenum ,:GLenum ,:dword
glTexGend  proto ,:GLenum ,:GLenum ,:GLdouble
glTexGendv  proto ,:GLenum ,:GLenum ,:dword
glTexGenf  proto ,:GLenum ,:GLenum ,:GLfloat
glTexGenfv  proto ,:GLenum ,:GLenum ,:dword
glTexGeni  proto ,:GLenum ,:GLenum ,:GLint
glTexGeniv  proto ,:GLenum ,:GLenum ,:dword
glTexImage1D  proto ,:GLenum ,:GLint ,:GLint ,:GLsizei ,:GLint ,:GLenum ,:GLenum ,:dword
glTexImage2D  proto ,:GLenum ,:GLint ,:GLint ,:GLsizei ,:GLsizei ,:GLint ,:GLenum ,:GLenum ,:dword
glTexParameterf  proto ,:GLenum ,:GLenum ,:GLfloat
glTexParameterfv  proto ,:GLenum ,:GLenum ,:dword
glTexParameteri  proto ,:GLenum ,:GLenum ,:GLint
glTexParameteriv  proto ,:GLenum ,:GLenum ,:dword
glTexSubImage1D  proto ,:GLenum ,:GLint ,:GLint ,:GLsizei ,:GLenum ,:GLenum ,:dword
glTexSubImage2D  proto ,:GLenum ,:GLint ,:GLint ,:GLint ,:GLsizei ,:GLsizei ,:GLenum ,:GLenum ,:dword
glTranslated  proto ,:GLdouble ,:GLdouble ,:GLdouble
glTranslatef  proto ,:GLfloat ,:GLfloat ,:GLfloat
glVertex2d  proto ,:GLdouble ,:GLdouble
glVertex2dv  proto ,:dword
glVertex2f  proto ,:GLfloat ,:GLfloat
glVertex2fv  proto ,:dword
glVertex2i  proto ,:GLint ,:GLint
glVertex2iv  proto ,:dword
glVertex2s  proto ,:GLshort ,:GLshort
glVertex2sv  proto ,:dword
glVertex3d  proto ,:GLdouble ,:GLdouble ,:GLdouble
glVertex3dv  proto ,:dword
glVertex3f  proto ,:GLfloat ,:GLfloat ,:GLfloat
glVertex3fv  proto ,:dword
glVertex3i  proto ,:GLint ,:GLint ,:GLint
glVertex3iv  proto ,:dword
glVertex3s  proto ,:GLshort ,:GLshort ,:GLshort
glVertex3sv  proto ,:dword
glVertex4d  proto ,:GLdouble ,:GLdouble ,:GLdouble ,:GLdouble
glVertex4dv  proto ,:dword
glVertex4f  proto ,:GLfloat ,:GLfloat ,:GLfloat ,:GLfloat
glVertex4fv  proto ,:dword
glVertex4i  proto ,:GLint ,:GLint ,:GLint ,:GLint
glVertex4iv  proto ,:dword
glVertex4s  proto ,:GLshort ,:GLshort ,:GLshort ,:GLshort
glVertex4sv  proto ,:dword
glVertexPointer  proto ,:GLint ,:GLenum ,:GLsizei ,:dword
glViewport  proto ,:GLint ,:GLint ,:GLsizei ,:GLsizei

comment  EXT_vertex_array 

comment  WIN_draw_range_elements 

comment  WIN_swap_hint 

endif
endif

