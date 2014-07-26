   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  07/25/14            */
   /*                                                     */
   /*                  WATCH HEADER FILE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Support functions for the watch and unwatch      */
/*   commands.                                               */
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
/*            Added EnvSetWatchItem function.                */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_watch
#define _H_watch

#ifndef _H_expressn
#include "expressn.h"
#endif

#define WATCH_DATA 54

struct watchItem
  {
   const char *name;
   unsigned *flag;
   int code,priority;
   unsigned (*accessFunc)(void *,int,unsigned,struct expr *);
   unsigned (*printFunc)(void *,const char *,int,struct expr *);
   struct watchItem *next;
  };

struct watchData
  { 
   struct watchItem *ListOfWatchItems;
  };

#define WatchData(theEnv) ((struct watchData *) GetEnvironmentData(theEnv,WATCH_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _WATCH_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define GetWatchItem(a) EnvGetWatchItem(GetCurrentEnvironment(),a)
#define SetWatchItem(a,b) EnvSetWatchItem(GetCurrentEnvironment(),a,b)

#if ALLOW_ENVIRONMENT_GLOBALS
   LOCALE intBool                        Watch(const char *);
   LOCALE intBool                        Unwatch(const char *);
#endif

   LOCALE intBool                        EnvWatch(void *,const char *);
   LOCALE intBool                        EnvUnwatch(void *,const char *);
   LOCALE void                           InitializeWatchData(void *);   
   LOCALE int                            EnvSetWatchItem(void *,const char *,unsigned,struct expr *);
   LOCALE int                            EnvGetWatchItem(void *,const char *);
   LOCALE intBool                        AddWatchItem(void *,const char *,int,unsigned *,int,
                                                      unsigned (*)(void *,int,unsigned,struct expr *),
                                                      unsigned (*)(void *,const char *,int,struct expr *));
   LOCALE const char                    *GetNthWatchName(void *,int);
   LOCALE int                            GetNthWatchValue(void *,int);
   LOCALE void                           WatchCommand(void *);
   LOCALE void                           UnwatchCommand(void *);
   LOCALE void                           ListWatchItemsCommand(void *);
   LOCALE void                           WatchFunctionDefinitions(void *);
   LOCALE int                            GetWatchItemCommand(void *);

#endif



