   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/06/16             */
   /*                                                     */
   /*              PREDICATE FUNCTIONS MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for several predicate          */
/*   functions including not, and, or, eq, neq, <=, >=, <,   */
/*   >, =, <>, symbolp, stringp, lexemep, numberp, integerp, */
/*   floatp, oddp, evenp, multifieldp, sequencep, and        */
/*   pointerp.                                               */
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
/*************************************************************/

#include <stdio.h>

#include "setup.h"

#include "argacces.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "multifld.h"
#include "router.h"

#include "prdctfun.h"

/**************************************************/
/* PredicateFunctionDefinitions: Defines standard */
/*   math and predicate functions.                */
/**************************************************/
void PredicateFunctionDefinitions(
  void *theEnv)
  {
#if ! RUN_TIME
   EnvAddUDF(theEnv,"not", BOOLEAN_TYPE, NotFunction, "NotFunction",   1,1,          NULL,NULL);
   EnvAddUDF(theEnv,"and", BOOLEAN_TYPE, AndFunction, "AndFunction",   2,UNBOUNDED , NULL,NULL);
   EnvAddUDF(theEnv,"or",  BOOLEAN_TYPE, OrFunction,  "OrFunction",    2,UNBOUNDED , NULL,NULL);

   EnvDefineFunction2(theEnv,"eq", 'b', PTIEF EqFunction, "EqFunction", "2*");
   EnvDefineFunction2(theEnv,"neq", 'b', PTIEF NeqFunction, "NeqFunction", "2*");

   EnvAddUDF(theEnv,"<=", BOOLEAN_TYPE, LessThanOrEqualFunction,    "LessThanOrEqualFunction",    2,UNBOUNDED , "ld",NULL);
   EnvAddUDF(theEnv,">=", BOOLEAN_TYPE, GreaterThanOrEqualFunction, "GreaterThanOrEqualFunction", 2,UNBOUNDED , "ld",NULL);
   EnvAddUDF(theEnv,"<",  BOOLEAN_TYPE, LessThanFunction,           "LessThanFunction",           2,UNBOUNDED , "ld",NULL);
   EnvAddUDF(theEnv,">",  BOOLEAN_TYPE, GreaterThanFunction,        "GreaterThanFunction",        2,UNBOUNDED , "ld",NULL);
   EnvAddUDF(theEnv,"=",  BOOLEAN_TYPE, NumericEqualFunction,       "NumericEqualFunction",       2,UNBOUNDED , "ld",NULL);
   EnvAddUDF(theEnv,"<>", BOOLEAN_TYPE, NumericNotEqualFunction,    "NumericNotEqualFunction",    2,UNBOUNDED , "ld",NULL);
   EnvAddUDF(theEnv,"!=", BOOLEAN_TYPE, NumericNotEqualFunction,    "NumericNotEqualFunction",    2,UNBOUNDED , "ld",NULL);

   EnvAddUDF(theEnv,"symbolp",     BOOLEAN_TYPE,  SymbolpFunction,     "SymbolpFunction", 1,1,NULL,NULL);
   EnvAddUDF(theEnv,"wordp",       BOOLEAN_TYPE,  SymbolpFunction,     "SymbolpFunction", 1,1,NULL,NULL);  // TBD Remove?
   EnvAddUDF(theEnv,"stringp",     BOOLEAN_TYPE,  StringpFunction,     "StringpFunction", 1,1,NULL,NULL);
   EnvAddUDF(theEnv,"lexemep",     BOOLEAN_TYPE,  LexemepFunction,     "LexemepFunction", 1,1,NULL,NULL);
   EnvAddUDF(theEnv,"numberp",     BOOLEAN_TYPE,  NumberpFunction,     "NumberpFunction", 1,1,NULL,NULL);
   EnvAddUDF(theEnv,"integerp",    BOOLEAN_TYPE,  IntegerpFunction,    "IntegerpFunction", 1,1,NULL,NULL);
   EnvAddUDF(theEnv,"floatp",      BOOLEAN_TYPE,  FloatpFunction,      "FloatpFunction", 1,1,NULL,NULL);
   EnvAddUDF(theEnv,"oddp",        BOOLEAN_TYPE,  OddpFunction,        "OddpFunction", 1,1,"l", NULL);
   EnvAddUDF(theEnv,"evenp",       BOOLEAN_TYPE,  EvenpFunction,       "EvenpFunction",  1,1,"l", NULL);
   EnvAddUDF(theEnv,"multifieldp", BOOLEAN_TYPE,  MultifieldpFunction, "MultifieldpFunction", 1,1,NULL,NULL);
   EnvAddUDF(theEnv,"sequencep",   BOOLEAN_TYPE,  MultifieldpFunction, "MultifieldpFunction", 1,1,NULL,NULL); // TBD Remove?
   EnvAddUDF(theEnv,"pointerp",    BOOLEAN_TYPE,  PointerpFunction,    "PointerpFunction", 1,1,NULL,NULL);
#else
#if MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }

/************************************/
/* EqFunction: H/L access routine   */
/*   for the eq function.           */
/************************************/
bool EqFunction(
  void *theEnv)
  {
   DATA_OBJECT item, nextItem;
   int numArgs, i;
   struct expr *theExpression;

   /*====================================*/
   /* Determine the number of arguments. */
   /*====================================*/

   numArgs = EnvRtnArgCount(theEnv);
   if (numArgs == 0) return(false);

   /*==============================================*/
   /* Get the value of the first argument against  */
   /* which subsequent arguments will be compared. */
   /*==============================================*/

   theExpression = GetFirstArgument();
   EvaluateExpression(theEnv,theExpression,&item);

   /*=====================================*/
   /* Compare all arguments to the first. */
   /* If any are the same, return false.  */
   /*=====================================*/

   theExpression = GetNextArgument(theExpression);
   for (i = 2 ; i <= numArgs ; i++)
     {
      EvaluateExpression(theEnv,theExpression,&nextItem);

      if (GetType(nextItem) != GetType(item))
        { return(false); }

      if (GetType(nextItem) == MULTIFIELD)
        {
         if (MultifieldDOsEqual(&nextItem,&item) == false)
           { return(false); }
        }
      else if (nextItem.value != item.value)
        { return(false); }

      theExpression = GetNextArgument(theExpression);
     }

   /*=====================================*/
   /* All of the arguments were different */
   /* from the first. Return true.        */
   /*=====================================*/

   return(true);
  }

/*************************************/
/* NeqFunction: H/L access routine   */
/*   for the neq function.           */
/*************************************/
bool NeqFunction(
  void *theEnv)
  {
   DATA_OBJECT item, nextItem;
   int numArgs, i;
   struct expr *theExpression;

   /*====================================*/
   /* Determine the number of arguments. */
   /*====================================*/

   numArgs = EnvRtnArgCount(theEnv);
   if (numArgs == 0) return(false);

   /*==============================================*/
   /* Get the value of the first argument against  */
   /* which subsequent arguments will be compared. */
   /*==============================================*/

   theExpression = GetFirstArgument();
   EvaluateExpression(theEnv,theExpression,&item);

   /*=====================================*/
   /* Compare all arguments to the first. */
   /* If any are different, return false. */
   /*=====================================*/

   for (i = 2, theExpression = GetNextArgument(theExpression);
        i <= numArgs;
        i++, theExpression = GetNextArgument(theExpression))
     {
      EvaluateExpression(theEnv,theExpression,&nextItem);
      if (GetType(nextItem) != GetType(item))
        { continue; }
      else if (nextItem.type == MULTIFIELD)
        {
         if (MultifieldDOsEqual(&nextItem,&item) == true)
           { return(false); }
        }
      else if (nextItem.value == item.value)
        { return(false); }
     }

   /*=====================================*/
   /* All of the arguments were identical */
   /* to the first. Return true.          */
   /*=====================================*/

   return(true);
  }

/*****************************************/
/* StringpFunction: H/L access routine   */
/*   for the stringp function.           */
/*****************************************/
void StringpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,STRING_TYPE))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/*****************************************/
/* SymbolpFunction: H/L access routine   */
/*   for the symbolp function.           */
/*****************************************/
void SymbolpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,SYMBOL_TYPE))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/*****************************************/
/* LexemepFunction: H/L access routine   */
/*   for the lexemep function.           */
/*****************************************/
void LexemepFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,LEXEME_TYPES))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/*****************************************/
/* NumberpFunction: H/L access routine   */
/*   for the numberp function.           */
/*****************************************/
void NumberpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,NUMBER_TYPES))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/****************************************/
/* FloatpFunction: H/L access routine   */
/*   for the floatp function.           */
/****************************************/
void FloatpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,FLOAT_TYPE))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/******************************************/
/* IntegerpFunction: H/L access routine   */
/*   for the integerp function.           */
/******************************************/
void IntegerpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,INTEGER_TYPE))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/*********************************************/
/* MultifieldpFunction: H/L access routine   */
/*   for the multifieldp function.           */
/*********************************************/
void MultifieldpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,MULTIFIELD_TYPE))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/******************************************/
/* PointerpFunction: H/L access routine   */
/*   for the pointerp function.           */
/******************************************/
void PointerpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;

   if (! UDFFirstArgument(context,ANY_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsType(&item,EXTERNAL_ADDRESS_TYPE))
     { CVSetBoolean(returnValue,true); }
   else
     { CVSetBoolean(returnValue,false); }
  }

