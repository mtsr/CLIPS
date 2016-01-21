   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  01/13/16             */
   /*                                                     */
   /*               EXTERNAL FUNCTION MODULE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for adding new user or system defined   */
/*   functions.                                              */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Corrected code to remove run-time program      */
/*            compiler warning.                              */
/*                                                           */
/*      6.30: Added support for passing context information  */ 
/*            to user defined functions.                     */
/*                                                           */
/*            Support for long long integers.                */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Changed restrictions from char * to            */
/*            symbolHashNode * to support strings            */
/*            originating from sources that are not          */
/*            statically allocated.                          */
/*                                                           */
/*            Callbacks must be environment aware.           */
/*                                                           */
/*************************************************************/

#include "setup.h"

#include <ctype.h>
#include <stdlib.h>

#include "argacces.h"
#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"

#include "extnfunc.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    AddHashFunction(void *,struct FunctionDefinition *);
   static void                    InitializeFunctionHashTable(void *);
   static void                    DeallocateExternalFunctionData(void *);
#if (! RUN_TIME)
   static bool                    RemoveHashFunction(void *,struct FunctionDefinition *);
#endif
   static void                    PrintType(void *,const char *,int,int *,const char *);

/*********************************************************/
/* InitializeExternalFunctionData: Allocates environment */
/*    data for external functions.                       */
/*********************************************************/
void InitializeExternalFunctionData(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,EXTERNAL_FUNCTION_DATA,sizeof(struct externalFunctionData),DeallocateExternalFunctionData);
  }

/***********************************************************/
/* DeallocateExternalFunctionData: Deallocates environment */
/*    data for external functions.                         */
/***********************************************************/
static void DeallocateExternalFunctionData(
  void *theEnv)
  {
   struct FunctionHash *fhPtr, *nextFHPtr;
   int i;

#if ! RUN_TIME
   struct FunctionDefinition *tmpPtr, *nextPtr;

   tmpPtr = ExternalFunctionData(theEnv)->ListOfFunctions;
   while (tmpPtr != NULL)
     {
      nextPtr = tmpPtr->next;
      rtn_struct(theEnv,FunctionDefinition,tmpPtr);
      tmpPtr = nextPtr;
     }
#endif

   if (ExternalFunctionData(theEnv)->FunctionHashtable == NULL)
     { return; }
     
   for (i = 0; i < SIZE_FUNCTION_HASH; i++)
     {
      fhPtr = ExternalFunctionData(theEnv)->FunctionHashtable[i];
      while (fhPtr != NULL)
        {
         nextFHPtr = fhPtr->next;
         rtn_struct(theEnv,FunctionHash,fhPtr);
         fhPtr = nextFHPtr;
        }
     }
   
   genfree(theEnv,ExternalFunctionData(theEnv)->FunctionHashtable,
           (int) sizeof (struct FunctionHash *) * SIZE_FUNCTION_HASH);
  }

#if (! RUN_TIME)

/******************************************************/
/* EnvDefineFunction: Used to define a system or user */
/*   external function so that the KB can access it.  */
/******************************************************/
bool EnvDefineFunction(
  void *theEnv,
  const char *name,
  int returnType,
  int (*pointer)(void *),
  const char *actualName)
  {
   return(DefineFunction3(theEnv,name,returnType,0,pointer,actualName,UNBOUNDED,UNBOUNDED,NULL,NULL));
  }
  
/************************************************************/
/* EnvDefineFunctionWithContext: Used to define a system or */
/*   user external function so that the KB can access it.   */
/************************************************************/
bool EnvDefineFunctionWithContext(
  void *theEnv,
  const char *name,
  int returnType,
  int (*pointer)(void *),
  const char *actualName,
  void *context)
  {
   return(DefineFunction3(theEnv,name,returnType,0,pointer,actualName,UNBOUNDED,UNBOUNDED,NULL,context));
  }
  
/*******************************************************/
/* EnvDefineFunction2: Used to define a system or user */
/*   external function so that the KB can access it.   */
/*******************************************************/
bool EnvDefineFunction2(
  void *theEnv,
  const char *name,
  int returnType,
  int (*pointer)(void *),
  const char *actualName,
  const char *restrictions)
  {
   return(DefineFunction3(theEnv,name,returnType,0,pointer,actualName,UNBOUNDED,UNBOUNDED,restrictions,NULL));
  }

