   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.30  08/16/14          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose: Definstances Construct Compiler Code             */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Added support for path name argument to        */
/*            constructs-to-c.                               */
/*                                                           */
/*************************************************************/

#ifndef _H_dfinscmp
#define _H_dfinscmp

#if DEFINSTANCES_CONSTRUCT && CONSTRUCT_COMPILER && (! RUN_TIME)

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

   void                           SetupDefinstancesCompiler(void *);
   void                           DefinstancesCModuleReference(void *,FILE *,int,int,int);

#endif /* DEFINSTANCES_CONSTRUCT && CONSTRUCT_COMPILER && (! RUN_TIME) */

#endif /* _H_dfinscmp */


