   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  07/30/16             */
   /*                                                     */
   /*                USER FUNCTIONS MODULE                */
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
/*      6.24: Created file to seperate UserFunctions and     */
/*            EnvUserFunctions from main.c.                  */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*************************************************************/

/***************************************************************************/
/*                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, and/or sell copies of the Software, and to permit persons   */
/* to whom the Software is furnished to do so.                             */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT   */
/* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY  */
/* CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES */
/* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN   */
/* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF */
/* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                         */
/***************************************************************************/

#include "clips.h"

#include <math.h>

void EnvUserFunctions(Environment *);

   void                           RTA(Environment *,UDFContext *,CLIPSValue *);
   void                           MUL(Environment *,UDFContext *,CLIPSValue *);
   void                           Positivep(Environment *,UDFContext *,CLIPSValue *);
   void                           Cube(Environment *,UDFContext *,CLIPSValue *);
   void                           TripleNumber(Environment *,UDFContext *,CLIPSValue *);
   void                           Reverse(Environment *,UDFContext *,CLIPSValue *);
   void                           MFL(Environment *,UDFContext *,CLIPSValue *);
   void                           CntMFChars(Environment *,UDFContext *,CLIPSValue *);
   void                           Sample4(Environment *,UDFContext *,CLIPSValue *);
   void                           Rest(Environment *,UDFContext *,CLIPSValue *);

/***********************************************************/
/* EnvUserFunctions: Informs the expert system environment */
/*   of any user defined functions. In the default case,   */
/*   there are no user defined functions. To define        */
/*   functions, either this function must be replaced by   */
/*   a function with the same name within this file, or    */
/*   this function can be deleted from this file and       */
/*   included in another file.                             */
/***********************************************************/
void EnvUserFunctions(
  Environment *environment)
  {
#if MAC_XCD
#pragma unused(environment)
#endif
   EnvAddUDF(environment,"rta","d",2,2,"ld",RTA,"RTA",NULL);
   EnvAddUDF(environment,"mul","l",2,2,"ld",MUL,"MUL",NULL);
   EnvAddUDF(environment,"positivep","b",1,1,"ld",Positivep,"Positivep",NULL);
   EnvAddUDF(environment,"cube","bld",1,1,"ld",Cube,"Cube",NULL);
   EnvAddUDF(environment,"triple","ld",1,1,"ld",TripleNumber,"TripleNumber",NULL);
   EnvAddUDF(environment,"reverse","syn",1,1,"syn",Reverse,"Reverse",NULL);
   EnvAddUDF(environment,"mfl","l",1,1,"m",MFL,"MFL",NULL);
   EnvAddUDF(environment,"cmfc","l",1,1,"m",CntMFChars,"CntMFChars",NULL);
   EnvAddUDF(environment,"sample4","m",0,0,NULL,Sample4,"Sample4",NULL);
   EnvAddUDF(environment,"rest","m",1,1,"m",Rest,"Rest",NULL);
  }

/*******/
/* RTA */
/*******/
void RTA(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue base, height;

   if (! UDFFirstArgument(context,NUMBER_TYPES,&base))
     { return; }

   if (! UDFNextArgument(context,NUMBER_TYPES,&height))
     { return; }

   returnValue->floatValue = EnvCreateFloat(theEnv,0.5 * CVCoerceToFloat(&base) * CVCoerceToFloat(&height));
  }

