   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/02/14            */
   /*                                                     */
   /*            EXPRESSION PARSER HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for parsing expressions.       */
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
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*************************************************************/

#ifndef _H_exprnpsr

#define _H_exprnpsr

#if (! RUN_TIME)

typedef struct saved_contexts
  {
   int rtn;
   int brk;
   struct saved_contexts *nxt;
  } SAVED_CONTEXTS;

#endif

#ifndef _H_extnfunc
#include "extnfunc.h"
#endif
#ifndef _H_scanner
#include "scanner.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _EXPRNPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE struct expr                   *Function0Parse(void *,const char *);
   LOCALE struct expr                   *Function1Parse(void *,const char *);
   LOCALE struct expr                   *Function2Parse(void *,const char *,const char *);
   LOCALE void                           PushRtnBrkContexts(void *);
   LOCALE void                           PopRtnBrkContexts(void *);
   LOCALE intBool                        ReplaceSequenceExpansionOps(void *,struct expr *,struct expr *,
                                                                     void *,void *);
   LOCALE struct expr                   *CollectArguments(void *,struct expr *,const char *);
   LOCALE struct expr                   *ArgumentParse(void *,const char *,int *);
   LOCALE struct expr                   *ParseAtomOrExpression(void *,const char *,struct token *);
   LOCALE EXPRESSION                    *ParseConstantArguments(void *,const char *,int *);
   LOCALE intBool                        EnvSetSequenceOperatorRecognition(void *,int);
   LOCALE intBool                        EnvGetSequenceOperatorRecognition(void *);
   LOCALE struct expr                   *GroupActions(void *,const char *,struct token *,
                                                      int,const char *,int);
   LOCALE struct expr                   *RemoveUnneededProgn(void *,struct expr *);

#if (! RUN_TIME)

   LOCALE int                            CheckExpressionAgainstRestrictions(void *,struct expr *,
                                                                            const char *,const char *);
#endif

#if ALLOW_ENVIRONMENT_GLOBALS

   LOCALE intBool                        SetSequenceOperatorRecognition(int);
   LOCALE intBool                        GetSequenceOperatorRecognition();

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif




