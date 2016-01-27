   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/20/16             */
   /*                                                     */
   /*             BASIC MATH FUNCTIONS MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Contains the code for numerous basic math        */
/*   functions including +, *, -, /, integer, float, div,    */
/*   abs,set-auto-float-dividend, get-auto-float-dividend,   */
/*   min, and max.                                           */
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
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Added Env prefix to GetEvaluationError and     */
/*            SetEvaluationError functions.                  */
/*                                                           */
/*            Added Env prefix to GetHaltExecution and       */
/*            SetHaltExecution functions.                    */
/*                                                           */
/*            Auto-float-dividend always enabled.            */
/*                                                           */
/*************************************************************/

#include <stdio.h>

#include "setup.h"

#include "argacces.h"
#include "envrnmnt.h"
#include "exprnpsr.h"
#include "router.h"

#include "bmathfun.h"

/***************************************************************/
/* BasicMathFunctionDefinitions: Defines basic math functions. */
/***************************************************************/
void BasicMathFunctionDefinitions(
  void *theEnv)
  {
#if ! RUN_TIME
   EnvAddUDF(theEnv, "+",       NUMBER_TYPES, AdditionFunction, "AdditionFunction", 2,UNBOUNDED, "ld" ,NULL);
   EnvAddUDF(theEnv, "*",       NUMBER_TYPES, MultiplicationFunction, "MultiplicationFunction",  2,UNBOUNDED, "ld", NULL);
   EnvAddUDF(theEnv, "-",       NUMBER_TYPES, SubtractionFunction, "SubtractionFunction",  2,UNBOUNDED, "ld", NULL);
   EnvAddUDF(theEnv, "/",       FLOAT_TYPE,   DivisionFunction, "DivisionFunction", 2,UNBOUNDED, "ld", NULL);
   EnvAddUDF(theEnv, "div",     INTEGER_TYPE, DivFunction, "DivFunction",  2,UNBOUNDED, "ld", NULL);
   
   EnvAddUDF(theEnv, "integer", INTEGER_TYPE, IntegerFunction,"IntegerFunction",1,1,"ld",NULL);
   EnvAddUDF(theEnv, "float",   FLOAT_TYPE,   FloatFunction,"FloatFunction",1,1,"ld",NULL);
   EnvAddUDF(theEnv, "abs",     NUMBER_TYPES, AbsFunction,"AbsFunction",1,1,"ld",NULL);
   EnvAddUDF(theEnv, "min",     NUMBER_TYPES, MinFunction,"MinFunction",1,UNBOUNDED,"ld",NULL);
   EnvAddUDF(theEnv, "max",     NUMBER_TYPES, MaxFunction,"MaxFunction",1,UNBOUNDED,"ld",NULL);
#endif
  }