/*************************************************************/
/* EnvDefineFunction2WithContext: Used to define a system or */
/*   user external function so that the KB can access it.    */
/*************************************************************/
bool EnvDefineFunction2WithContext(
  void *theEnv,
  const char *name,
  int returnType,
  int (*pointer)(void *),
  const char *actualName,
  const char *restrictions,
  void *context)
  {
   return(DefineFunction3(theEnv,name,returnType,0,pointer,actualName,UNBOUNDED,UNBOUNDED,restrictions,context));
  }

/*******************************************************/
/* EnvAddUDF: Used to define a system or user external */
/*   function so that the KB can access it.            */
/*******************************************************/
bool EnvAddUDF(
  void *theEnv,
  const char *name,
  unsigned returnTypeBits,
  void (*pointer)(UDFContext *,struct dataObject *),
  const char *actualName,
  int minArgs,
  int maxArgs,
  const char *restrictions,
  void *context)
  {
   return(DefineFunction3(theEnv,name,'z',returnTypeBits,PTIEF pointer,actualName,minArgs,maxArgs,restrictions,context));
  }

/*************************************************************/
/* DefineFunction3: Used to define a system or user external */
/*   function so that the KB can access it. Allows argument  */
/*   restrictions to be attached to the function.            */
/*   Return types are:                                       */
/*     a - external address                                  */
/*     b - boolean integer (converted to symbol)             */
/*     c - character (converted to symbol)                   */
/*     d - double precision float                            */
/*     f - single precision float (converted to double)      */
/*     g - long long integer                                 */
/*     i - integer (converted to long long integer)          */
/*     j - unknown (symbol, string,                          */
/*                  or instance name by convention)          */
/*     k - unknown (symbol or string by convention)          */
/*     l - long integer (converted to long long integer)     */
/*     m - unknown (multifield by convention)                */
/*     n - unknown (integer or float by convention)          */
/*     o - instance name                                     */
/*     s - string                                            */
/*     u - unknown                                           */
/*     v - void                                              */
/*     w - symbol                                            */
/*     x - instance address                                  */
/*     z - unknown (with return type bits)                   */
/*************************************************************/
bool DefineFunction3(
  void *theEnv,
  const char *name,
  int returnType,
  unsigned returnTypeBits,
  int (*pointer)(void *), // TBD New UDF Cast void (*pointer)(void *,DATA_OBJECT_PTR)
  const char *actualName,
  int minArgs,
  int maxArgs,
  const char *restrictions,
  void *context)
  {
   struct FunctionDefinition *newFunction;

   if ( (returnType != 'a') &&
        (returnType != 'b') &&
        (returnType != 'c') &&
        (returnType != 'd') &&
        (returnType != 'f') &&
        (returnType != 'g') &&
        (returnType != 'i') &&
        (returnType != 'j') &&
        (returnType != 'k') &&
        (returnType != 'l') &&
        (returnType != 'm') &&
        (returnType != 'n') &&
#if OBJECT_SYSTEM
        (returnType != 'o') &&
#endif
        (returnType != 's') &&
        (returnType != 'u') &&
        (returnType != 'v') &&
#if OBJECT_SYSTEM
        (returnType != 'x') &&
#endif
#if DEFTEMPLATE_CONSTRUCT
        (returnType != 'y') &&
#endif
        (returnType != 'w') &&
       
        (returnType != 'z'))
     { return(false); }

   newFunction = FindFunction(theEnv,name);
   if (newFunction != NULL) return(0);
   
   newFunction = get_struct(theEnv,FunctionDefinition);
   newFunction->callFunctionName = (SYMBOL_HN *) EnvAddSymbol(theEnv,name);
   IncrementSymbolCount(newFunction->callFunctionName);
   newFunction->next = GetFunctionList(theEnv);
   ExternalFunctionData(theEnv)->ListOfFunctions = newFunction;
   AddHashFunction(theEnv,newFunction);
     
   newFunction->returnValueType = (char) returnType;
   newFunction->unknownReturnValueType = returnTypeBits;
   newFunction->functionPointer = (int (*)(void)) pointer;
   newFunction->actualFunctionName = actualName;
   
   newFunction->minArgs = minArgs;
   newFunction->maxArgs = maxArgs;
   
   if ((restrictions != NULL) && (returnType != 'z'))
     {
      if (((int) (strlen(restrictions)) < 2) ? true :
          ((! isdigit(restrictions[0]) && (restrictions[0] != '*')) ||
           (! isdigit(restrictions[1]) && (restrictions[1] != '*'))))
        restrictions = NULL;
     }
     
   if (restrictions == NULL)
     { newFunction->restrictions = NULL; }
   else
     {
      newFunction->restrictions = EnvAddSymbol(theEnv,restrictions);
      IncrementSymbolCount(newFunction->restrictions);
     }
     
   newFunction->parser = NULL;
   newFunction->overloadable = true;
   newFunction->sequenceuseok = true;
   newFunction->usrData = NULL;
   newFunction->context = context;

   return(true);
  }
  
/***********************************************/
/* UndefineFunction: Used to remove a function */
/*   definition from the list of functions.    */
/***********************************************/
bool UndefineFunction(
  void *theEnv,
  const char *functionName)
  {
   SYMBOL_HN *findValue;
   struct FunctionDefinition *fPtr, *lastPtr = NULL;

   findValue = (SYMBOL_HN *) FindSymbolHN(theEnv,functionName);

   for (fPtr = ExternalFunctionData(theEnv)->ListOfFunctions;
        fPtr != NULL;
        fPtr = fPtr->next)
     {
      if (fPtr->callFunctionName == findValue)
        {
         DecrementSymbolCount(theEnv,fPtr->callFunctionName);
         RemoveHashFunction(theEnv,fPtr);

         if (lastPtr == NULL)
           { ExternalFunctionData(theEnv)->ListOfFunctions = fPtr->next; }
         else
           { lastPtr->next = fPtr->next; }
           
         if (fPtr->restrictions != NULL)
           { DecrementSymbolCount(theEnv,fPtr->restrictions); }
         ClearUserDataList(theEnv,fPtr->usrData);
         rtn_struct(theEnv,FunctionDefinition,fPtr);
         return(true);
        }

      lastPtr = fPtr;
     }

   return(false);
  }

/******************************************/
/* RemoveHashFunction: Removes a function */
/*   from the function hash table.        */
/******************************************/
static bool RemoveHashFunction(
  void *theEnv,
  struct FunctionDefinition *fdPtr)
  {
   struct FunctionHash *fhPtr, *lastPtr = NULL;
   unsigned hashValue;

   hashValue = HashSymbol(ValueToString(fdPtr->callFunctionName),SIZE_FUNCTION_HASH);

   for (fhPtr = ExternalFunctionData(theEnv)->FunctionHashtable[hashValue];
        fhPtr != NULL;
        fhPtr = fhPtr->next)
     {
      if (fhPtr->fdPtr == fdPtr)
        {
         if (lastPtr == NULL)
           { ExternalFunctionData(theEnv)->FunctionHashtable[hashValue] = fhPtr->next; }
         else
           { lastPtr->next = fhPtr->next; }

         rtn_struct(theEnv,FunctionHash,fhPtr);
         return(true);
        }

      lastPtr = fhPtr;
     }

   return(false);
  }

/***************************************************************************/
/* AddFunctionParser: Associates a specialized expression parsing function */
/*   with the function entry for a function which was defined using        */
/*   DefineFunction. When this function is parsed, the specialized parsing */
/*   function will be called to parse the arguments of the function. Only  */
/*   user and system defined functions can have specialized parsing        */
/*   routines. Generic functions and deffunctions can not have specialized */
/*   parsing routines.                                                     */
/***************************************************************************/
int AddFunctionParser(
  void *theEnv,
  const char *functionName,
  struct expr *(*fpPtr)(void *,struct expr *,const char *))
  {
   struct FunctionDefinition *fdPtr;

   fdPtr = FindFunction(theEnv,functionName);
   if (fdPtr == NULL)
     {
      EnvPrintRouter(theEnv,WERROR,"Function parsers can only be added for existing functions.\n");
      return(0);
     }
   fdPtr->restrictions = NULL;
   fdPtr->parser = fpPtr;
   fdPtr->overloadable = false;

   return(1);
  }

