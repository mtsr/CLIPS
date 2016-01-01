   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.30  08/16/14          */
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
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*************************************************************/

#ifndef _H_insmult
#define _H_insmult

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#if (! RUN_TIME)
   void                           SetupInstanceMultifieldCommands(void *);
#endif

   void                           MVSlotReplaceCommand(void *,DATA_OBJECT *);
   void                           MVSlotInsertCommand(void *,DATA_OBJECT *);
   void                           MVSlotDeleteCommand(void *,DATA_OBJECT *);
   intBool                        DirectMVReplaceCommand(void *);
   intBool                        DirectMVInsertCommand(void *);
   intBool                        DirectMVDeleteCommand(void *);

#endif /* _H_insmult */