/***********************************/
/* NotFunction: H/L access routine */
/*   for the not function.         */
/***********************************/
void NotFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   DATA_OBJECT result;

   if (! UDFFirstArgument(context,ANY_TYPE,&result))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   if (CVIsFalseSymbol(&result))
     {
      CVSetBoolean(returnValue,true);
      return;
     }

   CVSetBoolean(returnValue,false);
  }

/*************************************/
/* AndFunction: H/L access routine   */
/*   for the and function.           */
/*************************************/
void AndFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   DATA_OBJECT result;

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,ANY_TYPE,&result))
        {
         CVSetBoolean(returnValue,false);
         return;
        }
        
      if (CVIsFalseSymbol(&result))
        {
         CVSetBoolean(returnValue,false);
         return;
        }
     }

   CVSetBoolean(returnValue,true);
  }

/************************************/
/* OrFunction: H/L access routine   */
/*   for the or function.           */
/************************************/
void OrFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   DATA_OBJECT result;

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,ANY_TYPE,&result))
        {
         CVSetBoolean(returnValue,false);
         return;
        }
        
      if (! CVIsFalseSymbol(&result))
        {
         CVSetBoolean(returnValue,true);
         return;
        }
     }

   CVSetBoolean(returnValue,false);
  }

/*****************************************/
/* LessThanOrEqualFunction: H/L access   */
/*   routine for the <= function.        */
/*****************************************/
void LessThanOrEqualFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   DATA_OBJECT rv1, rv2;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&rv1))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   /*====================================================*/
   /* Compare each of the subsequent arguments to its    */
   /* predecessor. If any is greater, then return false. */
   /*====================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&rv2))
        {
         CVSetBoolean(returnValue,false);
         return;
        }

      if (CVIsType(&rv1,INTEGER_TYPE) && CVIsType(&rv2,INTEGER_TYPE))
        {
         if (CVToInteger(&rv1) > CVToInteger(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
      else
        {
         if (CVToFloat(&rv1) > CVToFloat(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }

      CVSetCLIPSValue(&rv1,&rv2);
     }

   /*======================================*/
   /* Each argument was less than or equal */
   /* to its predecessor. Return true.     */
   /*======================================*/

   CVSetBoolean(returnValue,true);
  }

