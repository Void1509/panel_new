#ifndef _GLOBAL_H
#define _GLOBAL_H

#if defined WIN32
  #define CMP_DECL_IMPORT __declspec(dllimport)
  #define CMP_DECL_EXPORT __declspec(dllexport)
  #define CMP_DECL_LOCAL
#else
  #if __GNUC__ >= 4
    #define CMP_DECL_IMPORT __attribute__ ((visibility("default")))
    #define CMP_DECL_EXPORT __attribute__ ((visibility("default")))
    #define CMP_DECL_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define CMP_DECL_IMPORT
    #define CMP_DECL_EXPORT
    #define CMP_DECL_LOCAL
  #endif
#endif

#ifndef CMP_STD_CALL
#  if defined (WIN32)
#     define CMP_STD_CALL __cdecl
#  elif defined(QNX6)
#     define CMP_STD_CALL
#  else
#     define CMP_STD_CALL
#  endif
#endif

#if defined (WIN32) && defined (_MSC_VER)
#define MSVC_DLLSUFF "_msvc"
#else
#define MSVC_DLLSUFF
#endif

//#include "os.h"
#include "agro_types.h"


#endif // _GLOBAL_H
