   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  07/25/14            */
   /*                                                     */
   /*                 ROUTER HEADER FILE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides a centralized mechanism for handling    */
/*   input and output requests.                              */
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
/*            Added support for passing context information  */ 
/*            to the router functions.                       */
/*                                                           */
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_router
#define _H_router

#ifndef _H_prntutil
#include "prntutil.h"
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#define WWARNING "wwarning"
#define WERROR "werror"
#define WTRACE "wtrace"
#define WDIALOG "wdialog"
#define WPROMPT  WPROMPT_STRING
#define WDISPLAY "wdisplay"

#define ROUTER_DATA 46

struct router
  {
   const char *name;
   int active;
   int priority;
   short int environmentAware;
   void *context;
   int (*query)(void *,const char *);
   int (*printer)(void *,const char *,const char *);
   int (*exiter)(void *,int);
   int (*charget)(void *,const char *);
   int (*charunget)(void *,int,const char *);
   struct router *next;
  };

struct routerData
  { 
   size_t CommandBufferInputCount;
   int AwaitingInput;
   const char *LineCountRouter;
   const char *FastCharGetRouter;
   char *FastCharGetString;
   long FastCharGetIndex;
   struct router *ListOfRouters;
   FILE *FastLoadFilePtr;
   FILE *FastSaveFilePtr;
   int Abort;
  };

#define RouterData(theEnv) ((struct routerData *) GetEnvironmentData(theEnv,ROUTER_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _ROUTER_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define ExitRouter(a) EnvExitRouter(GetCurrentEnvironment(),a)
#define GetcRouter(a) EnvGetcRouter(GetCurrentEnvironment(),a)
#define PrintRouter(a,b) EnvPrintRouter(GetCurrentEnvironment(),a,b)
#define UngetcRouter(a,b) EnvUngetcRouter(GetCurrentEnvironment(),a,b)
#define ActivateRouter(a) EnvActivateRouter(GetCurrentEnvironment(),a)
#define DeactivateRouter(a) EnvDeactivateRouter(GetCurrentEnvironment(),a)
#define DeleteRouter(a) EnvDeleteRouter(GetCurrentEnvironment(),a)

   LOCALE void                           InitializeDefaultRouters(void *);
   LOCALE int                            EnvPrintRouter(void *,const char *,const char *);
   LOCALE int                            EnvGetcRouter(void *,const char *);
   LOCALE int                            EnvUngetcRouter(void *,int,const char *);
   LOCALE void                           EnvExitRouter(void *,int);
   LOCALE void                           AbortExit(void *);
   LOCALE intBool                        EnvAddRouterWithContext(void *,
                                                   const char *,int,
                                                   int (*)(void *,const char *),
                                                   int (*)(void *,const char *,const char *),
                                                   int (*)(void *,const char *),
                                                   int (*)(void *,int,const char *),
                                                   int (*)(void *,int),
                                                   void *);
   LOCALE intBool                        EnvAddRouter(void *,
                                                   const char *,int,
                                                   int (*)(void *,const char *),
                                                   int (*)(void *,const char *,const char *),
                                                   int (*)(void *,const char *),
                                                   int (*)(void *,int,const char *),
                                                   int (*)(void *,int));
   LOCALE intBool                        AddRouter(const char *,int,
                                                   int (*)(const char *),
                                                   int (*)(const char *,const char *),
                                                   int (*)(const char *),
                                                   int (*)(int,const char *),
                                                   int (*)(int));
   LOCALE int                            EnvDeleteRouter(void *,const char *);
   LOCALE int                            QueryRouters(void *,const char *);
   LOCALE int                            EnvDeactivateRouter(void *,const char *);
   LOCALE int                            EnvActivateRouter(void *,const char *);
   LOCALE void                           SetFastLoad(void *,FILE *);
   LOCALE void                           SetFastSave(void *,FILE *);
   LOCALE FILE                          *GetFastLoad(void *);
   LOCALE FILE                          *GetFastSave(void *);
   LOCALE void                           UnrecognizedRouterMessage(void *,const char *);
   LOCALE void                           ExitCommand(void *);
   LOCALE int                            PrintNRouter(void *,const char *,const char *,unsigned long);

#endif


