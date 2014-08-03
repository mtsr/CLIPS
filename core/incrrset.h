   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/02/14            */
   /*                                                     */
   /*            INCREMENTAL RESET HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides functionality for the incremental       */
/*   reset of the pattern and join networks when a new       */
/*   rule is added.                                          */
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
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*************************************************************/

#ifndef _H_incrrset

#define _H_incrrset

#ifndef _H_ruledef
#include "ruledef.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _INCRRSET_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           IncrementalReset(void *,struct defrule *);
   LOCALE intBool                        EnvGetIncrementalReset(void *);
   LOCALE intBool                        EnvSetIncrementalReset(void *,intBool);
   LOCALE int                            GetIncrementalResetCommand(void *);
   LOCALE int                            SetIncrementalResetCommand(void *);

#if ALLOW_ENVIRONMENT_GLOBALS

   LOCALE intBool                        GetIncrementalReset(void);
   LOCALE intBool                        SetIncrementalReset(int);

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif /* _H_incrrset */









