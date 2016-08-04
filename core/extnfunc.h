   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.50  07/30/16            */
   /*                                                     */
   /*            EXTERNAL FUNCTIONS HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for adding new user or system defined   */
/*   functions.                                              */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Added support for passing context information  */ 
/*            to user defined functions.                     */
/*                                                           */
/*            Support for long long integers.                */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Replaced ALLOW_ENVIRONMENT_GLOBALS macros      */
/*            with functions.                                */
/*                                                           */
/*      6.40: Changed restrictions from char * to            */
/*            symbolHashNode * to support strings            */
/*            originating from sources that are not          */
/*            statically allocated.                          */
/*                                                           */
/*            Removed LOCALE definition.                     */
/*                                                           */
/*            Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*      6.50: Callbacks must be environment aware.           */
/*                                                           */
/*************************************************************/

#ifndef _H_extnfunc

#pragma once

#define _H_extnfunc

struct FunctionDefinition;
struct UDFContext_t;
typedef struct UDFContext_t UDFContext;

#include "evaluatn.h"
#include "expressn.h"
#include "symbol.h"
#include "userdata.h"

struct FunctionDefinition
  {
   struct symbolHashNode *callFunctionName;
   const char *actualFunctionName;
   char returnValueType;
   unsigned unknownReturnValueType;
   //int (*functionPointer)(void);
   void (*functionPointer)(UDFContext *,CLIPSValue *);
   struct expr *(*parser)(Environment *,struct expr *,const char *);
   struct symbolHashNode *restrictions;
   int minArgs;
   int maxArgs;
   bool overloadable; // TBD Use unsigned ints here
   bool sequenceuseok;
   short int bsaveIndex;
   struct FunctionDefinition *next;
   struct userData *usrData;
   void *context;
  };

#define ValueFunctionType(target) (((struct FunctionDefinition *) target)->returnValueType)
#define UnknownFunctionType(target) (((struct FunctionDefinition *) target)->unknownReturnValueType)
#define ExpressionFunctionType(target) (((struct FunctionDefinition *) ((target)->value))->returnValueType)
#define ExpressionFunctionPointer(target) (((struct FunctionDefinition *) ((target)->value))->functionPointer)
#define ExpressionFunctionCallName(target) (((struct FunctionDefinition *) ((target)->value))->callFunctionName)
#define ExpressionFunctionRealName(target) (((struct FunctionDefinition *) ((target)->value))->actualFunctionName)
#define ExpressionUnknownFunctionType(target) (((struct FunctionDefinition *) ((target)->value))->unknownReturnValueType)

#define PTIF (int (*)(void))
#define PTIEF (int (*)(Environment *))

/*==================*/
/* ENVIRONMENT DATA */
/*==================*/

#define EXTERNAL_FUNCTION_DATA 50

struct externalFunctionData
  {
   struct FunctionDefinition *ListOfFunctions;
   struct FunctionHash **FunctionHashtable;
  };

struct UDFContext_t
  {
   Environment *environment;
   struct FunctionDefinition *theFunction;
   int lastPosition;
   struct expr *lastArg;
   CLIPSValue *returnValue;
  };

#define ExternalFunctionData(theEnv) ((struct externalFunctionData *) GetEnvironmentData(theEnv,EXTERNAL_FUNCTION_DATA))

struct FunctionHash
  {
   struct FunctionDefinition *fdPtr;
   struct FunctionHash *next;
  };

#define SIZE_FUNCTION_HASH 517

   void                           InitializeExternalFunctionData(Environment *);
   bool                           EnvDefineFunction(Environment *,const char *,int,
                                                    int (*)(Environment *),const char *);
   bool                           EnvDefineFunction2(Environment *,const char *,int,
                                                            int (*)(Environment *),const char *,const char *);
   bool                           EnvDefineFunctionWithContext(Environment *,const char *,int,
                                                               int (*)(Environment *),const char *,void *);
   bool                           EnvDefineFunction2WithContext(Environment *,const char *,int,
                                                            int (*)(Environment *),const char *,const char *,void *);
   bool                           DefineFunction3(Environment *,const char *,int,unsigned,
                                                  void (*)(UDFContext *,CLIPSValue *),
                                                  const char *,int,int,const char *,void *);
   bool                           EnvAddUDF(Environment *,const char *,const char *,
                                            void (*)(UDFContext *,CLIPSValue *),
                                            const char *,int,int,const char *,void *);


   bool                           AddFunctionParser(Environment *,const char *,
                                                           struct expr *(*)( Environment *,struct expr *,const char *));
   bool                           RemoveFunctionParser(Environment *,const char *);
   bool                           FuncSeqOvlFlags(Environment *,const char *,bool,bool);
   struct FunctionDefinition     *GetFunctionList(Environment *);
   void                           InstallFunctionList(Environment *,struct FunctionDefinition *);
   struct FunctionDefinition     *FindFunction(Environment *,const char *);
   int                            GetNthRestriction(struct FunctionDefinition *,int);
   unsigned                       GetNthRestriction2(Environment *,struct FunctionDefinition *,int);
   const char                    *GetArgumentTypeName(int);
   bool                           UndefineFunction(Environment *,const char *);
   int                            GetMinimumArgs(struct FunctionDefinition *);
   int                            GetMaximumArgs(struct FunctionDefinition *);

   int                            UDFArgumentCount(UDFContext *);
   bool                           UDFNthArgument(UDFContext *,int,unsigned,struct dataObject *);
   void                           UDFInvalidArgumentMessage(UDFContext *,const char *);
   Environment                   *UDFContextEnvironment(UDFContext *);
   void                          *UDFContextUserContext(UDFContext *);
   const char                    *UDFContextFunctionName(UDFContext *);
   void                           PrintTypesString(void *,const char *,unsigned,bool);
   bool                           UDFFirstArgument(UDFContext *,unsigned,CLIPSValue *);
   bool                           UDFNextArgument(UDFContext *,unsigned,CLIPSValue *);
   void                           UDFThrowError(UDFContext *);

#define UDFHasNextArgument(context) (context->lastArg != NULL)

#endif /* _H_extnfunc */