/*********************************************************************/
/* RemoveFunctionParser: Removes a specialized expression parsing    */
/*   function (if it exists) from the function entry for a function. */
/*********************************************************************/
int RemoveFunctionParser(
  void *theEnv,
  const char *functionName)
  {
   struct FunctionDefinition *fdPtr;

   fdPtr = FindFunction(theEnv,functionName);
   if (fdPtr == NULL)
     {
      EnvPrintRouter(theEnv,WERROR,"Function parsers can only be removed from existing functions.\n");
      return(0);
     }

   fdPtr->parser = NULL;

   return(1);
  }

/*****************************************************************/
/* FuncSeqOvlFlags: Makes a system function overloadable or not, */
/* i.e. can the function be a method for a generic function.     */
/*****************************************************************/
bool FuncSeqOvlFlags(
  void *theEnv,
  const char *functionName,
  bool seqp,
  bool ovlp)
  {
   struct FunctionDefinition *fdPtr;

   fdPtr = FindFunction(theEnv,functionName);
   if (fdPtr == NULL)
     {
      EnvPrintRouter(theEnv,WERROR,"Only existing functions can be marked as using sequence expansion arguments/overloadable or not.\n");
      return(false);
     }
   fdPtr->sequenceuseok = (short) (seqp ? true : false);
   fdPtr->overloadable = (short) (ovlp ? true : false);
   return(true);
  }

#endif

/*********************************************************/
/* GetArgumentTypeName: Returns a descriptive string for */
/*   a function argument type (used by DefineFunction2). */
/*********************************************************/
const char *GetArgumentTypeName(
  int theRestriction)
  {
   switch ((char) theRestriction)
     {
      case 'a':
        return("external address");

      case 'e':
        return("instance address, instance name, or symbol");

      case 'd':
      case 'f':
        return("float");

      case 'g':
        return("integer, float, or symbol");

      case 'h':
        return("instance address, instance name, fact address, integer, or symbol");

      case 'j':
        return("symbol, string, or instance name");

      case 'k':
        return("symbol or string");

      case 'i':
      case 'l':
        return("integer");

      case 'm':
        return("multifield");

      case 'n':
        return("integer or float");

      case 'o':
        return("instance name");

      case 'p':
        return("instance name or symbol");

      case 'q':
        return("multifield, symbol, or string");

      case 's':
        return("string");

      case 'w':
        return("symbol");

      case 'x':
        return("instance address");

      case 'y':
        return("fact-address");

      case 'z':
        return("fact-address, integer, or symbol");

      case 'u':
        return("non-void return value");
     }

   return("unknown argument type");
  }

/***************************************************/
/* GetNthRestriction: Returns the restriction type */
/*   for the nth parameter of a function.          */
/***************************************************/
int GetNthRestriction(
  struct FunctionDefinition *theFunction,
  int position)
  {
   int defaultRestriction = (int) 'u';
   size_t theLength;
   int i = 2;

   /*===========================================================*/
   /* If no restrictions at all are specified for the function, */
   /* then return 'u' to indicate that any value is suitable as */
   /* an argument to the function.                              */
   /*===========================================================*/

   if (theFunction == NULL) return(defaultRestriction);

   if (theFunction->restrictions == NULL) return(defaultRestriction);

   /*===========================================================*/
   /* If no type restrictions are specified for the function,   */
   /* then return 'u' to indicate that any value is suitable as */
   /* an argument to the function.                              */
   /*===========================================================*/

   theLength = strlen(theFunction->restrictions->contents);

   if (theLength < 3) return(defaultRestriction);

   /*==============================================*/
   /* Determine the functions default restriction. */
   /*==============================================*/

   defaultRestriction = (int) theFunction->restrictions->contents[i];

   if (defaultRestriction == '*') defaultRestriction = (int) 'u';

   /*=======================================================*/
   /* If the requested position does not have a restriction */
   /* specified, then return the default restriction.       */
   /*=======================================================*/

   if (theLength < (size_t) (position + 3)) return(defaultRestriction);

   /*=========================================================*/
   /* Return the restriction specified for the nth parameter. */
   /*=========================================================*/

   return((int) theFunction->restrictions->contents[position + 2]);
  }

