/*
 *
 * Copyright (c) 2002-2008, NVIDIA Corporation.
 * 
 *  
 * 
 * NVIDIA Corporation("NVIDIA") supplies this software to you in consideration 
 * of your agreement to the following terms, and your use, installation, 
 * modification or redistribution of this NVIDIA software constitutes 
 * acceptance of these terms.  If you do not agree with these terms, please do 
 * not use, install, modify or redistribute this NVIDIA software.
 * 
 *  
 * 
 * In consideration of your agreement to abide by the following terms, and 
 * subject to these terms, NVIDIA grants you a personal, non-exclusive license,
 * under NVIDIA's copyrights in this original NVIDIA software (the "NVIDIA 
 * Software"), to use, reproduce, modify and redistribute the NVIDIA 
 * Software, with or without modifications, in source and/or binary forms; 
 * provided that if you redistribute the NVIDIA Software, you must retain the 
 * copyright notice of NVIDIA, this notice and the following text and 
 * disclaimers in all such redistributions of the NVIDIA Software. Neither the 
 * name, trademarks, service marks nor logos of NVIDIA Corporation may be used 
 * to endorse or promote products derived from the NVIDIA Software without 
 * specific prior written permission from NVIDIA.  Except as expressly stated 
 * in this notice, no other rights or licenses express or implied, are granted 
 * by NVIDIA herein, including but not limited to any patent rights that may be 
 * infringed by your derivative works or by other works in which the NVIDIA 
 * Software may be incorporated. No hardware is licensed hereunder. 
 * 
 *  
 * 
 * THE NVIDIA SOFTWARE IS BEING PROVIDED ON AN "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING 
 * WITHOUT LIMITATION, WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR ITS USE AND OPERATION 
 * EITHER ALONE OR IN COMBINATION WITH OTHER PRODUCTS.
 * 
 *  
 * 
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, 
 * EXEMPLARY, CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, LOST 
 * PROFITS; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) OR ARISING IN ANY WAY OUT OF THE USE, 
 * REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE NVIDIA SOFTWARE, 
 * HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING 
 * NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF NVIDIA HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */ 

#ifndef _cg_h
#define _cg_h

/*************************************************************************/
/*** CG Run-Time Library API                                          ***/
/*************************************************************************/

#define CG_VERSION_NUM                2100

#ifdef _WIN32
# ifndef APIENTRY /* From Win32's <windef.h> */
#  define CG_APIENTRY_DEFINED
#  if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__)
#   define APIENTRY    __stdcall
#  else
#   define APIENTRY
#  endif
# endif
# ifndef WINGDIAPI /* From Win32's <wingdi.h> and <winnt.h> */
#  define CG_WINGDIAPI_DEFINED
#  define WINGDIAPI __declspec(dllimport)
# endif
#endif /* _WIN32 */

/* Set up CG_API for Win32 dllexport or gcc visibility */

#ifndef CG_API
# ifdef CG_EXPORTS
#  ifdef _WIN32
#   define CG_API __declspec(dllexport)
#  elif defined(__GNUC__) && __GNUC__>=4
#   define CG_API __attribute__ ((visibility("default")))
#  elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#   define CG_API __global
#  else
#   define CG_API
#  endif
# else
#  define CG_API
# endif
#endif

#ifndef CGENTRY
# ifdef _WIN32
#  define CGENTRY __cdecl
# else
#  define CGENTRY
# endif
#endif

/*************************************************************************/
/*** Data types and enumerants                                         ***/
/*************************************************************************/

typedef int CGbool;

#define CG_FALSE ((CGbool)0)
#define CG_TRUE ((CGbool)1)

typedef struct _CGcontext *CGcontext;
typedef struct _CGprogram *CGprogram;
typedef struct _CGparameter *CGparameter;
typedef struct _CGobj *CGobj;
typedef struct _CGbuffer *CGbuffer;
typedef struct _CGeffect *CGeffect;
typedef struct _CGtechnique *CGtechnique;
typedef struct _CGpass *CGpass;
typedef struct _CGstate *CGstate;
typedef struct _CGstateassignment *CGstateassignment;
typedef struct _CGannotation *CGannotation;
typedef void *CGhandle;


//!!! PREPROCESS BEGIN

typedef enum
 {
  CG_UNKNOWN_TYPE,
  CG_STRUCT,  
  CG_ARRAY,
  CG_TYPELESS_STRUCT, 

  CG_TYPE_START_ENUM = 1024,
# define CG_DATATYPE_MACRO(name, compiler_name, enum_name, base_name, ncols, nrows, pc) \
  enum_name ,

#include <Cg/cg_datatypes.h>

# undef CG_DATATYPE_MACRO

  

 } CGtype;

