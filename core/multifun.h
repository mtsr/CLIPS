   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  07/30/16            */
   /*                                                     */
   /*           MULTIFIELD FUNCTIONS HEADER FILE          */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary Riley and Brian Dantes                          */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*            Changed name of variable exp to theExp         */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Moved ImplodeMultifield to multifld.c.         */
/*                                                           */
/*      6.30: Changed integer type/precision.                */
/*                                                           */
/*            Support for long long integers.                */
/*                                                           */
/*            Changed garbage collection algorithm.          */
/*                                                           */
/*            Fixed memory leaks when error occurred.        */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Fixed linkage issue when DEFMODULE_CONSTRUCT   */
/*            compiler flag is set to 0.                     */
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

#ifndef _H_multifun

#pragma once

#define _H_multifun

#include "evaluatn.h"

   void                    MultifieldFunctionDefinitions(Environment *);
#if MULTIFIELD_FUNCTIONS
   void                    DeleteFunction(UDFContext *,CLIPSValue *);
   void                    MVDeleteFunction(Environment *,DATA_OBJECT_PTR);
   void                    ReplaceFunction(UDFContext *,CLIPSValue *);
   void                    MVReplaceFunction(Environment *,DATA_OBJECT_PTR);
   void                    DeleteMemberFunction(UDFContext *,CLIPSValue *);
   void                    ReplaceMemberFunction(UDFContext *,CLIPSValue *);
   void                    InsertFunction(UDFContext *,CLIPSValue *);
   void                    ExplodeFunction(UDFContext *,CLIPSValue *);
   void                    ImplodeFunction(UDFContext *,CLIPSValue *);
   void                    SubseqFunction(UDFContext *,CLIPSValue *);
   void                    MVSubseqFunction(Environment *,DATA_OBJECT_PTR);
   void                    FirstFunction(UDFContext *,CLIPSValue *);
   void                    RestFunction(UDFContext *,CLIPSValue *);
   void                    NthFunction(UDFContext *,CLIPSValue *);
   void                    SubsetpFunction(UDFContext *,CLIPSValue *);
   void                    MemberFunction(UDFContext *,CLIPSValue *);
   void                    MultifieldPrognFunction(UDFContext *,CLIPSValue *);
   void                    ForeachFunction(UDFContext *,CLIPSValue *);
   void                    GetMvPrognField(UDFContext *,CLIPSValue *);
   void                    GetMvPrognIndex(UDFContext *,CLIPSValue *);
   bool                    FindDOsInSegment(DATA_OBJECT_PTR,int,DATA_OBJECT_PTR,
                                            long *,long *,long *,int);
#endif
   bool                    ReplaceMultiValueField(Environment *,struct dataObject *,
                                                  struct dataObject *,
                                                  long,long,
                                                  struct dataObject *,const char *);
   bool                    InsertMultiValueField(Environment *,struct dataObject *,
                                                 struct dataObject *,
                                                 long,struct dataObject *,const char *);
   bool                    DeleteMultiValueField(Environment *,struct dataObject *,struct dataObject *,
                                                 long,long,const char *);

#endif /* _H_multifun */

