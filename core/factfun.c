   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  11/01/16             */
   /*                                                     */
   /*               FACT FUNCTIONS MODULE                 */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/*                                                           */
/* (fact-existp <fact-address-or-index>)                     */
/*    Returns TRUE if the fact exists, otherwise FALSE is    */
/*    returned.                                              */
/*                                                           */
/* (fact-relation <fact-address-or-index>)                   */
/*    Returns the deftemplate name of the fact. Returns      */
/*    FALSE if the specified fact doesn't exist.             */
/*                                                           */
/* (fact-slot-value <fact-address-or-index> <slot-name>)     */
/*    Returns the contents of a slot (use the slot name      */
/*    implied for the implied multifield slot of an ordered  */
/*    fact). Returns the value FALSE if the slot name is     */
/*    invalid or the fact doesn't exist.                     */
/*                                                           */
/* (fact-slot-names <fact-address-or-index>)                 */
/*    Returns the slot names associated with a fact in a     */
/*    multifield value. Returns FALSE if the fact doesn't    */
/*    exist.                                                 */
/*                                                           */
/* (get-fact-list [<module-name>])                           */
/*    Returns the list of facts visible to the specified     */
/*    module or to the current module if none is specified.  */
/*    If * is specified then all facts are returned.         */
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
/*            Added STDOUT and STDIN logical name            */
/*            definitions.                                   */
/*                                                           */
/*      6.40: Added Env prefix to GetEvaluationError and     */
/*            SetEvaluationError functions.                  */
/*                                                           */
/*            Added Env prefix to GetHaltExecution and       */
/*            SetHaltExecution functions.                    */
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
/*            UDF redesign.                                  */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <string.h>

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT

#include "argacces.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "multifld.h"
#include "prntutil.h"
#include "router.h"
#include "sysdep.h"
#include "tmpltutl.h"

#include "factfun.h"

