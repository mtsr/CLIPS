   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.30  07/25/14          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_insqypsr
#define _H_insqypsr

#if INSTANCE_SET_QUERIES && (! RUN_TIME)

#ifndef _H_expressn
#include "expressn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _INSQYPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE EXPRESSION *ParseQueryNoAction(void *,EXPRESSION *,const char *);
LOCALE EXPRESSION *ParseQueryAction(void *,EXPRESSION *,const char *);

#ifndef _INSQYPSR_SOURCE_
#endif

#endif

#endif



