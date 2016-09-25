   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  08/25/16            */
   /*                                                     */
   /*              FACT FUNCTIONS HEADER FILE             */
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
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*            Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*      6.24: Added ppfact function.                         */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
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
/*            UDF redesign.                                  */
/*                                                           */
/*************************************************************/

#ifndef _H_factfun

#pragma once

#define _H_factfun

#include "factmngr.h"

   void                           FactFunctionDefinitions(Environment *);
   void                           FactRelationFunction(Environment *,UDFContext *,CLIPSValue *);
   CLIPSLexeme                   *FactRelation(Fact *);
   Deftemplate                   *EnvFactDeftemplate(Environment *,Fact *);
   void                           FactExistpFunction(Environment *,UDFContext *,CLIPSValue *);
   bool                           EnvFactExistp(Environment *,Fact *);
   void                           FactSlotValueFunction(Environment *,UDFContext *,CLIPSValue *);
   void                           FactSlotValue(Environment *,Fact *,const char *,CLIPSValue *);
   void                           FactSlotNamesFunction(Environment *,UDFContext *,CLIPSValue *);
   void                           EnvFactSlotNames(Environment *,Fact *,CLIPSValue *);
   void                           GetFactListFunction(Environment *,UDFContext *,CLIPSValue *);
   void                           EnvGetFactList(Environment *,CLIPSValue *,Defmodule *);
   void                           PPFactFunction(Environment *,UDFContext *,CLIPSValue *);
   void                           EnvPPFact(Environment *,Fact *,const char *,bool);
   Fact                          *GetFactAddressOrIndexArgument(UDFContext *,bool);

#endif /* _H_factfun */