typedef enum
 {
# define CG_BINDLOCATION_MACRO(name,enum_name,compiler_name,\
                               enum_int,addressable,param_type) \
  enum_name = enum_int,

#include <Cg/cg_bindlocations.h>

  CG_UNDEFINED = 3256,

 } CGresource;

typedef enum
 {
  CG_PROFILE_START = 6144,
  CG_PROFILE_UNKNOWN,

# define CG_PROFILE_MACRO(name, compiler_id, compiler_id_caps, compiler_opt,int_id,vertex_profile) \
   CG_PROFILE_##compiler_id_caps = int_id,
  
#include <Cg/cg_profiles.h>

  CG_PROFILE_MAX = 7100,
 } CGprofile;

typedef enum
 {
# define CG_ERROR_MACRO(code, enum_name, message) \
   enum_name = code,
# include <Cg/cg_errors.h>
 } CGerror;

typedef enum
 {
# define CG_ENUM_MACRO(enum_name, enum_val) \
   enum_name = enum_val,
# include <Cg/cg_enums.h>
 } CGenum;

//!!! PREPROCESS END

typedef enum
 {
  CG_PARAMETERCLASS_UNKNOWN = 0,
  CG_PARAMETERCLASS_SCALAR,
  CG_PARAMETERCLASS_VECTOR,
  CG_PARAMETERCLASS_MATRIX,
  CG_PARAMETERCLASS_STRUCT,
  CG_PARAMETERCLASS_ARRAY,
  CG_PARAMETERCLASS_SAMPLER,
  CG_PARAMETERCLASS_OBJECT
 } CGparameterclass;

typedef enum
{
    CG_UNKNOWN_DOMAIN = 0,
    CG_FIRST_DOMAIN   = 1,
    CG_VERTEX_DOMAIN  = 1,
    CG_FRAGMENT_DOMAIN,
    CG_GEOMETRY_DOMAIN,
    CG_NUMBER_OF_DOMAINS
} CGdomain;

typedef enum
{
    CG_MAP_READ = 0,
    CG_MAP_WRITE,
    CG_MAP_READ_WRITE,
    CG_MAP_WRITE_DISCARD,
    CG_MAP_WRITE_NO_OVERWRITE
} CGbufferaccess;

typedef enum
{
    CG_BUFFER_USAGE_STREAM_DRAW = 0,
    CG_BUFFER_USAGE_STREAM_READ,
    CG_BUFFER_USAGE_STREAM_COPY,
    CG_BUFFER_USAGE_STATIC_DRAW,
    CG_BUFFER_USAGE_STATIC_READ,
    CG_BUFFER_USAGE_STATIC_COPY,
    CG_BUFFER_USAGE_DYNAMIC_DRAW,
    CG_BUFFER_USAGE_DYNAMIC_READ,
    CG_BUFFER_USAGE_DYNAMIC_COPY
} CGbufferusage;

#ifdef __cplusplus
extern "C" {
#endif
CGcontext (*cgCreateContext)(void);
CGprogram (*cgCreateProgram)(CGcontext,CGenum,const char *,CGprofile ,const char *,const char **);
void (*cgDestroyProgram)(CGprogram);
CGerror (*cgGetError)(void);
const char * (*cgGetErrorString)(CGerror);
CGparameter (*cgGetNamedParameter)(CGprogram,const char *);
#ifdef __cplusplus
}
#endif

#ifdef CG_APIENTRY_DEFINED
# undef CG_APIENTRY_DEFINED
# undef APIENTRY
#endif

#ifdef CG_WINGDIAPI_DEFINED
# undef CG_WINGDIAPI_DEFINED
# undef WINGDIAPI
#endif

int LoadCGLib()
{
 static int loaded=0;
 if (loaded) return 1;
 
 HINSTANCE dlib=LoadLibrary("cg.dll");
 if (dlib==0) return 0;
 cgCreateContext=(CGcontext(*)(void))GetProcAddress(dlib,"cgCreateContext");
 cgCreateProgram=
 (CGprogram (*)(CGcontext,CGenum,const char *,CGprofile ,const char *,const char **))GetProcAddress(dlib,"cgCreateProgram");
 cgDestroyProgram=(void (*)(CGprogram))GetProcAddress(dlib,"cgDestroyProgram");;
 cgGetError=(CGerror (*)(void))GetProcAddress(dlib,"cgGetError");
 cgGetErrorString=(const char * (*)(CGerror))GetProcAddress(dlib,"cgGetErrorString");
 cgGetNamedParameter=(CGparameter (*)(CGprogram,const char *))GetProcAddress(dlib,"cgGetNamedParameter");
 
 loaded=1;
 return 1;
}

#endif
