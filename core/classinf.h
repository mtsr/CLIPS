   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  08/06/16            */
   /*                                                     */
   /*     CLASS INFO PROGRAMMATIC ACCESS HEADER FILE      */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                            */
/*      6.24: Added allowed-classes slot facet.              */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Borland C (IBM_TBC) and Metrowerks CodeWarrior */
/*            (MAC_MCW, IBM_MCW) are no longer supported.    */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
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
/*            ALLOW_ENVIRONMENT_GLOBALS no longer supported. */
/*                                                           */
/*************************************************************/

#ifndef _H_classinf

#pragma once

#define _H_classinf

#include "evaluatn.h"

   void                           ClassAbstractPCommand(UDFContext *,CLIPSValue *);
#if DEFRULE_CONSTRUCT
   void                           ClassReactivePCommand(UDFContext *,CLIPSValue *);
#endif
   Defclass                      *ClassInfoFnxArgs(UDFContext *,const char *,bool *);
   void                           ClassSlotsCommand(UDFContext *,CLIPSValue *);
   void                           ClassSuperclassesCommand(UDFContext *,CLIPSValue *);
   void                           ClassSubclassesCommand(UDFContext *,CLIPSValue *);
   void                           GetDefmessageHandlersListCmd(UDFContext *,CLIPSValue *);
   void                           SlotFacetsCommand(UDFContext *,CLIPSValue *);
   void                           SlotSourcesCommand(UDFContext *,CLIPSValue *);
   void                           SlotTypesCommand(UDFContext *,CLIPSValue *);
   void                           SlotAllowedValuesCommand(UDFContext *,CLIPSValue *);
   void                           SlotAllowedClassesCommand(UDFContext *,CLIPSValue *);
   void                           SlotRangeCommand(UDFContext *,CLIPSValue *);
   void                           SlotCardinalityCommand(UDFContext *,CLIPSValue *);
   bool                           EnvClassAbstractP(Environment *,Defclass *);
#if DEFRULE_CONSTRUCT
   bool                           EnvClassReactiveP(Environment *,Defclass *);
#endif
   void                           EnvClassSlots(Environment *,Defclass *,DATA_OBJECT *,bool);
   void                           EnvGetDefmessageHandlerList(Environment *,Defclass *,DATA_OBJECT *,bool);
   void                           EnvClassSuperclasses(Environment *,Defclass *,DATA_OBJECT *,bool);
   void                           EnvClassSubclasses(Environment *,Defclass *,DATA_OBJECT *,bool);
   void                           ClassSubclassAddresses(Environment *,Defclass *,DATA_OBJECT *,bool);
   void                           EnvSlotFacets(Environment *,Defclass *,const char *,DATA_OBJECT *);
   void                           EnvSlotSources(Environment *,Defclass *,const char *,DATA_OBJECT *);
   void                           EnvSlotTypes(Environment *,Defclass *,const char *,DATA_OBJECT *);
   void                           EnvSlotAllowedValues(Environment *,Defclass *,const char *,DATA_OBJECT *);
   void                           EnvSlotAllowedClasses(Environment *,Defclass *,const char *,DATA_OBJECT *);
   void                           EnvSlotRange(Environment *,Defclass *,const char *,DATA_OBJECT *);
   void                           EnvSlotCardinality(Environment *,Defclass *,const char *,DATA_OBJECT *);

#endif /* _H_classinf */





