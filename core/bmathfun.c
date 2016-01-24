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
   EnvDefineFunction2(theEnv,"+", 'n',PTIEF AdditionFunction, "AdditionFunction", "2*n");
   EnvDefineFunction2(theEnv,"*", 'n', PTIEF MultiplicationFunction, "MultiplicationFunction", "2*n");
   EnvDefineFunction2(theEnv,"-", 'n', PTIEF SubtractionFunction, "SubtractionFunction", "2*n");
   EnvDefineFunction2(theEnv,"/", 'n', PTIEF DivisionFunction, "DivisionFunction", "2*n");
   EnvDefineFunction2(theEnv,"div", 'g', PTIEF DivFunction, "DivFunction", "2*n");
   
   EnvAddUDF(theEnv,"integer",INTEGER_TYPE,IntegerFunction,"IntegerFunction",1,1,"ld",NULL);
   EnvAddUDF(theEnv,"float",FLOAT_TYPE,FloatFunction,"FloatFunction",1,1,"ld",NULL);
   EnvAddUDF(theEnv,"abs",NUMBER_TYPES,AbsFunction,"AbsFunction",1,1,"ld",NULL);
   EnvAddUDF(theEnv,"min",NUMBER_TYPES,MinFunction,"MinFunction",1,UNBOUNDED,"ld",NULL);
   EnvAddUDF(theEnv,"max",NUMBER_TYPES,MaxFunction,"MaxFunction",1,UNBOUNDED,"ld",NULL);
#endif
  }

/**********************************/
/* AdditionFunction: H/L access   */
/*   routine for the + function.  */
/**********************************/
void AdditionFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 0.0;
   long long ltotal = 0LL;
   bool useFloatTotal = false;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;

   /*=================================================*/
   /* Loop through each of the arguments adding it to */
   /* a running total. If a floating point number is  */
   /* encountered, then do all subsequent operations  */
   /* using floating point values.                    */
   /*=================================================*/

   theExpression = GetFirstArgument();

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"+",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (useFloatTotal)
        { ftotal += ValueToDouble(theArgument.value); }
      else
        {
         if (theArgument.type == INTEGER)
           { ltotal += ValueToLong(theArgument.value); }
         else
           {
            ftotal = (double) ltotal + ValueToDouble(theArgument.value);
            useFloatTotal = true;
           }
        }

      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
     }
   else
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,ltotal);
     }
  }

/****************************************/
/* MultiplicationFunction: CLIPS access */
/*   routine for the * function.        */
/****************************************/
void MultiplicationFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 1.0;
   long long ltotal = 1LL;
   bool useFloatTotal = false;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;

   /*===================================================*/
   /* Loop through each of the arguments multiplying it */
   /* by a running product. If a floating point number  */
   /* is encountered, then do all subsequent operations */
   /* using floating point values.                      */
   /*===================================================*/

   theExpression = GetFirstArgument();

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"*",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (useFloatTotal)
        { ftotal *= ValueToDouble(theArgument.value); }
      else
        {
         if (theArgument.type == INTEGER)
           { ltotal *= ValueToLong(theArgument.value); }
         else
           {
            ftotal = (double) ltotal * ValueToDouble(theArgument.value);
            useFloatTotal = true;
           }
        }
      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
     }
   else
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,ltotal);
     }
  }

/*************************************/
/* SubtractionFunction: CLIPS access */
/*   routine for the - function.     */
/*************************************/
void SubtractionFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 0.0;
   long long ltotal = 0LL;
   bool useFloatTotal = false;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;

   /*=================================================*/
   /* Get the first argument. This number which will  */
   /* be the starting total from which all subsequent */
   /* arguments will subtracted.                      */
   /*=================================================*/

   theExpression = GetFirstArgument();
   if (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"-",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER)
        { ltotal = ValueToLong(theArgument.value); }
      else
        {
         ftotal = ValueToDouble(theArgument.value);
         useFloatTotal = true;
        }
      pos++;
     }

   /*===================================================*/
   /* Loop through each of the arguments subtracting it */
   /* from a running total. If a floating point number  */
   /* is encountered, then do all subsequent operations */
   /* using floating point values.                      */
   /*===================================================*/

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"-",&theArgument,useFloatTotal,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (useFloatTotal)
        { ftotal -= ValueToDouble(theArgument.value); }
      else
        {
         if (theArgument.type == INTEGER)
           { ltotal -= ValueToLong(theArgument.value); }
         else
           {
            ftotal = (double) ltotal - ValueToDouble(theArgument.value);
            useFloatTotal = true;
           }
        }
      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   if (useFloatTotal)
     {
      returnValue->type = FLOAT;
      returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
     }
   else
     {
      returnValue->type = INTEGER;
      returnValue->value = (void *) EnvAddLong(theEnv,ltotal);
     }
  }

