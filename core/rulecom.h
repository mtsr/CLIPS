   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  07/21/14            */
   /*                                                     */
   /*             DEFRULE COMMANDS HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides the matches command. Also provides the  */
/*   the developer commands show-joins and rule-complexity.  */
/*   Also provides the initialization routine which          */
/*   registers rule commands found in other modules.         */
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
/*      6.30: Added support for hashed alpha memories.       */
/*                                                           */
/*            Improvements to matches command.               */
/*                                                           */
/*            Add join-activity and join-activity-reset      */
/*            commands.                                      */
/*                                                           */
/*            Added get-join-hashing and set-join-hashing    */
/*            functions.                                     */
/*                                                           */
/*************************************************************/

#ifndef _H_rulecom
#define _H_rulecom

#ifndef _H_evaluatn
#include "evaluatn.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULECOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

struct joinInformation
  {
   int whichCE;
   struct joinNode *theJoin;
   int patternBegin;
   int patternEnd;
   int marked;
   struct betaMemory *theMemory;
   struct joinNode *nextJoin;
  };

/* TBD Remove
#define Matches(a) EnvMatches(GetCurrentEnvironment(),a)
#define JoinActivity(a,b) EnvJoinActivity(GetCurrentEnvironment(),a,b)
#define MatchesCount(a) EnvMatchesCount(GetCurrentEnvironment(),a)
#define GetBetaMemoryResizing() EnvGetBetaMemoryResizing(GetCurrentEnvironment())
#define SetBetaMemoryResizing(a) EnvSetBetaMemoryResizing(GetCurrentEnvironment(),a)
*/

#define VERBOSE  0
#define SUCCINCT 1
#define TERSE    2

   LOCALE intBool                        EnvGetBetaMemoryResizing(void *);
   LOCALE intBool                        EnvSetBetaMemoryResizing(void *,intBool);
   LOCALE int                            GetBetaMemoryResizingCommand(void *);
   LOCALE int                            SetBetaMemoryResizingCommand(void *);

   LOCALE void                           EnvMatches(void *,void *,int,DATA_OBJECT *);
   LOCALE void                           EnvJoinActivity(void *,void *,int,DATA_OBJECT *);
   LOCALE void                           DefruleCommands(void *);
   LOCALE void                           MatchesCommand(void *,DATA_OBJECT *);
   LOCALE void                           JoinActivityCommand(void *,DATA_OBJECT *);
   LOCALE long long                      TimetagFunction(void *);
   LOCALE long                           EnvAlphaJoinCount(void *,void *);
   LOCALE long                           EnvBetaJoinCount(void *,void *);
   LOCALE struct joinInformation        *EnvCreateJoinArray(void *,long);
   LOCALE void                           EnvFreeJoinArray(void *,struct joinInformation *,long);
   LOCALE void                           EnvAlphaJoins(void *,void *,long,struct joinInformation *);
   LOCALE void                           EnvBetaJoins(void *,void *,long,struct joinInformation *);
   LOCALE void                           JoinActivityResetCommand(void *);
#if DEVELOPER
   LOCALE void                           ShowJoinsCommand(void *);
   LOCALE long                           RuleComplexityCommand(void *);
   LOCALE void                           ShowAlphaHashTable(void *);
#endif

#endif


