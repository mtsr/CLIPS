   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/06/16             */
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
   bool                           DirectMVReplaceCommand(void *);
   bool                           DirectMVInsertCommand(void *);
   bool                           DirectMVDeleteCommand(void *);

#endif /* _H_insmult */



