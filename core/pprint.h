   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/16/14            */
   /*                                                     */
   /*               PRETTY PRINT HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Routines for processing the pretty print         */
/*   representation of constructs.                           */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Changed integer type/precision.                */
/*                                                           */
/*            Used genstrcpy instead of strcpy.              */
/*                                                           */             
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#ifndef _H_pprint
#define _H_pprint

#define PRETTY_PRINT_DATA 52

struct prettyPrintData
  { 
   int PPBufferStatus;
   int PPBufferEnabled;
   int IndentationDepth;
   size_t PPBufferPos;
   size_t PPBufferMax;
   size_t PPBackupOnce;
   size_t PPBackupTwice;
   char *PrettyPrintBuffer;
  };

#define PrettyPrintData(theEnv) ((struct prettyPrintData *) GetEnvironmentData(theEnv,PRETTY_PRINT_DATA))

   void                           InitializePrettyPrintData(void *);
   void                           FlushPPBuffer(void *);
   void                           DestroyPPBuffer(void *);
   void                           SavePPBuffer(void *,const char *);
   void                           PPBackup(void *);
   char                          *CopyPPBuffer(void *);
   char                          *GetPPBuffer(void *);
   void                           PPCRAndIndent(void *);
   void                           IncrementIndentDepth(void *,int);
   void                           DecrementIndentDepth(void *,int);
   void                           SetIndentDepth(void *,int);
   void                           SetPPBufferStatus(void *,int);
   int                            GetPPBufferStatus(void *);
   int                            SetPPBufferEnabled(void *,int);
   int                            GetPPBufferEnabled(void *);

#endif



