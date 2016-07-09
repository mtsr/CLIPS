   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  07/05/16             */
   /*                                                     */
   /*              DEFFACTS DEFINITION MODULE             */
   /*******************************************************/

/*************************************************************/
/* Purpose: Defines basic deffacts primitive functions such  */
/*   as allocating and deallocating, traversing, and finding */
/*   deffacts data structures.                               */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Corrected code to remove run-time program      */
/*            compiler warning.                              */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Changed find construct functionality so that   */
/*            imported modules are search when locating a    */
/*            named construct.                               */
/*                                                           */
/*      6.40: Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*************************************************************/

#include "setup.h"

#if DEFFACTS_CONSTRUCT

#include <stdio.h>

#include "dffctbsc.h"
#include "dffctpsr.h"
#include "envrnmnt.h"
#include "memalloc.h"

#if BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE
#include "bload.h"
#include "dffctbin.h"
#endif

#if CONSTRUCT_COMPILER && (! RUN_TIME)
#include "dffctcmp.h"
#endif

#include "dffctdef.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                   *AllocateModule(void *);
   static void                    ReturnModule(void *,void *);
   static void                    ReturnDeffacts(void *,void *);
   static void                    InitializeDeffactsModules(void *);
   static void                    DeallocateDeffactsData(void *);
#if ! RUN_TIME
   static void                    DestroyDeffactsAction(void *,struct constructHeader *,void *);
#endif

/***********************************************************/
/* InitializeDeffacts: Initializes the deffacts construct. */
/***********************************************************/
void InitializeDeffacts(  
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,DEFFACTS_DATA,sizeof(struct deffactsData),DeallocateDeffactsData);
  
   InitializeDeffactsModules(theEnv);

   DeffactsBasicCommands(theEnv);

   DeffactsData(theEnv)->DeffactsConstruct =
      AddConstruct(theEnv,"deffacts","deffacts",ParseDeffacts,EnvFindDeffacts,
                   GetConstructNamePointer,GetConstructPPForm,
                   GetConstructModuleItem,EnvGetNextDeffacts,SetNextConstruct,
                   EnvIsDeffactsDeletable,EnvUndeffacts,ReturnDeffacts);
  }
  
/***************************************************/
/* DeallocateDeffactsData: Deallocates environment */
/*    data for the deffacts construct.             */
/***************************************************/
static void DeallocateDeffactsData(
  void *theEnv)
  {
#if ! RUN_TIME
   struct deffactsModule *theModuleItem;
   void *theModule;

#if BLOAD || BLOAD_AND_BSAVE
   if (Bloaded(theEnv)) return;
#endif

   DoForAllConstructs(theEnv,DestroyDeffactsAction,DeffactsData(theEnv)->DeffactsModuleIndex,false,NULL);

   for (theModule = EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = EnvGetNextDefmodule(theEnv,theModule))
     {
      theModuleItem = (struct deffactsModule *)
                      GetModuleItem(theEnv,(struct defmodule *) theModule,
                                    DeffactsData(theEnv)->DeffactsModuleIndex);
      rtn_struct(theEnv,deffactsModule,theModuleItem);
     }
#else
#if MAC_XCD
#pragma unused(theEnv)
#endif
#endif
  }
  
#if ! RUN_TIME
/*********************************************************/
/* DestroyDeffactsAction: Action used to remove deffacts */
/*   as a result of DestroyEnvironment.                  */
/*********************************************************/
static void DestroyDeffactsAction(
  void *theEnv,
  struct constructHeader *theConstruct,
  void *buffer)
  {
#if MAC_XCD
#pragma unused(buffer)
#endif
#if (! BLOAD_ONLY) && (! RUN_TIME)
   struct deffacts *theDeffacts = (struct deffacts *) theConstruct;
   
   if (theDeffacts == NULL) return;

   ReturnPackedExpression(theEnv,theDeffacts->assertList);
   
   DestroyConstructHeader(theEnv,&theDeffacts->header);

   rtn_struct(theEnv,deffacts,theDeffacts);
#else
#if MAC_XCD
#pragma unused(theEnv,theConstruct)
#endif
#endif
  }
#endif

/*******************************************************/
/* InitializeDeffactsModules: Initializes the deffacts */
/*   construct for use with the defmodule construct.   */
/*******************************************************/
static void InitializeDeffactsModules(  
  void *theEnv)
  {
   DeffactsData(theEnv)->DeffactsModuleIndex = 
      RegisterModuleItem(theEnv,"deffacts",
                         AllocateModule,
                         ReturnModule,
#if BLOAD_AND_BSAVE || BLOAD || BLOAD_ONLY
                         BloadDeffactsModuleReference,
#else
                         NULL,
#endif
#if CONSTRUCT_COMPILER && (! RUN_TIME)
                         DeffactsCModuleReference,
#else
                         NULL,
#endif
                         EnvFindDeffactsInModule);
  }

