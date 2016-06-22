   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  06/20/16            */
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
/*      6.40: Removed LOCALE definition.                     */
/*                                                           */
/*************************************************************/

#ifndef _H_cstrnutl

#pragma once

#define _H_cstrnutl

#include "constrnt.h"

   struct constraintRecord       *GetConstraintRecord(void *);
   int                            CompareNumbers(void *,int,void *,int,void *);
   struct constraintRecord       *CopyConstraintRecord(void *,CONSTRAINT_RECORD *);
   bool                           SetConstraintType(int,CONSTRAINT_RECORD *);
   void                           SetAnyAllowedFlags(CONSTRAINT_RECORD *,bool);
   void                           SetAnyRestrictionFlags(CONSTRAINT_RECORD *,bool);
   CONSTRAINT_RECORD             *ArgumentTypeToConstraintRecord(void *,int);
   CONSTRAINT_RECORD             *FunctionCallToConstraintRecord(void *,void *);
   CONSTRAINT_RECORD             *ExpressionToConstraintRecord(void *,struct expr *);
   CONSTRAINT_RECORD             *ArgumentTypeToConstraintRecord2(void *,unsigned);

#endif /* _H_cstrnutl */


