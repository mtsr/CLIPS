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
/*      Gary D. Riley                                        */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Added fact-set queries.                        */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_factqpsr
#define _H_factqpsr

#if FACT_SET_QUERIES && (! RUN_TIME)

#ifndef _H_expressn
#include "expressn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FACTQPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE EXPRESSION *FactParseQueryNoAction(void *,EXPRESSION *,const char *);
LOCALE EXPRESSION *FactParseQueryAction(void *,EXPRESSION *,const char *);

#ifndef _FACTQPSR_SOURCE_
#endif

#endif

#endif



