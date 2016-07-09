   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  07/05/16             */
   /*                                                     */
   /*         DEFGLOBAL BASIC COMMANDS HEADER FILE        */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements core commands for the defglobal       */
/*   construct such as clear, reset, save, undefglobal,      */
/*   ppdefglobal, list-defglobals, and get-defglobals-list.  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*            Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*            Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Moved WatchGlobals global to defglobalData.    */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*************************************************************/

#include "setup.h"

#if DEFGLOBAL_CONSTRUCT

#include "constrct.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "globlbin.h"
#endif
#if CONSTRUCT_COMPILER && (! RUN_TIME)
#include "globlcmp.h"
#endif
#include "globlcom.h"
#include "globldef.h"
#include "watch.h"

#include "globlbsc.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    SaveDefglobals(void *,void *,const char *);
   static void                    ResetDefglobalAction(void *,struct constructHeader *,void *);
#if DEBUGGING_FUNCTIONS && (! RUN_TIME)
   static bool                    DefglobalWatchAccess(void *,int,bool,struct expr *);
   static bool                    DefglobalWatchPrint(void *,const char *,int,struct expr *);
#endif

/*****************************************************************/
/* DefglobalBasicCommands: Initializes basic defglobal commands. */
/*****************************************************************/
void DefglobalBasicCommands(
  void *theEnv)
  {
   AddSaveFunction(theEnv,"defglobal",SaveDefglobals,40);
   EnvAddResetFunction(theEnv,"defglobal",ResetDefglobals,50);

#if ! RUN_TIME
   EnvDefineFunction2(theEnv,"get-defglobal-list",'m',PTIEF GetDefglobalListFunction,"GetDefglobalListFunction","01w");
   EnvDefineFunction2(theEnv,"undefglobal",'v',PTIEF UndefglobalCommand,"UndefglobalCommand","11w");
   EnvDefineFunction2(theEnv,"defglobal-module",'w',PTIEF DefglobalModuleFunction,"DefglobalModuleFunction","11w");

#if DEBUGGING_FUNCTIONS
   EnvDefineFunction2(theEnv,"list-defglobals",'v', PTIEF ListDefglobalsCommand,"ListDefglobalsCommand","01w");
   EnvDefineFunction2(theEnv,"ppdefglobal",'v',PTIEF PPDefglobalCommand,"PPDefglobalCommand","11w");
   AddWatchItem(theEnv,"globals",0,&DefglobalData(theEnv)->WatchGlobals,0,DefglobalWatchAccess,DefglobalWatchPrint);
#endif

#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE)
   DefglobalBinarySetup(theEnv);
#endif

#if CONSTRUCT_COMPILER && (! RUN_TIME)
   DefglobalCompilerSetup(theEnv);
#endif

#endif
  }

/*************************************************************/
/* ResetDefglobals: Defglobal reset routine for use with the */
/*   reset command. Restores the values of the defglobals.   */
/*************************************************************/
void ResetDefglobals(
  void *theEnv)
  {
   if (! EnvGetResetGlobals(theEnv)) return;
   DoForAllConstructs(theEnv,ResetDefglobalAction,DefglobalData(theEnv)->DefglobalModuleIndex,true,NULL);
  }

/******************************************************/
/* ResetDefglobalAction: Action to be applied to each */
/*   defglobal construct during a reset command.      */
/******************************************************/
static void ResetDefglobalAction(
  void *theEnv,
  struct constructHeader *theConstruct,
  void *buffer)
  {
#if MAC_XCD
#pragma unused(buffer)
#endif
   struct defglobal *theDefglobal = (struct defglobal *) theConstruct;
   DATA_OBJECT assignValue;

   if (EvaluateExpression(theEnv,theDefglobal->initial,&assignValue))
     {
      assignValue.type = SYMBOL;
      assignValue.value = EnvFalseSymbol(theEnv);
     }

   QSetDefglobalValue(theEnv,theDefglobal,&assignValue,false);
  }

/******************************************/
/* SaveDefglobals: Defglobal save routine */
/*   for use with the save command.       */
/******************************************/
static void SaveDefglobals(
  void *theEnv,
  void *theModule,
  const char *logicalName)
  {
   SaveConstruct(theEnv,theModule,logicalName,DefglobalData(theEnv)->DefglobalConstruct); 
  }

/********************************************/
/* UndefglobalCommand: H/L access routine   */
/*   for the undefglobal command.           */
/********************************************/
void UndefglobalCommand(
  void *theEnv)
  {
   UndefconstructCommand(theEnv,"undefglobal",DefglobalData(theEnv)->DefglobalConstruct); 
  }

/************************************/
/* EnvUndefglobal: C access routine */
/*   for the undefglobal command.   */
/************************************/
bool EnvUndefglobal(
  void *theEnv,
  void *theDefglobal)
  {
   return(Undefconstruct(theEnv,theDefglobal,DefglobalData(theEnv)->DefglobalConstruct)); 
  }

/**************************************************/
/* GetDefglobalListFunction: H/L access routine   */
/*   for the get-defglobal-list function.         */
/**************************************************/
void GetDefglobalListFunction(
  void *theEnv,
  DATA_OBJECT_PTR returnValue)
  { 
   GetConstructListFunction(theEnv,"get-defglobal-list",returnValue,DefglobalData(theEnv)->DefglobalConstruct); 
  }