/**********************************/
/* AdditionFunction: H/L access   */
/*   routine for the + function.  */
/**********************************/
void AdditionFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSFloat ftotal = 0.0;
   CLIPSInteger ltotal = 0LL;
   bool useFloatTotal = false;
   CLIPSValue theArg;

   /*=================================================*/
   /* Loop through each of the arguments adding it to */
   /* a running total. If a floating point number is  */
   /* encountered, then do all subsequent operations  */
   /* using floating point values.                    */
   /*=================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&theArg))
        {
         if (useFloatTotal)
           { CVSetFloat(returnValue,ftotal); }
         else
           { CVSetInteger(returnValue,ltotal); }
         return;
        }

      if (useFloatTotal)
        { ftotal += CVToFloat(&theArg); }
      else
        {
         if (CVIsType(&theArg,INTEGER_TYPE))
           { ltotal += CVToInteger(&theArg); }
         else
           {
            ftotal = ((CLIPSFloat) ltotal) + CVToFloat(&theArg);
            useFloatTotal = true;
           }
        }
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     { CVSetFloat(returnValue,ftotal); }
   else
     { CVSetInteger(returnValue,ltotal); }
  }

/****************************************/
/* MultiplicationFunction: CLIPS access */
/*   routine for the * function.        */
/****************************************/
void MultiplicationFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSFloat ftotal = 1.0;
   CLIPSInteger ltotal = 1LL;
   bool useFloatTotal = false;
   CLIPSValue theArg;

   /*===================================================*/
   /* Loop through each of the arguments multiplying it */
   /* by a running product. If a floating point number  */
   /* is encountered, then do all subsequent operations */
   /* using floating point values.                      */
   /*===================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&theArg))
        {
         if (useFloatTotal)
           { CVSetFloat(returnValue,ftotal); }
         else
           { CVSetInteger(returnValue,ltotal); }
         return;
        }

      if (useFloatTotal)
        { ftotal *= CVToFloat(&theArg); }
      else
        {
         if (CVIsType(&theArg,INTEGER_TYPE))
           { ltotal *= CVToInteger(&theArg); }
         else
           {
            ftotal = ((CLIPSFloat) ltotal) * CVToFloat(&theArg);
            useFloatTotal = true;
           }
        }
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     { CVSetFloat(returnValue,ftotal); }
   else
     { CVSetInteger(returnValue,ltotal); }
  }

/*************************************/
/* SubtractionFunction: CLIPS access */
/*   routine for the - function.     */
/*************************************/
void SubtractionFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSFloat ftotal = 0.0;
   CLIPSInteger ltotal = 0LL;
   bool useFloatTotal = false;
   CLIPSValue theArg;

   /*=================================================*/
   /* Get the first argument. This number which will  */
   /* be the starting total from which all subsequent */
   /* arguments will subtracted.                      */
   /*=================================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&theArg))
     {
      CVSetInteger(returnValue,0L);
      return;
     }

   if (CVIsType(&theArg,INTEGER_TYPE))
     { ltotal = CVToInteger(&theArg); }
   else
     {
      ftotal = CVToFloat(&theArg);
      useFloatTotal = true;
     }

   /*===================================================*/
   /* Loop through each of the arguments subtracting it */
   /* from a running total. If a floating point number  */
   /* is encountered, then do all subsequent operations */
   /* using floating point values.                      */
   /*===================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&theArg))
        {
         if (useFloatTotal)
           { CVSetFloat(returnValue,ftotal); }
         else
           { CVSetInteger(returnValue,ltotal); }
         return;
        }

      if (useFloatTotal)
        { ftotal -= CVToFloat(&theArg); }
      else
        {
         if (CVIsType(&theArg,INTEGER_TYPE))
           { ltotal -= CVToInteger(&theArg); }
         else
           {
            ftotal = ((CLIPSFloat) ltotal) - CVToFloat(&theArg);
            useFloatTotal = true;
           }
        }
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     { CVSetFloat(returnValue,ftotal); }
   else
     { CVSetInteger(returnValue,ltotal); }
  }

/***********************************/
/* DivisionFunction:  CLIPS access */
/*   routine for the / function.   */
/***********************************/
void DivisionFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSFloat ftotal = 1.0;
   CLIPSFloat theNumber;
   CLIPSValue theArg;
   Environment *theEnv = UDFContextEnvironment(context);
   
   /*===================================================*/
   /* Get the first argument. This number which will be */
   /* the starting product from which all subsequent    */
   /* arguments will divide. If the auto float dividend */
   /* feature is enable, then this number is converted  */
   /* to a float if it is an integer.                   */
   /*===================================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&theArg))
     {
      CVSetFloat(returnValue,ftotal);
      return;
     }

   ftotal = CVToFloat(&theArg);

   /*====================================================*/
   /* Loop through each of the arguments dividing it     */
   /* into a running product. If a floating point number */
   /* is encountered, then do all subsequent operations  */
   /* using floating point values. Each argument is      */
   /* checked to prevent a divide by zero error.         */
   /*====================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&theArg))
        {
         CVSetFloat(returnValue,ftotal);
         return;
        }
        
      theNumber = CVToFloat(&theArg);
      
      if (theNumber == 0.0)
        {
         DivideByZeroErrorMessage(theEnv,"/");
         EnvSetEvaluationError(theEnv,true);
         CVSetFloat(returnValue,1.0);
         return;
        }

      ftotal /= theNumber;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   CVSetFloat(returnValue,ftotal);
  }

/*************************************/
/* DivFunction: H/L access routine   */
/*   for the div function.           */
/*************************************/
void DivFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSInteger total = 1LL;
   DATA_OBJECT theArg;
   CLIPSInteger theNumber;
   void *theEnv = UDFContextEnvironment(context);

   /*===================================================*/
   /* Get the first argument. This number which will be */
   /* the starting product from which all subsequent    */
   /* arguments will divide.                            */
   /*===================================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&theArg))
     {
      CVSetInteger(returnValue,1L);
      return;
     }
   total = CVToInteger(&theArg);

   /*=====================================================*/
   /* Loop through each of the arguments dividing it into */
   /* a running product. Floats are converted to integers */
   /* and each argument is checked to prevent a divide by */
   /* zero error.                                         */
   /*=====================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&theArg))
        {
         CVSetInteger(returnValue,total);
         return;
        }

      theNumber = CVToInteger(&theArg);

      if (theNumber == 0LL)
        {
         DivideByZeroErrorMessage(theEnv,"div");
         EnvSetEvaluationError(theEnv,true);
         CVSetInteger(returnValue,1L);
         return;
        }
        
      total /= theNumber;
     }

   /*======================================================*/
   /* The result of the div function is always an integer. */
   /*======================================================*/

   CVSetInteger(returnValue,total);
  }

