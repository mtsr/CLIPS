#ifndef CLIPSWin32_H
#define CLIPSWin32_H

/*
struct dataObject
  {
   void *supplementalInfo;
   unsigned short type;
   void *value;
   long begin;
   long end;
   struct dataObject *next;
  };

typedef struct dataObject DATA_OBJECT;
typedef struct dataObject * DATA_OBJECT_PTR;
*/
void __declspec(dllimport) * __CreateEnvironment(void);
void __declspec(dllimport) __DestroyEnvironment(void *);
void __declspec(dllimport) __EnvClear(void *);
void __declspec(dllimport) __EnvReset(void *);
int __declspec(dllimport) __EnvLoad(void *,char *);
long long __declspec(dllimport) __EnvRun(void *,long long);
int __declspec(dllimport) __EnvBuild(void *,char *);
int __declspec(dllimport) __EnvEval(void *,char *,void *);  
void __declspec(dllimport) __EnvIncrementFactCount(void *,void *);
void __declspec(dllimport) __EnvDecrementFactCount(void *,void *);
void __declspec(dllimport) __EnvIncrementInstanceCount(void *,void *);
void __declspec(dllimport) __EnvDecrementInstanceCount(void *,void *);
long long __declspec(dllimport) __EnvFactIndex(void *,void *);
int __declspec(dllimport) __EnvGetFactSlot(void *,void *,char *,void *);  
char __declspec(dllimport) * __EnvGetInstanceName(void *,void *); 
int __declspec(dllimport) __EnvWatch(void *,char *);  
int __declspec(dllimport) __EnvUnwatch(void *,char *);  
int __declspec(dllexport) __EnvAddRouterWithContext(void *,const char *,int,int (*)(void *,const char *),
                                                    int (*)(void *,const char *,const char *),int (*)(void *,const char *),
                                                    int (*)(void *,int,const char *),int (*)(void *,int),void *);
void __declspec(dllexport) * __GetEnvironmentContext(void *);
void __declspec(dllexport) * __GetEnvironmentRouterContext(void *);
#endif