/******************************************/
/* EnvGetDefglobalList: C access routine  */
/*   for the get-defglobal-list function. */
/******************************************/
void EnvGetDefglobalList(
  void *theEnv,
  DATA_OBJECT_PTR returnValue,
  void *theModule)
  {
   GetConstructList(theEnv,returnValue,DefglobalData(theEnv)->DefglobalConstruct,(struct defmodule *) theModule); 
  }

/*************************************************/
/* DefglobalModuleFunction: H/L access routine   */
/*   for the defglobal-module function.          */
/*************************************************/
void *DefglobalModuleFunction(
  void *theEnv)
  { 
   return(GetConstructModuleCommand(theEnv,"defglobal-module",DefglobalData(theEnv)->DefglobalConstruct)); 
  }

#if DEBUGGING_FUNCTIONS

/********************************************/
/* PPDefglobalCommand: H/L access routine   */
/*   for the ppdefglobal command.           */
/********************************************/
void PPDefglobalCommand(
  void *theEnv)
  {
   PPConstructCommand(theEnv,"ppdefglobal",DefglobalData(theEnv)->DefglobalConstruct); 
  }

/*************************************/
/* PPDefglobal: C access routine for */
/*   the ppdefglobal command.        */
/*************************************/
int PPDefglobal(
  void *theEnv,
  const char *defglobalName,
  const char *logicalName)
  {
   return(PPConstruct(theEnv,defglobalName,logicalName,DefglobalData(theEnv)->DefglobalConstruct)); 
  }

/***********************************************/
/* ListDefglobalsCommand: H/L access routine   */
/*   for the list-defglobals command.          */
/***********************************************/
void ListDefglobalsCommand(
  void *theEnv)
  {
   ListConstructCommand(theEnv,"list-defglobals",DefglobalData(theEnv)->DefglobalConstruct);
  }

/***************************************/
/* EnvListDefglobals: C access routine */
/*   for the list-defglobals command.  */
/***************************************/
void EnvListDefglobals(
  void *theEnv,
  const char *logicalName,
  void *vTheModule)
  {
   struct defmodule *theModule = (struct defmodule *) vTheModule;

   ListConstruct(theEnv,DefglobalData(theEnv)->DefglobalConstruct,logicalName,theModule);
  }

/*********************************************************/
/* EnvGetDefglobalWatch: C access routine for retrieving */
/*   the current watch value of a defglobal.             */
/*********************************************************/
bool EnvGetDefglobalWatch(
  void *theEnv,
  void *theGlobal)
  { 
#if MAC_XCD
#pragma unused(theEnv)
#endif

   return(((struct defglobal *) theGlobal)->watch); 
  }

/******************************************************/
/* EnvSetDefglobalWatch: C access routine for setting */
/*   the current watch value of a defglobal.          */
/******************************************************/
void EnvSetDefglobalWatch(
  void *theEnv,
  bool newState,
  void *theGlobal)
  {  
#if MAC_XCD
#pragma unused(theEnv)
#endif

   ((struct defglobal *) theGlobal)->watch = newState; 
  }

#if ! RUN_TIME

/********************************************************/
/* DefglobalWatchAccess: Access routine for setting the */
/*   watch flag of a defglobal via the watch command.   */
/********************************************************/
static bool DefglobalWatchAccess(
  void *theEnv,
  int code,
  bool newState,
  EXPRESSION *argExprs)
  {
#if MAC_XCD
#pragma unused(code)
#endif

   return(ConstructSetWatchAccess(theEnv,DefglobalData(theEnv)->DefglobalConstruct,newState,argExprs,
                                  EnvGetDefglobalWatch,EnvSetDefglobalWatch));
  }

/*********************************************************************/
/* DefglobalWatchPrint: Access routine for printing which defglobals */
/*   have their watch flag set via the list-watch-items command.     */
/*********************************************************************/
static bool DefglobalWatchPrint(
  void *theEnv,
  const char *logName,
  int code,
  EXPRESSION *argExprs)
  {
#if MAC_XCD
#pragma unused(code)
#endif

   return(ConstructPrintWatchAccess(theEnv,DefglobalData(theEnv)->DefglobalConstruct,logName,argExprs,
                                    EnvGetDefglobalWatch,EnvSetDefglobalWatch));
  }

#endif /* ! RUN_TIME */

#endif /* DEBUGGING_FUNCTIONS */

/*#####################################*/
/* ALLOW_ENVIRONMENT_GLOBALS Functions */
/*#####################################*/

#if ALLOW_ENVIRONMENT_GLOBALS

void GetDefglobalList(
  DATA_OBJECT_PTR returnValue,
  void *theModule)
  {
   EnvGetDefglobalList(GetCurrentEnvironment(),returnValue,theModule);
  }

#if DEBUGGING_FUNCTIONS

bool GetDefglobalWatch(
  void *theGlobal)
  {
   return EnvGetDefglobalWatch(GetCurrentEnvironment(),theGlobal);
  }

void ListDefglobals(
  const char *logicalName,
  void *vTheModule)
  {
   EnvListDefglobals(GetCurrentEnvironment(),logicalName,vTheModule);
  }

void SetDefglobalWatch(
  bool newState,
  void *theGlobal)
  {
   EnvSetDefglobalWatch(GetCurrentEnvironment(),newState,theGlobal);
  }

#endif /* DEBUGGING_FUNCTIONS */

bool Undefglobal(
  void *theDefglobal)
  {
   return EnvUndefglobal(GetCurrentEnvironment(),theDefglobal);
  }

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif /* DEFGLOBAL_CONSTRUCT */