/*****************************************/
/* IntegerFunction: H/L access routine   */
/*   for the integer function.           */
/*****************************************/
void IntegerFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   /*======================================*/
   /* Check that the argument is a number. */
   /*======================================*/

   if (! UDFNthArgument(context,1,NUMBER_TYPES,returnValue))
     {
      CVSetInteger(returnValue,0LL);
      return;
     }

   /*============================================*/
   /* Convert a float type to integer, otherwise */
   /* return the argument unchanged.             */
   /*============================================*/

   if (CVIsType(returnValue,FLOAT_TYPE))
     { CVSetInteger(returnValue,CVToInteger(returnValue)); }
  }

/***************************************/
/* FloatFunction: H/L access routine   */
/*   for the float function.           */
/***************************************/
void FloatFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   /*======================================*/
   /* Check that the argument is a number. */
   /*======================================*/

   if (! UDFNthArgument(context,1,NUMBER_TYPES,returnValue))
     {
      CVSetFloat(returnValue,0.0);
      return;
     }

   /*=============================================*/
   /* Convert an integer type to float, otherwise */
   /* return the argument unchanged.              */
   /*=============================================*/

   if (CVIsType(returnValue,INTEGER_TYPE))
     { CVSetFloat(returnValue,CVToFloat(returnValue)); }
  }

/*************************************/
/* AbsFunction: H/L access routine   */
/*   for the abs function.           */
/*************************************/
void AbsFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   /*======================================*/
   /* Check that the argument is a number. */
   /*======================================*/

   if (! UDFNthArgument(context,1,NUMBER_TYPES,returnValue))
     {
      CVSetInteger(returnValue,0LL);
      return;
     }

   /*==========================================*/
   /* Return the absolute value of the number. */
   /*==========================================*/

   if (CVIsType(returnValue,INTEGER_TYPE))
     {
      CLIPSInteger lv = CVToInteger(returnValue);
      if (lv < 0L) CVSetInteger(returnValue,-lv);
     }
   else
     {
      CLIPSFloat dv = CVToFloat(returnValue);
      if (dv < 0.0) CVSetFloat(returnValue,-dv);
     }
  }

/*************************************/
/* MinFunction: H/L access routine   */
/*   for the min function.           */
/*************************************/
void MinFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue nextPossible;

   /*============================================*/
   /* Check that the first argument is a number. */
   /*============================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,returnValue))
     {
      CVSetInteger(returnValue,0LL);
      return;
     }
    
   /*===========================================================*/
   /* Loop through the remaining arguments, first checking each */
   /* argument to see that it is a number, and then determining */
   /* if the argument is less than the previous arguments and   */
   /* is thus the maximum value.                                */
   /*===========================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&nextPossible))
        { return; }
      
      /*=============================================*/
      /* If either argument is a float, convert both */
      /* to floats. Otherwise compare two integers.  */
      /*=============================================*/
      
      if (CVIsType(returnValue,FLOAT_TYPE) || CVIsType(&nextPossible,FLOAT_TYPE))
        {
         if (CVToFloat(returnValue) > CVToFloat(&nextPossible))
           { CVSetCLIPSValue(returnValue,&nextPossible); }
        }
      else
        {
         if (CVToInteger(returnValue) > CVToInteger(&nextPossible))
           { CVSetCLIPSValue(returnValue,&nextPossible); }
        }
     }
  }

/*************************************/
/* MaxFunction: H/L access routine   */
/*   for the max function.           */
/*************************************/
void MaxFunction(
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue nextPossible;
   
   /*============================================*/
   /* Check that the first argument is a number. */
   /*============================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,returnValue))
     {
      CVSetInteger(returnValue,0LL);
      return;
     }

   /*===========================================================*/
   /* Loop through the remaining arguments, first checking each */
   /* argument to see that it is a number, and then determining */
   /* if the argument is greater than the previous arguments    */
   /* and is thus the maximum value.                            */
   /*===========================================================*/

   while (UDFHasNextArgument(context))
     {
      if (! UDFNextArgument(context,NUMBER_TYPES,&nextPossible))
        { return; }
      
      /*=============================================*/
      /* If either argument is a float, convert both */
      /* to floats. Otherwise compare two integers.  */
      /*=============================================*/
      
      if (CVIsType(returnValue,FLOAT_TYPE) || CVIsType(&nextPossible,FLOAT_TYPE))
        {
         if (CVToFloat(returnValue) < CVToFloat(&nextPossible))
           { CVSetCLIPSValue(returnValue,&nextPossible); }
        }
      else
        {
         if (CVToInteger(returnValue) < CVToInteger(&nextPossible))
           { CVSetCLIPSValue(returnValue,&nextPossible); }
        }
     }
  }