/********************************************/
/* GreaterThanOrEqualFunction: H/L access   */
/*   routine for the >= function.           */
/********************************************/
void GreaterThanOrEqualFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   DATA_OBJECT rv1, rv2;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&rv1))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   /*===================================================*/
   /* Compare each of the subsequent arguments to its   */
   /* predecessor. If any is lesser, then return false. */
   /*===================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&rv2))
        {
         CVSetBoolean(returnValue,false);
         return;
        }

      if (CVIsType(&rv1,INTEGER_TYPE) && CVIsType(&rv2,INTEGER_TYPE))
        {
         if (CVToInteger(&rv1) < CVToInteger(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
      else
        {
         if (CVToFloat(&rv1) < CVToFloat(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }

      CVSetCLIPSValue(&rv1,&rv2);
     }

   /*=========================================*/
   /* Each argument was greater than or equal */
   /* to its predecessor. Return true.        */
   /*=========================================*/

   CVSetBoolean(returnValue,true);
  }

/**********************************/
/* LessThanFunction: H/L access   */
/*   routine for the < function.  */
/**********************************/
void LessThanFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue rv1, rv2;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&rv1))
     {
      CVSetBoolean(returnValue,false);
      return;
     }
   
   /*==========================================*/
   /* Compare each of the subsequent arguments */
   /* to its predecessor. If any is greater or */
   /* equal, then return false.                */
   /*==========================================*/
   
   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&rv2))
        {
         CVSetBoolean(returnValue,false);
         return;
        }
        
      if (CVIsType(&rv1,INTEGER_TYPE) && CVIsType(&rv2,INTEGER_TYPE))
        {
         if (CVToInteger(&rv1) >= CVToInteger(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
      else
        {
         if (CVToFloat(&rv1) >= CVToFloat(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
        
      CVSetCLIPSValue(&rv1,&rv2);
     }

   /*=================================*/
   /* Each argument was less than its */
   /* predecessor. Return true.       */
   /*=================================*/

   CVSetBoolean(returnValue,true);
  }

/*************************************/
/* GreaterThanFunction: H/L access   */
/*   routine for the > function.     */
/*************************************/
void GreaterThanFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue rv1, rv2;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&rv1))
     {
      CVSetBoolean(returnValue,false);
      return;
     }
   
   /*==========================================*/
   /* Compare each of the subsequent arguments */
   /* to its predecessor. If any is lesser or  */
   /* equal, then return false.                */
   /*==========================================*/
   
   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&rv2))
        {
         CVSetBoolean(returnValue,false);
         return;
        }
        
      if (CVIsType(&rv1,INTEGER_TYPE) && CVIsType(&rv2,INTEGER_TYPE))
        {
         if (CVToInteger(&rv1) <= CVToInteger(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
      else
        {
         if (CVToFloat(&rv1) <= CVToFloat(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
        
      CVSetCLIPSValue(&rv1,&rv2);
     }

   /*=================================*/
   /* Each argument was less than its */
   /* predecessor. Return true.       */
   /*=================================*/

   CVSetBoolean(returnValue,true);
  }

/**************************************/
/* NumericEqualFunction: H/L access   */
/*   routine for the = function.      */
/**************************************/
void NumericEqualFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue rv1, rv2;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&rv1))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   /*=================================================*/
   /* Compare each of the subsequent arguments to the */
   /* first. If any is unequal, then return false.    */
   /*=================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&rv2))
        {
         CVSetBoolean(returnValue,false);
         return;
        }
        
      if (CVIsType(&rv1,INTEGER_TYPE) && CVIsType(&rv2,INTEGER_TYPE))
        {
         if (CVToInteger(&rv1) != CVToInteger(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
      else
        {
         if (CVToFloat(&rv1) != CVToFloat(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
     }
     
   /*=================================*/
   /* All arguments were equal to the */
   /* first argument. Return true.    */
   /*=================================*/

   CVSetBoolean(returnValue,true);
  }

/*****************************************/
/* NumericNotEqualFunction: H/L access   */
/*   routine for the <> function.        */
/*****************************************/
void NumericNotEqualFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue rv1, rv2;

   /*=========================*/
   /* Get the first argument. */
   /*=========================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&rv1))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   /*=================================================*/
   /* Compare each of the subsequent arguments to the */
   /* first. If any is equal, then return false.      */
   /*=================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&rv2))
        {
         CVSetBoolean(returnValue,false);
         return;
        }
        
      if (CVIsType(&rv1,INTEGER_TYPE) && CVIsType(&rv2,INTEGER_TYPE))
        {
         if (CVToInteger(&rv1) == CVToInteger(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
      else
        {
         if (CVToFloat(&rv1) == CVToFloat(&rv2))
           {
            CVSetBoolean(returnValue,false);
            return;
           }
        }
     }
     
   /*===================================*/
   /* All arguments were unequal to the */
   /* first argument. Return true.      */
   /*===================================*/

   CVSetBoolean(returnValue,true);
  }