/***************************************************/
/* GetNthRestriction2: Returns the restriction type */
/*   for the nth parameter of a function.          */
/***************************************************/
unsigned GetNthRestriction2(
  struct FunctionDefinition *theFunction,
  int position)
  {
   unsigned rv, df;
   const char *restrictions;
   
   if (theFunction == NULL) return(ANY_TYPE);

   if (theFunction->restrictions == NULL) return(ANY_TYPE);
   restrictions = theFunction->restrictions->contents;
   
   PopulateRestriction(&df,ANY_TYPE,restrictions,0);
   PopulateRestriction(&rv,df,restrictions,position);

   return rv;
  }

/*************************************************/
/* GetFunctionList: Returns the ListOfFunctions. */
/*************************************************/
struct FunctionDefinition *GetFunctionList(
  void *theEnv)
  {
   return(ExternalFunctionData(theEnv)->ListOfFunctions);
  }

/**************************************************************/
/* InstallFunctionList: Sets the ListOfFunctions and adds all */
/*   the function entries to the FunctionHashTable.           */
/**************************************************************/
void InstallFunctionList(
  void *theEnv,
  struct FunctionDefinition *value)
  {
   int i;
   struct FunctionHash *fhPtr, *nextPtr;

   if (ExternalFunctionData(theEnv)->FunctionHashtable != NULL)
     {
      for (i = 0; i < SIZE_FUNCTION_HASH; i++)
        {
         fhPtr = ExternalFunctionData(theEnv)->FunctionHashtable[i];
         while (fhPtr != NULL)
           {
            nextPtr = fhPtr->next;
            rtn_struct(theEnv,FunctionHash,fhPtr);
            fhPtr = nextPtr;
           }
         ExternalFunctionData(theEnv)->FunctionHashtable[i] = NULL;
        }
     }

   ExternalFunctionData(theEnv)->ListOfFunctions = value;

   while (value != NULL)
     {
      AddHashFunction(theEnv,value);
      value = value->next;
     }
  }

/********************************************************/
/* FindFunction: Returns a pointer to the corresponding */
/*   FunctionDefinition structure if a function name is */
/*   in the function list, otherwise returns NULL.      */
/********************************************************/
struct FunctionDefinition *FindFunction(
  void *theEnv,
  const char *functionName)
  {
   struct FunctionHash *fhPtr;
   unsigned hashValue;
   SYMBOL_HN *findValue;

   if (ExternalFunctionData(theEnv)->FunctionHashtable == NULL) return(NULL);
   
   hashValue = HashSymbol(functionName,SIZE_FUNCTION_HASH);

   findValue = (SYMBOL_HN *) FindSymbolHN(theEnv,functionName);

   for (fhPtr = ExternalFunctionData(theEnv)->FunctionHashtable[hashValue];
        fhPtr != NULL;
        fhPtr = fhPtr->next)
     {
      if (fhPtr->fdPtr->callFunctionName == findValue)
        { return(fhPtr->fdPtr); }
     }

   return(NULL);
  }

/*********************************************************/
/* InitializeFunctionHashTable: Purpose is to initialize */
/*   the function hash table to NULL.                    */
/*********************************************************/
static void InitializeFunctionHashTable(
  void *theEnv)
  {
   int i;

   ExternalFunctionData(theEnv)->FunctionHashtable = (struct FunctionHash **)
                       gm2(theEnv,(int) sizeof (struct FunctionHash *) *
                           SIZE_FUNCTION_HASH);

   for (i = 0; i < SIZE_FUNCTION_HASH; i++) ExternalFunctionData(theEnv)->FunctionHashtable[i] = NULL;
  }

/****************************************************************/
/* AddHashFunction: Adds a function to the function hash table. */
/****************************************************************/
static void AddHashFunction(
  void *theEnv,
  struct FunctionDefinition *fdPtr)
  {
   struct FunctionHash *newhash, *temp;
   unsigned hashValue;

   if (ExternalFunctionData(theEnv)->FunctionHashtable == NULL) InitializeFunctionHashTable(theEnv);

   newhash = get_struct(theEnv,FunctionHash);
   newhash->fdPtr = fdPtr;

   hashValue = HashSymbol(fdPtr->callFunctionName->contents,SIZE_FUNCTION_HASH);

   temp = ExternalFunctionData(theEnv)->FunctionHashtable[hashValue];
   ExternalFunctionData(theEnv)->FunctionHashtable[hashValue] = newhash;
   newhash->next = temp;
  }

