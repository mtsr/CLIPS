   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  07/25/14            */
   /*                                                     */
   /*            DEFTEMPLATE PARSER HEADER FILE           */
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

#ifndef _H_tmpltpsr

#define _H_tmpltpsr

#ifndef _H_symbol
#include "symbol.h"
#endif
#ifndef _H_tmpltdef
#include "tmpltdef.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _TMPLTPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE int                            ParseDeftemplate(void *,const char *);
   LOCALE void                           InstallDeftemplate(void *,struct deftemplate *);

#endif