/**************************************/
/* OddpFunction: H/L access routine   */
/*   for the oddp function.           */
/**************************************/
void OddpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;
   CLIPSInteger num, halfnum;
      
   /*===========================================*/
   /* Check for the correct types of arguments. */
   /*===========================================*/
 
   if (! UDFFirstArgument(context,INTEGER_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }
    
   /*===========================*/
   /* Compute the return value. */
   /*===========================*/
   
   num = CVToInteger(&item);
   halfnum = (num / 2) * 2;

   if (num == halfnum) CVSetBoolean(returnValue,false);
   else CVSetBoolean(returnValue,true);
  }

/***************************************/
/* EvenpFunction: H/L access routine   */
/*   for the evenp function.           */
/***************************************/
void EvenpFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue item;
   CLIPSInteger num, halfnum;
   
   /*===========================================*/
   /* Check for the correct types of arguments. */
   /*===========================================*/
     
   if (! UDFFirstArgument(context,INTEGER_TYPE,&item))
     {
      CVSetBoolean(returnValue,false);
      return;
     }

   /*===========================*/
   /* Compute the return value. */
   /*===========================*/
   
   num = CVToInteger(&item);
   halfnum = (num / 2) * 2;
   
   if (num != halfnum) CVSetBoolean(returnValue,false);
   else CVSetBoolean(returnValue,true);
  }



