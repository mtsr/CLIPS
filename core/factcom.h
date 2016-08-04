   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  07/30/16            */
   /*                                                     */
   /*               FACT COMMANDS HEADER FILE             */
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
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*      6.24: Added environment parameter to GenClose.       */
/*            Added environment parameter to GenOpen.        */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW and       */
/*            MAC_MCW).                                      */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Added code to prevent a clear command from     */
/*            being executed during fact assertions via      */
/*            Increment/DecrementClearReadyLocks API.        */
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
/*************************************************************/

#ifndef _H_factcom

#pragma once

#define _H_factcom

#include "evaluatn.h"

   void                           FactCommandDefinitions(Environment *);
   void                           AssertCommand(UDFContext *,CLIPSValue *);
   void                           RetractCommand(UDFContext *,CLIPSValue *);
   void                           AssertStringFunction(UDFContext *,CLIPSValue *);
   void                           FactsCommand(UDFContext *,CLIPSValue *);
   void                           EnvFacts(Environment *,const char *,Defmodule *,long long,long long,long long);
   void                           SetFactDuplicationCommand(UDFContext *,CLIPSValue *);
   void                           GetFactDuplicationCommand(UDFContext *,CLIPSValue *);
   void                           SaveFactsCommand(UDFContext *,CLIPSValue *);
   void                           LoadFactsCommand(UDFContext *,CLIPSValue *);
   bool                           EnvSaveFacts(Environment *,const char *,int);
   bool                           EnvSaveFactsDriver(Environment *,const char *,int,struct expr *);
   bool                           EnvLoadFacts(Environment *,const char *);
   bool                           EnvLoadFactsFromString(Environment *,const char *,long);
   void                           FactIndexFunction(UDFContext *,CLIPSValue *);

#endif /* _H_factcom */


