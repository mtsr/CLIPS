   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  07/25/14            */
   /*                                                     */
   /*             STRING FUNCTIONS HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_strngfun

#define _H_strngfun

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _STRNGFUN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#if ALLOW_ENVIRONMENT_GLOBALS
   LOCALE int                            Build(const char *);
   LOCALE int                            Eval(const char *,DATA_OBJECT_PTR);
#endif

   LOCALE int                            EnvBuild(void *,const char *);
   LOCALE int                            EnvEval(void *,const char *,DATA_OBJECT_PTR);
   LOCALE void                           StringFunctionDefinitions(void *);
   LOCALE void                           StrCatFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           SymCatFunction(void *,DATA_OBJECT_PTR);
   LOCALE long long                      StrLengthFunction(void *);
   LOCALE void                           UpcaseFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           LowcaseFunction(void *,DATA_OBJECT_PTR);
   LOCALE long long                      StrCompareFunction(void *);
   LOCALE void                          *SubStringFunction(void *);
   LOCALE void                           StrIndexFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           EvalFunction(void *,DATA_OBJECT_PTR);
   LOCALE int                            BuildFunction(void *);
   LOCALE void                           StringToFieldFunction(void *,DATA_OBJECT *);
   LOCALE void                           StringToField(void *,const char *,DATA_OBJECT *);

#endif






