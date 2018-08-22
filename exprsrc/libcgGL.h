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

#ifdef _WIN32
# ifndef APIENTRY /* From Win32's <windef.h> */
#  define CGGL_APIENTRY_DEFINED
#  if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__)
#   define APIENTRY    __stdcall
#  else
#   define APIENTRY
#  endif
# endif
# ifndef WINGDIAPI /* From Win32's <wingdi.h> and <winnt.h> */
#  define CGGL_WINGDIAPI_DEFINED
#  define WINGDIAPI __declspec(dllimport)
# endif
#endif /* _WIN32 */

/* Set up for either Win32 import/export/lib. */
#ifndef CGGL_API
# ifdef CGGL_EXPORTS
#  ifdef _WIN32
#   define CGGL_API __declspec(dllexport)
#  elif defined(__GNUC__) && __GNUC__>=4
#   define CGGL_API __attribute__ ((visibility("default")))
#  elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#   define CGGL_API __global
#  else
#   define CGGL_API
#  endif
# else
#  define CGGL_API
# endif
#endif

#ifndef CGGLENTRY
# ifdef _WIN32
#  define CGGLENTRY __cdecl
# else
#  define CGGLENTRY
# endif
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*** cgGL Type Definitions                                                 ***/
/*****************************************************************************/

typedef enum
 {
  CG_GL_MATRIX_IDENTITY = 0,
  CG_GL_MATRIX_TRANSPOSE = 1,
  CG_GL_MATRIX_INVERSE = 2,
  CG_GL_MATRIX_INVERSE_TRANSPOSE = 3,

  CG_GL_MODELVIEW_MATRIX,
  CG_GL_PROJECTION_MATRIX,
  CG_GL_TEXTURE_MATRIX,
  CG_GL_MODELVIEW_PROJECTION_MATRIX,

  CG_GL_VERTEX,
  CG_GL_FRAGMENT,
  CG_GL_GEOMETRY

 } CGGLenum;


CGprofile (*cgGLGetLatestProfile)(CGGLenum);
void (*cgGLSetOptimalOptions)(CGprofile);
void (*cgGLLoadProgram)(CGprogram);
void (*cgGLBindProgram)(CGprogram);
void (*cgGLUnbindProgram)(CGprofile);
void (*cgGLEnableProfile)(CGprofile);
void (*cgGLDisableProfile)(CGprofile);
void (*cgGLSetStateMatrixParameter)(CGparameter,CGGLenum,CGGLenum);
void (*cgGLSetTextureParameter)(CGparameter,GLuint);
void (*cgGLEnableTextureParameter)(CGparameter);
void (*cgGLDisableTextureParameter)(CGparameter);
void (*cgGLSetParameter1f)(CGparameter,float);
void (*cgGLSetParameter3fv)(CGparameter,const float *);
void (*cgGLSetParameter4fv)(CGparameter,const float *);


#ifdef __cplusplus
}
#endif

#ifdef CGGL_APIENTRY_DEFINED
# undef CGGL_APIENTRY_DEFINED
# undef APIENTRY
#endif

#ifdef CGGL_WINGDIAPI_DEFINED
# undef CGGL_WINGDIAPI_DEFINED
# undef WINGDIAPI
#endif

int LoadCGGLLib()
{
 static int loaded=0;
 if (loaded) return 1;
 
 HINSTANCE dlib=LoadLibrary("cgGL.dll");
 if (dlib==0) return 0;
 cgGLGetLatestProfile=(CGprofile (*)(CGGLenum))GetProcAddress(dlib,"cgGLGetLatestProfile");
 cgGLSetOptimalOptions=(void (*)(CGprofile))GetProcAddress(dlib,"cgGLSetOptimalOptions");
 cgGLLoadProgram=(void (*)(CGprogram))GetProcAddress(dlib,"cgGLLoadProgram");
 cgGLEnableProfile=(void (*)(CGprofile))GetProcAddress(dlib,"cgGLEnableProfile");
 cgGLBindProgram=(void (*)(CGprogram))GetProcAddress(dlib,"cgGLBindProgram");
 cgGLSetStateMatrixParameter=(void (*)(CGparameter,CGGLenum,CGGLenum))GetProcAddress(dlib,"cgGLSetStateMatrixParameter");
 cgGLSetTextureParameter=(void (*)(CGparameter,GLuint))GetProcAddress(dlib,"cgGLSetTextureParameter");
 cgGLEnableTextureParameter=(void (*)(CGparameter))GetProcAddress(dlib,"cgGLEnableTextureParameter");
 cgGLDisableTextureParameter=(void (*)(CGparameter))GetProcAddress(dlib,"cgGLDisableTextureParameter");
 cgGLSetParameter1f=(void (*)(CGparameter,float))GetProcAddress(dlib,"cgGLSetParameter1f");
 cgGLSetParameter3fv=(void (*)(CGparameter,const float *))GetProcAddress(dlib,"cgGLSetParameter3fv");
 cgGLSetParameter4fv=(void (*)(CGparameter,const float *))GetProcAddress(dlib,"cgGLSetParameter4fv");
 cgGLUnbindProgram=(void (*)(CGprofile))GetProcAddress(dlib,"cgGLUnbindProgram");
 cgGLDisableProfile=(void (*)(CGprofile))GetProcAddress(dlib,"cgGLDisableProfile");
 
 loaded=1;
 return 1;
}
