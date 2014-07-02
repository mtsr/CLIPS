   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.20  01/31/02            */
   /*                                                     */
   /*              CONSTRUCT PARSER MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Parsing routines and utilities for parsing       */
/*   constructs.                                             */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/

#ifndef _H_cstrcpsr

#define _H_cstrcpsr

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif
#ifndef _H_constrct
#include "constrct.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CSTRCPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#if ALLOW_ENVIRONMENT_GLOBALS
   LOCALE int                            Load(char *);
#endif

   LOCALE int                            EnvLoad(void *,char *);
   LOCALE int                            LoadConstructsFromLogicalName(void *,char *);
   LOCALE int                            ParseConstruct(void *,char *,char *);
   LOCALE void                           RemoveConstructFromModule(void *,struct constructHeader *);
   LOCALE struct symbolHashNode         *GetConstructNameAndComment(void *,char *,
                                         struct token *,char *,
                                         void *(*)(void *,char *),
                                         int (*)(void *,void *),
                                         char *,int,int,int,int);
   LOCALE void                           ImportExportConflictMessage(void *,char *,char *,char *,char *);
   LOCALE void                           FlushParsingMessages(void *);
   LOCALE char                          *EnvGetParsingFileName(void *);
   LOCALE void                           EnvSetParsingFileName(void *,char *);
   LOCALE char                          *EnvGetErrorFileName(void *);
   LOCALE void                           EnvSetErrorFileName(void *,char *);
   LOCALE char                          *EnvGetWarningFileName(void *);
   LOCALE void                           EnvSetWarningFileName(void *,char *);
   LOCALE void                           CreateErrorCaptureRouter(void *);
   LOCALE void                           DeleteErrorCaptureRouter(void *);

#endif







