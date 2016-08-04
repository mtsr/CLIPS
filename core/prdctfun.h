   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  07/30/16            */
   /*                                                     */
   /*            PREDICATE FUNCTIONS HEADER FILE          */
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
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW and       */
/*            MAC_MCW).                                      */
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

#ifndef _H_prdctfun

#pragma once

#define _H_prdctfun

   void                           PredicateFunctionDefinitions(Environment *);
   void                           EqFunction(UDFContext *,CLIPSValue *);
   void                           NeqFunction(UDFContext *,CLIPSValue *);
   void                           StringpFunction(UDFContext *,CLIPSValue *);
   void                           SymbolpFunction(UDFContext *,CLIPSValue *);
   void                           LexemepFunction(UDFContext *,CLIPSValue *);
   void                           NumberpFunction(UDFContext *,CLIPSValue *);
   void                           FloatpFunction(UDFContext *,CLIPSValue *);
   void                           IntegerpFunction(UDFContext *,CLIPSValue *);
   void                           MultifieldpFunction(UDFContext *,CLIPSValue *);
   void                           PointerpFunction(UDFContext *,CLIPSValue *);
   void                           NotFunction(UDFContext *,CLIPSValue *);
   void                           AndFunction(UDFContext *,CLIPSValue *);
   void                           OrFunction(UDFContext *,CLIPSValue *);
   void                           LessThanOrEqualFunction(UDFContext *,CLIPSValue *);
   void                           GreaterThanOrEqualFunction(UDFContext *,CLIPSValue *);
   void                           LessThanFunction(UDFContext *,CLIPSValue *);
   void                           GreaterThanFunction(UDFContext *,CLIPSValue *);
   void                           NumericEqualFunction(UDFContext *,CLIPSValue *);
   void                           NumericNotEqualFunction(UDFContext *,CLIPSValue *);
   void                           OddpFunction(UDFContext *,CLIPSValue *);
   void                           EvenpFunction(UDFContext *,CLIPSValue *);

#endif /* _H_prdctfun */



