   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  07/25/14            */
   /*                                                     */
   /*               RULE PARSING HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Coordinates parsing of a rule.                   */
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
/*************************************************************/

#ifndef _H_rulepsr
#define _H_rulepsr

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULEPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE int                            ParseDefrule(void *,const char *);
   LOCALE struct lhsParseNode           *FindVariable(struct symbolHashNode *,
                                                      struct lhsParseNode *);
#if DEVELOPER && DEBUGGING_FUNCTIONS
   LOCALE void                           DumpRuleAnalysis(void *,struct lhsParseNode *);
#endif

#endif