/*******/
/* MUL */
/*******/
void MUL(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue theArg;
   long long firstNumber, secondNumber;

   /*=============================================================*/
   /* Get the first argument using the UDFFirstArgument function. */
   /* Return if the correct type has not been passed.             */
   /*=============================================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&theArg))
     { return; }

   /*========================================================*/
   /* Convert the first argument to an integer. If it's not  */
   /* an integer, then it must be a float (so round it to    */
   /* the nearest integer using the C library ceil function. */
   /*========================================================*/

   if (CVIsType(&theArg,INTEGER_TYPE))
     { firstNumber = theArg.integerValue->contents; }
   else /* the type must be FLOAT */
     { firstNumber = (long long) ceil(theArg.floatValue->contents - 0.5); }

   /*=============================================================*/
   /* Get the second argument using the UDFNextArgument function. */
   /* Return if the correct type has not been passed.             */
   /*=============================================================*/

   if (! UDFNextArgument(context,NUMBER_TYPES,&theArg))
     { return; }

   /*========================================================*/
   /* Convert the second argument to an integer. If it's not */
   /* an integer, then it must be a float (so round it to    */
   /* the nearest integer using the C library ceil function. */
   /*========================================================*/

   if (CVIsType(&theArg,INTEGER_TYPE))
     { secondNumber = theArg.integerValue->contents; }
   else /* the type must be FLOAT */
     { secondNumber = (long long) ceil(theArg.floatValue->contents - 0.5); }

   /*=========================================================*/
   /* Multiply the two values together and return the result. */
   /*=========================================================*/

   returnValue->integerValue = EnvCreateInteger(theEnv,firstNumber * secondNumber);
  }

/*************/
/* Positivep */
/*************/
void Positivep(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue theArg;

   /*==================================*/
   /* Get the first argument using the */
   /* UDFFirstArgument function.       */
   /*==================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&theArg))
     { return; }

   /*=====================================*/
   /* Determine if the value is positive. */
   /*=====================================*/

   if (CVIsType(&theArg,INTEGER_TYPE))
     {
      if (theArg.integerValue->contents <= 0L)
        { returnValue->lexemeValue = theEnv->FalseSymbol; }
      else
        { returnValue->lexemeValue = theEnv->TrueSymbol; }
     }
   else /* the type must be FLOAT */
     {
      if (theArg.floatValue->contents <= 0.0)
        { returnValue->lexemeValue = theEnv->FalseSymbol; }
      else
        { returnValue->lexemeValue = theEnv->TrueSymbol; }
     }
  }

/********/
/* Cube */
/********/
void Cube(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue theArg;
   long long theInteger;
   double theFloat;

   /*==================================*/
   /* Get the first argument using the */
   /* UDFFirstArgument function.       */
   /*==================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&theArg))
     {
      returnValue->lexemeValue = theEnv->FalseSymbol;
      return;
     }

   /*====================*/
   /* Cube the argument. */
   /*====================*/

   if (CVIsType(&theArg,INTEGER_TYPE))
     {
      theInteger = theArg.integerValue->contents;
      returnValue->integerValue = EnvCreateInteger(theEnv,theInteger * theInteger * theInteger);
     }
   else /* the type must be FLOAT */
     {
      theFloat = theArg.floatValue->contents;
      returnValue->floatValue = EnvCreateFloat(theEnv,theFloat * theFloat * theFloat);
     }
  }

/****************/
/* TripleNumber */
/****************/
void TripleNumber(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue theArg;

   /*==================================*/
   /* Get the first argument using the */
   /* UDFFirstArgument function.       */
   /*==================================*/

   if (! UDFFirstArgument(context,NUMBER_TYPES,&theArg))
     { return; }

   /*======================*/
   /* Triple the argument. */
   /*======================*/

   if (CVIsType(&theArg,INTEGER_TYPE))
     { returnValue->integerValue = EnvCreateInteger(theEnv,3 * theArg.integerValue->contents); }
   else /* the type must be FLOAT */
     { returnValue->floatValue = EnvCreateFloat(theEnv,3.0 * theArg.floatValue->contents); }
  }

