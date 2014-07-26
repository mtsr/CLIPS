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
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_dffnxpsr
#define _H_dffnxpsr

#if DEFFUNCTION_CONSTRUCT && (! BLOAD_ONLY) && (! RUN_TIME)

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _DFFNXPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

LOCALE intBool ParseDeffunction(void *,const char *);

#ifndef _DFFNXPSR_SOURCE_
#endif

#endif

#endif




