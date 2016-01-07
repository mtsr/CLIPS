   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/06/16             */
   /*                                                     */
   /*            CONSTRAINT UTILITY HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose: Utility routines for manipulating, initializing, */
/*   creating, copying, and comparing constraint records.    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian Dantes                                         */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_cstrnutl
#define _H_cstrnutl

#ifndef _H_constrnt
#include "constrnt.h"
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

   struct constraintRecord       *GetConstraintRecord(void *);
   int                            CompareNumbers(void *,int,void *,int,void *);
   struct constraintRecord       *CopyConstraintRecord(void *,CONSTRAINT_RECORD *);
   bool                           SetConstraintType(int,CONSTRAINT_RECORD *);
   void                           SetAnyAllowedFlags(CONSTRAINT_RECORD *,bool);
   void                           SetAnyRestrictionFlags(CONSTRAINT_RECORD *,bool);
   CONSTRAINT_RECORD             *ArgumentTypeToConstraintRecord(void *,int);
   CONSTRAINT_RECORD             *FunctionCallToConstraintRecord(void *,void *);
   CONSTRAINT_RECORD             *ExpressionToConstraintRecord(void *,struct expr *);

#endif /* _H_cstrnutl */


