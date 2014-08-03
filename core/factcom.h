   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/02/14            */
   /*                                                     */
   /*               FACT COMMANDS HEADER FILE             */
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
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*************************************************************/

#ifndef _H_factcom
#define _H_factcom

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FACTCOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           FactCommandDefinitions(void *);
   LOCALE void                           AssertCommand(void *,DATA_OBJECT_PTR);
   LOCALE void                           RetractCommand(void *);
   LOCALE void                           AssertStringFunction(void *,DATA_OBJECT_PTR);
   LOCALE void                           FactsCommand(void *);
   LOCALE void                           EnvFacts(void *,const char *,void *,long long,long long,long long);
   LOCALE int                            SetFactDuplicationCommand(void *);
   LOCALE int                            GetFactDuplicationCommand(void *);
   LOCALE int                            SaveFactsCommand(void *);
   LOCALE int                            LoadFactsCommand(void *);
   LOCALE int                            EnvSaveFacts(void *,const char *,int,struct expr *);
   LOCALE int                            EnvLoadFacts(void *,const char *);
   LOCALE int                            EnvLoadFactsFromString(void *,const char *,int);
   LOCALE long long                      FactIndexFunction(void *);

#if ALLOW_ENVIRONMENT_GLOBALS

#if DEBUGGING_FUNCTIONS
   LOCALE void                           Facts(const char *,void *,long long,long long,long long);
#endif
   LOCALE intBool                        LoadFacts(const char *);
   LOCALE intBool                        SaveFacts(const char *,int,struct expr *);
   LOCALE intBool                        LoadFactsFromString(const char *,int);

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif /* _H_factcom */


