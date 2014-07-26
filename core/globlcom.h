   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  07/25/14            */
   /*                                                     */
   /*            DEFGLOBAL COMMANDS HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
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
/*      6.30: Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_globlcom
#define _H_globlcom

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _GLOBLCOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

#define GetResetGlobals() EnvGetResetGlobals(GetCurrentEnvironment())
#define SetResetGlobals(a) EnvSetResetGlobals(GetCurrentEnvironment(),a)
#define ShowDefglobals(a,b) EnvShowDefglobals(GetCurrentEnvironment(),a,b)

   LOCALE void                           DefglobalCommandDefinitions(void *);
   LOCALE int                            SetResetGlobalsCommand(void *);
   LOCALE intBool                        EnvSetResetGlobals(void *,int);
   LOCALE int                            GetResetGlobalsCommand(void *);
   LOCALE intBool                        EnvGetResetGlobals(void *);
   LOCALE void                           ShowDefglobalsCommand(void *);
   LOCALE void                           EnvShowDefglobals(void *,const char *,void *);

#endif