/*************************************************/
/* GetMinimumArgs: Returns the minimum number of */
/*   arguments expected by an external function. */
/*************************************************/
int GetMinimumArgs(
  struct FunctionDefinition *theFunction)
  {
   char theChar[2];
   const char *restrictions;

   if (theFunction->restrictions == NULL) return(-1);
   restrictions = theFunction->restrictions->contents;

   theChar[0] = restrictions[0];
   theChar[1] = '\0';

   if (isdigit(theChar[0]))
     { return atoi(theChar); }
   else if (theChar[0] == '*')
     { return(-1); }
   
   return(-1); 
  }
  
/*************************************************/
/* GetMaximumArgs: Returns the maximum number of */
/*   arguments expected by an external function. */
/*************************************************/
int GetMaximumArgs(
  struct FunctionDefinition *theFunction)
  {
   char theChar[2];
   const char *restrictions;

   if (theFunction->restrictions == NULL) return(-1);
   restrictions = theFunction->restrictions->contents;
   if (restrictions[0] == '\0') return(-1);

   theChar[0] = restrictions[1];
   theChar[1] = '\0';

   if (isdigit(theChar[0]))
     { return atoi(theChar); }
   else if (theChar[0] == '*')
     { return(-1); }
   
   return(-1); 
  }

/*********************/
/* UDFArgCountCheck: */
/*********************/
int UDFArgCountCheck(
  UDFContext *context) // TBD can this be removed with static constraint checking always enabled?
  {
   struct FunctionDefinition *theFunction = context->theFunction;
   const char *functionName = theFunction->callFunctionName->contents;
   void *theEnv = UDFContextEnvironment(context);
      
   if ((theFunction->minArgs == UNBOUNDED) && (theFunction->maxArgs == UNBOUNDED))
     { return EnvRtnArgCount(theEnv); }
     
   if ((theFunction->minArgs != UNBOUNDED) && (theFunction->maxArgs == UNBOUNDED))
     { return EnvArgCountCheck(theEnv,functionName,AT_LEAST,theFunction->minArgs); }

   if ((theFunction->minArgs == UNBOUNDED) && (theFunction->maxArgs != UNBOUNDED))
     { return EnvArgCountCheck(theEnv,functionName,NO_MORE_THAN,theFunction->maxArgs); }

   if (theFunction->minArgs == theFunction->maxArgs)
     { return EnvArgCountCheck(theEnv,functionName,EXACTLY,theFunction->maxArgs); }

   if (theFunction->minArgs < theFunction->maxArgs)
     { return EnvArgCountCheck(theEnv,functionName,EXACTLY,theFunction->maxArgs); }
    
   return -1;
  }
  
/********************/
/* UDFArgTypeCheck: */
/********************/
bool UDFArgTypeCheck(
  UDFContext *context,
  int argumentPosition,
  unsigned expectedType,
  DATA_OBJECT_PTR returnValue)
  {
   void *theEnv = UDFContextEnvironment(context);
   
   EnvRtnUnknown(theEnv,argumentPosition,returnValue);
   if (EvaluationData(theEnv)->EvaluationError) return false;
   
   switch (returnValue->type)
     {
      case RVOID:
        if (expectedType & VOID_TYPE) return(true);
        break;

      case INTEGER:
        if (expectedType & INTEGER_TYPE) return(true);
        break;

      case FLOAT:
        if (expectedType & FLOAT_TYPE) return(true);
        break;

      case SYMBOL:
        if (expectedType & SYMBOL_TYPE) return(true);
        break;

      case STRING:
        if (expectedType & STRING_TYPE) return(true);
        break;

      case INSTANCE_NAME:
        if (expectedType & INSTANCE_NAME_TYPE) return(true);
        break;

      case EXTERNAL_ADDRESS:
        if (expectedType & EXTERNAL_ADDRESS_TYPE) return(true);
        break;

      case FACT_ADDRESS:
        if (expectedType & FACT_ADDRESS_TYPE) return(true);
        break;

      case INSTANCE_ADDRESS:
        if (expectedType & INSTANCE_ADDRESS_TYPE) return(true);
        break;
        
      case MULTIFIELD:
        if (expectedType & MULTIFIELD_TYPE) return(true);
        break;
     }

   ExpectedTypeError0(theEnv,UDFContextFunctionName(context),argumentPosition);
   PrintTypesString(theEnv,WERROR,expectedType,true);

   EnvSetHaltExecution(theEnv,true);
   EnvSetEvaluationError(theEnv,true);

   return false;
  }