/****************************************************/
/* FactFunctionDefinitions: Defines fact functions. */
/****************************************************/
void FactFunctionDefinitions(
  Environment *theEnv)
  {
#if ! RUN_TIME
   EnvAddUDF(theEnv,"fact-existp","b",1,1,"lf",FactExistpFunction,"FactExistpFunction",NULL);
   EnvAddUDF(theEnv,"fact-relation","y",1,1,"lf",FactRelationFunction,"FactRelationFunction",NULL);
   EnvAddUDF(theEnv,"fact-slot-value","*",2,2,";lf;y",FactSlotValueFunction,"FactSlotValueFunction",NULL);
   EnvAddUDF(theEnv,"fact-slot-names","*",1,1,"lf",FactSlotNamesFunction,"FactSlotNamesFunction",NULL);
   EnvAddUDF(theEnv,"get-fact-list","m",0,1,"y",GetFactListFunction,"GetFactListFunction",NULL);
   EnvAddUDF(theEnv,"ppfact","v",1,3,"*;lf",PPFactFunction,"PPFactFunction",NULL);
#else
#if MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

/**********************************************/
/* FactRelationFunction: H/L access routine   */
/*   for the fact-relation function.          */
/**********************************************/
void FactRelationFunction(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   struct fact *theFact;

   theFact = GetFactAddressOrIndexArgument(context,false);

   if (theFact == NULL)
     {
      returnValue->lexemeValue = FalseSymbol(theEnv);
      return;
     }

   returnValue->value = FactRelation(theFact);
  }

/**************************************/
/* FactRelation: C access routine for */
/*   the fact-relation function.      */
/**************************************/
CLIPSLexeme *FactRelation(
  Fact *theFact)
  {
   return theFact->whichDeftemplate->header.name;
  }

/****************************************/
/* EnvFactDeftemplate: C access routine */
/*   to retrieve a fact's deftemplate.  */
/****************************************/
Deftemplate *EnvFactDeftemplate(
  Environment *theEnv,
  Fact *theFact)
  {
#if MAC_XCD
#pragma unused(theEnv)
#endif

   return theFact->whichDeftemplate;
  }

/********************************************/
/* FactExistpFunction: H/L access routine   */
/*   for the fact-existp function.          */
/********************************************/
void FactExistpFunction(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   Fact *theFact;

   theFact = GetFactAddressOrIndexArgument(context,false);

   returnValue->lexemeValue = EnvCreateBoolean(theEnv,EnvFactExistp(theEnv,theFact));
  }

/***********************************/
/* EnvFactExistp: C access routine */
/*   for the fact-existp function. */
/***********************************/
bool EnvFactExistp(
  Environment *theEnv,
  Fact *theFact)
  {
#if MAC_XCD
#pragma unused(theEnv)
#endif
   if (theFact == NULL) return false;

   if (theFact->garbage) return false;

   return true;
  }

/***********************************************/
/* FactSlotValueFunction: H/L access routine   */
/*   for the fact-slot-value function.         */
/***********************************************/
void FactSlotValueFunction(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   struct fact *theFact;
   UDFValue theArg;
   CLIPSValue result;

   /*================================*/
   /* Get the reference to the fact. */
   /*================================*/

   theFact = GetFactAddressOrIndexArgument(context,true);
   if (theFact == NULL)
     {
      returnValue->lexemeValue = FalseSymbol(theEnv);
      return;
     }

   /*===========================*/
   /* Get the name of the slot. */
   /*===========================*/

   if (! UDFNextArgument(context,SYMBOL_BIT,&theArg))
     { return; }

   /*=======================*/
   /* Get the slot's value. */
   /*=======================*/

   FactSlotValue(theEnv,theFact,theArg.lexemeValue->contents,&result);
   CLIPSToUDFValue(&result,returnValue);
  }

/***************************************/
/* FactSlotValue: C access routine for */
/*   the fact-slot-value function.     */
/***************************************/
void FactSlotValue(
  Environment *theEnv,
  Fact *theFact,
  const char *theSlotName,
  CLIPSValue *returnValue)
  {
   short position;

   /*==================================================*/
   /* Make sure the slot exists (the symbol implied is */
   /* used for the implied slot of an ordered fact).   */
   /*==================================================*/

   if (theFact->whichDeftemplate->implied)
     {
      if (strcmp(theSlotName,"implied") != 0)
        {
         EnvSetEvaluationError(theEnv,true);
         InvalidDeftemplateSlotMessage(theEnv,theSlotName,
                                       theFact->whichDeftemplate->header.name->contents,false);
         returnValue->lexemeValue = FalseSymbol(theEnv);
         return;
        }
     }

   else if (FindSlot(theFact->whichDeftemplate,EnvCreateSymbol(theEnv,theSlotName),&position) == NULL)
     {
      EnvSetEvaluationError(theEnv,true);
      InvalidDeftemplateSlotMessage(theEnv,theSlotName,
                                    theFact->whichDeftemplate->header.name->contents,false);
      returnValue->lexemeValue = FalseSymbol(theEnv);
      return;
     }

   /*==========================*/
   /* Return the slot's value. */
   /*==========================*/

   if (theFact->whichDeftemplate->implied)
     { EnvGetFactSlot(theEnv,theFact,NULL,returnValue); }
   else
     { EnvGetFactSlot(theEnv,theFact,theSlotName,returnValue); }
  }

/***********************************************/
/* FactSlotNamesFunction: H/L access routine   */
/*   for the fact-slot-names function.         */
/***********************************************/
void FactSlotNamesFunction(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   Fact *theFact;
   CLIPSValue result;

   /*================================*/
   /* Get the reference to the fact. */
   /*================================*/

   theFact = GetFactAddressOrIndexArgument(context,true);
   if (theFact == NULL)
     {
      returnValue->lexemeValue = FalseSymbol(theEnv);
      return;
     }

   /*=====================*/
   /* Get the slot names. */
   /*=====================*/

   EnvFactSlotNames(theEnv,theFact,&result);
   CLIPSToUDFValue(&result,returnValue);
  }

/***************************************/
/* EnvFactSlotNames: C access routine  */
/*   for the fact-slot-names function. */
/***************************************/
void EnvFactSlotNames(
  Environment *theEnv,
  Fact *theFact,
  CLIPSValue *returnValue)
  {
   Multifield *theList;
   struct templateSlot *theSlot;
   unsigned long count;

   /*===============================================*/
   /* If we're dealing with an implied deftemplate, */
   /* then the only slot names is "implied."        */
   /*===============================================*/

   if (theFact->whichDeftemplate->implied)
     {
      theList = EnvCreateMultifield(theEnv,(int) 1);
      theList->theFields[0].lexemeValue = EnvCreateSymbol(theEnv,"implied");
      returnValue->value = theList;
      return;
     }

   /*=================================*/
   /* Count the number of slot names. */
   /*=================================*/

   for (count = 0, theSlot = theFact->whichDeftemplate->slotList;
        theSlot != NULL;
        count++, theSlot = theSlot->next)
     { /* Do Nothing */ }

   /*=============================================================*/
   /* Create a multifield value in which to store the slot names. */
   /*=============================================================*/

   theList = EnvCreateMultifield(theEnv,count);
   returnValue->value = theList;

   /*===============================================*/
   /* Store the slot names in the multifield value. */
   /*===============================================*/

   for (count = 0, theSlot = theFact->whichDeftemplate->slotList;
        theSlot != NULL;
        count++, theSlot = theSlot->next)
     {
      theList->theFields[count].lexemeValue = theSlot->slotName;
     }
  }

/*********************************************/
/* GetFactListFunction: H/L access routine   */
/*   for the get-fact-list function.         */
/*********************************************/
void GetFactListFunction(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   Defmodule *theModule;
   UDFValue theArg;
   CLIPSValue result;

   /*===========================================*/
   /* Determine if a module name was specified. */
   /*===========================================*/

   if (UDFHasNextArgument(context))
     {
      if (! UDFFirstArgument(context,SYMBOL_BIT,&theArg))
        { return; }

      if ((theModule = EnvFindDefmodule(theEnv,theArg.lexemeValue->contents)) == NULL)
        {
         if (strcmp("*",theArg.lexemeValue->contents) != 0)
           {
            EnvSetMultifieldErrorValue(theEnv,returnValue);
            UDFInvalidArgumentMessage(context,"defmodule name");
            return;
           }

         theModule = NULL;
        }
     }
   else
     { theModule = EnvGetCurrentModule(theEnv); }

   /*=====================*/
   /* Get the constructs. */
   /*=====================*/

   EnvGetFactList(theEnv,&result,theModule);
   CLIPSToUDFValue(&result,returnValue);
  }

/*************************************/
/* EnvGetFactList: C access routine  */
/*   for the get-fact-list function. */
/*************************************/
void EnvGetFactList(
  Environment *theEnv,
  CLIPSValue *returnValue,
  Defmodule *theModule)
  {
   Fact *theFact;
   unsigned long count;
   Multifield *theList;

   /*==========================*/
   /* Save the current module. */
   /*==========================*/

   SaveCurrentModule(theEnv);

   /*============================================*/
   /* Count the number of facts to be retrieved. */
   /*============================================*/

   if (theModule == NULL)
     {
      for (theFact = EnvGetNextFact(theEnv,NULL), count = 0;
           theFact != NULL;
           theFact = EnvGetNextFact(theEnv,theFact), count++)
        { /* Do Nothing */ }
     }
   else
     {
      EnvSetCurrentModule(theEnv,theModule);
      UpdateDeftemplateScope(theEnv);
      for (theFact = GetNextFactInScope(theEnv,NULL), count = 0;
           theFact != NULL;
           theFact = GetNextFactInScope(theEnv,theFact), count++)
        { /* Do Nothing */ }
     }

   /*===========================================================*/
   /* Create the multifield value to store the construct names. */
   /*===========================================================*/

   theList = EnvCreateMultifield(theEnv,count);
   returnValue->value = theList;

   /*==================================================*/
   /* Store the fact pointers in the multifield value. */
   /*==================================================*/

   if (theModule == NULL)
     {
      for (theFact = EnvGetNextFact(theEnv,NULL), count = 0;
           theFact != NULL;
           theFact = EnvGetNextFact(theEnv,theFact), count++)
        {
         theList->theFields[count].factValue = theFact;
        }
     }
   else
     {
      for (theFact = GetNextFactInScope(theEnv,NULL), count = 0;
           theFact != NULL;
           theFact = GetNextFactInScope(theEnv,theFact), count++)
        {
         theList->theFields[count].factValue = theFact;
        }
     }

   /*=============================*/
   /* Restore the current module. */
   /*=============================*/

   RestoreCurrentModule(theEnv);
   UpdateDeftemplateScope(theEnv);
  }

/**************************************/
/* PPFactFunction: H/L access routine */
/*   for the ppfact function.         */
/**************************************/
void PPFactFunction(
  Environment *theEnv,
  UDFContext *context,
  UDFValue *returnValue)
  {
   struct fact *theFact;
   const char *logicalName = NULL;      /* Avoids warning */
   bool ignoreDefaults = false;
   UDFValue theArg;

   theFact = GetFactAddressOrIndexArgument(context,true);
   if (theFact == NULL) return;

   /*===============================================================*/
   /* Determine the logical name to which the fact will be printed. */
   /*===============================================================*/

   if (UDFHasNextArgument(context))
     {
      logicalName = GetLogicalName(context,STDOUT);
      if (logicalName == NULL)
        {
         IllegalLogicalNameMessage(theEnv,"ppfact");
         EnvSetHaltExecution(theEnv,true);
         EnvSetEvaluationError(theEnv,true);
         return;
        }
     }
   else
     { logicalName = STDOUT; }

   /*=========================================*/
   /* Should slot values be printed if they   */
   /* are the same as the default slot value. */
   /*=========================================*/

   if (UDFHasNextArgument(context))
     {
      UDFNextArgument(context,ANY_TYPE_BITS,&theArg);

      if (theArg.value == FalseSymbol(theEnv))
        { ignoreDefaults = false; }
      else
        { ignoreDefaults = true; }
     }

   /*============================================================*/
   /* Determine if any router recognizes the output destination. */
   /*============================================================*/

   if (strcmp(logicalName,"nil") == 0)
     { return; }
   else if (QueryRouters(theEnv,logicalName) == false)
     {
      UnrecognizedRouterMessage(theEnv,logicalName);
      return;
     }

   EnvPPFact(theEnv,theFact,logicalName,ignoreDefaults);
  }

/*******************************/
/* EnvPPFact: C access routine */
/*   for the ppfact function.  */
/*******************************/
void EnvPPFact(
  Environment *theEnv,
  Fact *theFact,
  const char *logicalName,
  bool ignoreDefaults)
  {
#if MAC_XCD
#pragma unused(theEnv)
#endif

   if (theFact == NULL) return;

   if (theFact->garbage) return;

   PrintFact(theEnv,logicalName,theFact,true,ignoreDefaults);

   EnvPrintRouter(theEnv,logicalName,"\n");
  }

/**************************************************************/
/* GetFactAddressOrIndexArgument: Retrieves an argument for a */
/*   function which should be a reference to a valid fact.    */
/**************************************************************/
Fact *GetFactAddressOrIndexArgument(
  UDFContext *context,
  bool noFactError)
  {
   UDFValue theArg;
   long long factIndex;
   Fact *theFact;
   Environment *theEnv = context->environment;
   char tempBuffer[20];

   if (! UDFNextArgument(context,ANY_TYPE_BITS,&theArg))
     { return NULL; }

   if (theArg.header->type == FACT_ADDRESS_TYPE)
     {
      if (theArg.factValue->garbage) return NULL;
      else return theArg.factValue;
     }
   else if (theArg.header->type == INTEGER_TYPE)
     {
      factIndex = theArg.integerValue->contents;
      if (factIndex < 0)
        {
         UDFInvalidArgumentMessage(context,"fact-address or fact-index");
         return NULL;
        }

      theFact = FindIndexedFact(theEnv,factIndex);
      if ((theFact == NULL) && noFactError)
        {
         gensprintf(tempBuffer,"f-%lld",factIndex);
         CantFindItemErrorMessage(theEnv,"fact",tempBuffer);
         return NULL;
        }

      return(theFact);
     }

   UDFInvalidArgumentMessage(context,"fact-address or fact-index");
   return NULL;
  }

#endif /* DEFTEMPLATE_CONSTRUCT */