/***********/
/* Reverse */
/***********/
void Reverse(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue theArg;
   const char *theString;
   char *tempString;
   size_t length, i;

   /*=========================================================*/
   /* Get the first argument using the ArgTypeCheck function. */
   /*=========================================================*/

   if (! UDFFirstArgument(context,LEXEME_TYPES | INSTANCE_NAME_TYPE,&theArg))
     { return; }

   theString = theArg.lexemeValue->contents;

   /*========================================================*/
   /* Allocate temporary space to store the reversed string. */
   /*========================================================*/

   length = strlen(theString);
   tempString = (char *) malloc(length + 1);

   /*=====================*/
   /* Reverse the string. */
   /*=====================*/

   for (i = 0; i < length; i++)
     { tempString[length - (i + 1)] = theString[i]; }
   tempString[length] = '\0';

   /*==========================================*/
   /* Set the return value before deallocating */
   /* the temporary reversed string.           */
   /*==========================================*/

   switch(theArg.header->type)
     {
      case STRING:
        returnValue->lexemeValue = EnvCreateString(theEnv,tempString);
        break;

      case SYMBOL:
        returnValue->lexemeValue = EnvCreateSymbol(theEnv,tempString);
        break;

      case INSTANCE_NAME:
        returnValue->lexemeValue = EnvCreateInstanceName(theEnv,tempString);
        break;
     }

   free(tempString);
  }

/*******/
/* MFL */
/*******/
void MFL(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue theArg;

   /*======================================================*/
   /* Check that the first argument is a multifield value. */
   /*======================================================*/

   if (! UDFFirstArgument(context,MULTIFIELD_TYPE,&theArg))
     {
      returnValue->integerValue = EnvCreateInteger(theEnv,-1);
      return;
     }

   /*============================================*/
   /* Return the length of the multifield value. */
   /*============================================*/

   returnValue->integerValue = EnvCreateInteger(theEnv,MFLength(&theArg));
  }

/**************/
/* CntMFChars */
/**************/
void CntMFChars(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   CLIPSValue theArg;
   long long i, count = 0;
   long long mfLength;

   /*======================================================*/
   /* Check that the first argument is a multifield value. */
   /*======================================================*/

   if (! UDFFirstArgument(context,MULTIFIELD_TYPE,&theArg))
     { return; }

   /*=====================================*/
   /* Count the characters in each field. */
   /*=====================================*/

   mfLength = MFLength(&theArg);
   for (i = 0; i < mfLength; i++)
     {
      if (ValueIsType(theArg.multifieldValue->theFields[i].value,LEXEME_TYPES))
        { count += strlen(theArg.multifieldValue->theFields[i].lexemeValue->contents); }
     }

   /*=============================*/
   /* Return the character count. */
   /*=============================*/

   returnValue->integerValue = EnvCreateInteger(theEnv,count);
  }

/***********/
/* Sample4 */
/***********/
void Sample4(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   /*=======================================*/
   /* Create a multifield value of length 2 */
   /*=======================================*/

   returnValue->multifieldValue = EnvCreateMultifield(theEnv,2);
   returnValue->begin = 0;
   returnValue->end = 1;

   /*============================================*/
   /* The first field in the multi-field value   */
   /* will be a SYMBOL. Its value will be        */
   /* "altitude".                                */
   /*============================================*/

   returnValue->multifieldValue->theFields[0].lexemeValue = EnvCreateSymbol(theEnv,"altitude");

   /*===========================================*/
   /* The second field in the multi-field value */
   /* will be a FLOAT. Its value will be 900.   */
   /*===========================================*/

   returnValue->multifieldValue->theFields[1].floatValue = EnvCreateFloat(theEnv,900.00);
  }

/********/
/* Rest */
/********/
void Rest(
  Environment *theEnv,
  UDFContext *context,
  CLIPSValue *returnValue)
  {
   long long mfLength;

   /*======================================================*/
   /* Check that the first argument is a multifield value. */
   /*======================================================*/

   if (! UDFFirstArgument(context,MULTIFIELD_TYPE,returnValue))
     { return; }

   /*===========================================*/
   /* Set the range to exclude the first value. */
   /*===========================================*/

   mfLength = MFLength(returnValue);
   if (mfLength != 0)
     {
      returnValue->begin = 1;
      returnValue->end = mfLength - 1;
     }
  }