/******************************/
/* UDFInvalidArgumentMessage: */
/******************************/
void UDFInvalidArgumentMessage(
  UDFContext *context,
  int argumentPosition,
  const char *typeString)
  {
   ExpectedTypeError1(UDFContextEnvironment(context),UDFContextFunctionName(context),argumentPosition,typeString);
  }

/**************************/
/* UDFContextEnvironment: */
/**************************/
void *UDFContextEnvironment(
  UDFContext *context)
  {
   return context->environment;
  }

/***************************/
/* UDFContextFunctionName: */
/***************************/
const char *UDFContextFunctionName(
  UDFContext *context)
  {
   return context->theFunction->callFunctionName->contents;
  }

/**************************/
/* UDFContextUserContext: */
/**************************/
void *UDFContextUserContext(
  UDFContext *context)
  {
   return context->theFunction->context;
  }

/**************/
/* PrintType: */
/**************/
static void PrintType(
  void *theEnv,
  const char *logicalName,
  int typeCount,
  int *typesPrinted,
  const char *typeName)
  {
   if (*typesPrinted == 0)
     {
      EnvPrintRouter(theEnv,logicalName,typeName);
      (*typesPrinted)++;
      return;
     }

   if (typeCount == 2)
     { EnvPrintRouter(theEnv,logicalName," or "); }
   else if (((*typesPrinted) + 1) == typeCount)
     { EnvPrintRouter(theEnv,logicalName,", or "); }
   else
     { EnvPrintRouter(theEnv,logicalName,", "); }
     
   EnvPrintRouter(theEnv,logicalName,typeName);
   (*typesPrinted)++;
  }

/********************/
/* PrintTypesString */
/********************/
void PrintTypesString(
  void *theEnv,
  const char *logicalName,
  unsigned expectedType,
  bool printCRLF)
  {
   int typeCount, typesPrinted;

   typeCount = 0;
   if (expectedType & INTEGER_TYPE) typeCount++;
   if (expectedType & FLOAT_TYPE) typeCount++;
   if (expectedType & SYMBOL_TYPE) typeCount++;
   if (expectedType & STRING_TYPE) typeCount++;
   if (expectedType & INSTANCE_NAME_TYPE) typeCount++;
   if (expectedType & INSTANCE_ADDRESS_TYPE) typeCount++;
   if (expectedType & FACT_ADDRESS_TYPE) typeCount++;
   if (expectedType & EXTERNAL_ADDRESS_TYPE) typeCount++;
   if (expectedType & MULTIFIELD_TYPE) typeCount++;
   
   typesPrinted = 0;
   if (expectedType & INTEGER_TYPE)
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"integer"); }
  
    if (expectedType & FLOAT_TYPE)
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"float"); }

   if (expectedType & SYMBOL_TYPE)
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"symbol"); }

   if (expectedType & STRING_TYPE)
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"string"); }

   if (expectedType & INSTANCE_NAME_TYPE) 
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"instance name"); }

   if (expectedType & INSTANCE_ADDRESS_TYPE) 
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"instance address"); }

   if (expectedType & FACT_ADDRESS_TYPE) 
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"fact address"); }

   if (expectedType & EXTERNAL_ADDRESS_TYPE) 
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"external address"); }

   if (expectedType & MULTIFIELD_TYPE) 
     { PrintType(theEnv,logicalName,typeCount,&typesPrinted,"multifield"); }
   
   if (printCRLF)
     { EnvPrintRouter(theEnv,logicalName,"\n"); }
  }