/************************************************/
/* AllocateModule: Allocates a deffacts module. */
/************************************************/
static void *AllocateModule(
  void *theEnv)
  {
   return((void *) get_struct(theEnv,deffactsModule)); 
  }

/************************************************/
/* ReturnModule: Deallocates a deffacts module. */
/************************************************/
static void ReturnModule(
  void *theEnv,
  void *theItem)
  {
   FreeConstructHeaderModule(theEnv,(struct defmoduleItemHeader *) theItem,DeffactsData(theEnv)->DeffactsConstruct);
   rtn_struct(theEnv,deffactsModule,theItem);
  }

/*************************************************************/
/* GetDeffactsModuleItem: Returns a pointer to the defmodule */
/*  item for the specified deffacts or defmodule.            */
/*************************************************************/
struct deffactsModule *GetDeffactsModuleItem(
  void *theEnv,
  struct defmodule *theModule)
  { 
   return((struct deffactsModule *) GetConstructModuleItemByIndex(theEnv,theModule,DeffactsData(theEnv)->DeffactsModuleIndex)); 
  }

/**************************************************/
/* EnvFindDeffacts: Searches for a deffact in the */
/*   list of deffacts. Returns a pointer to the   */
/*   deffact if found, otherwise NULL.            */
/**************************************************/
void *EnvFindDeffacts(
  void *theEnv,
  const char *deffactsName)
  { 
   return(FindNamedConstructInModuleOrImports(theEnv,deffactsName,DeffactsData(theEnv)->DeffactsConstruct)); 
  }

/**************************************************/
/* EnvFindDeffactsInModule: Searches for a deffact in the */
/*   list of deffacts. Returns a pointer to the   */
/*   deffact if found, otherwise NULL.            */
/**************************************************/
void *EnvFindDeffactsInModule(
  void *theEnv,
  const char *deffactsName)
  { 
   return(FindNamedConstructInModule(theEnv,deffactsName,DeffactsData(theEnv)->DeffactsConstruct));
  }

/*********************************************************/
/* EnvGetNextDeffacts: If passed a NULL pointer, returns */
/*   the first deffacts in the ListOfDeffacts. Otherwise */
/*   returns the next deffacts following the deffacts    */
/*   passed as an argument.                              */
/*********************************************************/
void *EnvGetNextDeffacts(
  void *theEnv,
  void *deffactsPtr)
  {
   return((void *) GetNextConstructItem(theEnv,(struct constructHeader *) deffactsPtr,DeffactsData(theEnv)->DeffactsModuleIndex)); 
  }

/********************************************************/
/* EnvIsDeffactsDeletable: Returns true if a particular */
/*   deffacts can be deleted, otherwise returns false.  */
/********************************************************/
bool EnvIsDeffactsDeletable(
  void *theEnv,
  void *ptr)
  {
#if MAC_XCD
#pragma unused(ptr)
#endif
   if (! ConstructsDeletable(theEnv))
     { return false; }

   if (ConstructData(theEnv)->ResetInProgress) return false;

   return true;
  }

/***********************************************************/
/* ReturnDeffacts: Returns the data structures associated  */
/*   with a deffacts construct to the pool of free memory. */
/***********************************************************/
static void ReturnDeffacts(
  void *theEnv,
  void *vTheDeffacts)
  {
#if (! BLOAD_ONLY) && (! RUN_TIME)
   struct deffacts *theDeffacts = (struct deffacts *) vTheDeffacts;

   if (theDeffacts == NULL) return;

   ExpressionDeinstall(theEnv,theDeffacts->assertList);
   ReturnPackedExpression(theEnv,theDeffacts->assertList);

   DeinstallConstructHeader(theEnv,&theDeffacts->header);

   rtn_struct(theEnv,deffacts,theDeffacts);
#endif
  }

/*##################################*/
/* Additional Environment Functions */
/*##################################*/

const char *EnvDeffactsModule(
  void *theEnv,
  void *theDeffacts)
  {
   return GetConstructModuleName((struct constructHeader *) theDeffacts);
  }

const char *EnvGetDeffactsName(
  void *theEnv,
  void *theDeffacts)
  {
   return GetConstructNameString((struct constructHeader *) theDeffacts);
  }

const char *EnvGetDeffactsPPForm(
  void *theEnv,
  void *theDeffacts)
  {
   return GetConstructPPForm(theEnv,(struct constructHeader *) theDeffacts);
  }


#endif /* DEFFACTS_CONSTRUCT */


