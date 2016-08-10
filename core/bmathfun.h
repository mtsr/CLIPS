   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.50  08/06/16            */
   /*                                                     */
   /*             BASIC MATH FUNCTIONS MODULE             */
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
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Removed LOCALE definition.                     */
/*                                                           */
/*            Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*            ALLOW_ENVIRONMENT_GLOBALS no longer supported. */
/*                                                           */
/*      6.50: Auto-float-dividend always enabled.            */
/*                                                           */
/*************************************************************/

#ifndef _H_bmathfun

#pragma once

#define _H_bmathfun

#include "evaluatn.h"

   void                    BasicMathFunctionDefinitions(Environment *);
   void                    AdditionFunction(UDFContext *,CLIPSValue *);
   void                    MultiplicationFunction(UDFContext *,CLIPSValue *);
   void                    SubtractionFunction(UDFContext *,CLIPSValue *);
   void                    DivisionFunction(UDFContext *,CLIPSValue *);
   void                    DivFunction(UDFContext *,CLIPSValue *);
   void                    IntegerFunction(UDFContext *,CLIPSValue *);
   void                    FloatFunction(UDFContext *,CLIPSValue *);
   void                    AbsFunction(UDFContext *,CLIPSValue *);
   void                    MinFunction(UDFContext *,CLIPSValue *);
   void                    MaxFunction(UDFContext *,CLIPSValue *);

#endif