/***********************************/
/* DivisionFunction:  CLIPS access */
/*   routine for the / function.   */
/***********************************/
void DivisionFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  {
   double ftotal = 1.0;
   long long ltotal = 1LL;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;
   
   /*===================================================*/
   /* Get the first argument. This number which will be */
   /* the starting product from which all subsequent    */
   /* arguments will divide. If the auto float dividend */
   /* feature is enable, then this number is converted  */
   /* to a float if it is an integer.                   */
   /*===================================================*/

   theExpression = GetFirstArgument();
   if (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"/",&theArgument,true,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER)
        { ltotal = ValueToLong(theArgument.value); }
      else
        {
         ftotal = ValueToDouble(theArgument.value);
        }
      pos++;
     }

   /*====================================================*/
   /* Loop through each of the arguments dividing it     */
   /* into a running product. If a floating point number */
   /* is encountered, then do all subsequent operations  */
   /* using floating point values. Each argument is      */
   /* checked to prevent a divide by zero error.         */
   /*====================================================*/

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"/",&theArgument,true,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if ((theArgument.type == INTEGER) ? (ValueToLong(theArgument.value) == 0L) :
                                 ((theArgument.type == FLOAT) ? ValueToDouble(theArgument.value) == 0.0 : false))
        {
         DivideByZeroErrorMessage(theEnv,"/");
         EnvSetEvaluationError(theEnv,true);
         returnValue->type = FLOAT;
         returnValue->value = (void *) EnvAddDouble(theEnv,1.0);
         return;
        }

      ftotal /= ValueToDouble(theArgument.value);
      pos++;
     }

   /*======================================================*/
   /* If a floating point number was in the argument list, */
   /* then return a float, otherwise return an integer.    */
   /*======================================================*/

   returnValue->type = FLOAT;
   returnValue->value = (void *) EnvAddDouble(theEnv,ftotal);
  }

/*************************************/
/* DivFunction: H/L access routine   */
/*   for the div function.           */
/*************************************/
long long DivFunction(
  void *theEnv)
  {
   long long total = 1LL;
   EXPRESSION *theExpression;
   DATA_OBJECT theArgument;
   int pos = 1;
   long long theNumber;

   /*===================================================*/
   /* Get the first argument. This number which will be */
   /* the starting product from which all subsequent    */
   /* arguments will divide.                            */
   /*===================================================*/

   theExpression = GetFirstArgument();
   if (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"div",&theArgument,false,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER)
        { total = ValueToLong(theArgument.value); }
      else
        { total = (long long) ValueToDouble(theArgument.value); }
      pos++;
     }

   /*=====================================================*/
   /* Loop through each of the arguments dividing it into */
   /* a running product. Floats are converted to integers */
   /* and each argument is checked to prevent a divide by */
   /* zero error.                                         */
   /*=====================================================*/

   while (theExpression != NULL)
     {
      if (! GetNumericArgument(theEnv,theExpression,"div",&theArgument,false,pos)) theExpression = NULL;
      else theExpression = GetNextArgument(theExpression);

      if (theArgument.type == INTEGER) theNumber = ValueToLong(theArgument.value);
      else if (theArgument.type == FLOAT) theNumber = (long long) ValueToDouble(theArgument.value);
      else theNumber = 1;

      if (theNumber == 0LL)
        {
         DivideByZeroErrorMessage(theEnv,"div");
         EnvSetEvaluationError(theEnv,true);
         return(1L);
        }

      if (theArgument.type == INTEGER)
        { total /= ValueToLong(theArgument.value); }
      else
        { total = total / (long long) ValueToDouble(theArgument.value); }

      pos++;
     }

   /*======================================================*/
   /* The result of the div function is always an integer. */
   /*======================================================*/

   return(total);
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

   if (! UDFArgTypeCheck(context,1,NUMBER_TYPES,returnValue))
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

   if (! UDFArgTypeCheck(context,1,NUMBER_TYPES,returnValue))
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

   if (! UDFArgTypeCheck(context,1,NUMBER_TYPES,returnValue))
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

   if (! UDFGetFirstArgument(context,NUMBER_TYPES,returnValue))
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

   while (UDFHasNextArg(context))
     {
      if (! UDFGetNextArgument(context,NUMBER_TYPES,&nextPossible))
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

   if (! UDFGetFirstArgument(context,NUMBER_TYPES,returnValue))
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

   while (UDFHasNextArg(context))
     {
      if (! UDFGetNextArgument(context,NUMBER_TYPES,&nextPossible))
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

