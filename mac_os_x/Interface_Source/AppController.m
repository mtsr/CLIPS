#import "AppController.h"
#import "PreferenceController.h"
#import "CLIPSTerminalController.h"
#import "CLIPSAgendaController.h"
#import "CLIPSFactController.h"
#import "CLIPSInstanceController.h"
#import "CLIPSConstructInspectorController.h"
#import "CLIPSEnvironment.h"
#import <CLIPS/clips.h>

@implementation AppController

@synthesize mainEnvironment, mainTerminal, constructInspectorText;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* Initialization/Deallocation Methods */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/***********************************************/
/* initialize: Set up the default preferences. */
/***********************************************/
+ (void) initialize
  { 
   NSDictionary *appDefaults; 
   NSUserDefaults *defaults; 
   NSFont *theFont;
     
   theFont = [NSFont userFixedPitchFontOfSize:0.0];

   appDefaults = 
      [NSDictionary dictionaryWithObjectsAndKeys:
         [NSNumber numberWithBool:YES], @"watchCompilations", 
         [NSNumber numberWithBool:NO],  @"watchFacts", 
         [NSNumber numberWithBool:NO],  @"watchRules", 
         [NSNumber numberWithBool:NO],  @"watchStatistics", 
         [NSNumber numberWithBool:NO],  @"watchActivations", 
         [NSNumber numberWithBool:NO],  @"watchFocus", 
         [NSNumber numberWithBool:NO],  @"watchGlobals", 
         [NSNumber numberWithBool:NO],  @"watchDeffunctions", 
         [NSNumber numberWithBool:NO],  @"watchGenericFunctions", 
         [NSNumber numberWithBool:NO],  @"watchMethods", 
         [NSNumber numberWithBool:NO],  @"watchInstances", 
         [NSNumber numberWithBool:NO],  @"watchSlots", 
         [NSNumber numberWithBool:NO],  @"watchMessageHandlers", 
         [NSNumber numberWithBool:NO],  @"watchMessages",

         [NSNumber numberWithInt: WHEN_DEFINED],   @"salienceEvaluation", 
         [NSNumber numberWithInt: DEPTH_STRATEGY], @"strategy", 
         
         [NSNumber numberWithBool:NO],  @"dynamicConstraintChecking",
         [NSNumber numberWithBool:NO],  @"sequenceExpansionOperatorRecognition",
         [NSNumber numberWithBool:NO],  @"factDuplication",
         
         [theFont fontName],                             @"editorTextFontName",
         [NSNumber numberWithFloat:[theFont pointSize]], @"editorTextFontSize", 
         [NSNumber numberWithBool:YES],                  @"editorBalanceParens",

         [NSNumber numberWithBool:NO], @"factsDisplayDefaultedValues",
         
         nil]; 
 
   defaults = [NSUserDefaults standardUserDefaults]; 
   [defaults registerDefaults:appDefaults]; 

   [[NSUserDefaultsController sharedUserDefaultsController] setInitialValues:appDefaults];
  } 

/*****************/
/* awakeFromNib: */
/*****************/
- (void) awakeFromNib
  {
   factControllers = [[NSMutableArray alloc] init];
   instanceControllers = [[NSMutableArray alloc] init];
   agendaControllers = [[NSMutableArray alloc] init];
   fileOpenLock = [[NSLock alloc] init];
  }

/*********************/
/* validateMenuItem: */
/*********************/
- (BOOL) validateMenuItem: (NSMenuItem *) menuItem
  {
   /*===================================================*/
   /* The Halt and Halt Immediately menu items are only */
   /* available if the CLIPS environment is executing.  */
   /*===================================================*/
   
   if (([menuItem action] == @selector(haltRules:)) ||
       ([menuItem action] == @selector(haltExecution:)))
     {
      if ([[mainEnvironment executionLock] tryLock])
        {
         [[mainEnvironment executionLock] unlock];
         return NO;
        }
      else
        { return YES; }
     }
     
   /*=====================================================*/
   /* The Load Constructs, Load Batch, Set Directory, and */
   /* Clear Scrollback menu items are only available if   */
   /* the CLIPS environment is not executing.             */
   /*=====================================================*/

   else if (([menuItem action] == @selector(clear:)) ||
            ([menuItem action] == @selector(loadConstructs:)) ||
            ([menuItem action] == @selector(loadBatch:)) ||
            ([menuItem action] == @selector(setDirectory:)) ||
            ([menuItem action] == @selector(reset:)) ||
            ([menuItem action] == @selector(run:)) ||
            ([menuItem action] == @selector(clearScrollback:)))
     {
      if ([[mainEnvironment executionLock] tryLock])
        {
         [[mainEnvironment executionLock] unlock];
         return YES;
        }
      else
        { return NO; }
     }

   /*===================================*/
   /* Otherwise the menu item is valid. */
   /*===================================*/
   
   return YES;
  }
  
/**********************/
/* addFactController: */
/**********************/
- (void) addFactController: (CLIPSFactController *) theController
  {
   [factControllers addObject: theController];
  }

/*************************/
/* removeFactController: */
/*************************/
- (void) removeFactController: (CLIPSFactController *) theController
  {
   [factControllers removeObject: theController];
  }

/**************************/
/* addInstanceController: */
/**************************/
- (void) addInstanceController: (CLIPSInstanceController *) theController
  {
   [instanceControllers addObject: theController];
  }

/*****************************/
/* removeInstanceController: */
/*****************************/
- (void) removeInstanceController: (CLIPSInstanceController *) theController
  {
   [instanceControllers removeObject: theController];
  }

/************************/
/* addAgendaController: */
/************************/
- (void) addAgendaController: (CLIPSAgendaController *) theController
  {
   [agendaControllers addObject: theController];
  }

/***************************/
/* removeAgendaController: */
/***************************/
- (void) removeAgendaController: (CLIPSAgendaController *) theController
  {
   [agendaControllers removeObject: theController];
  }

/*%%%%%%%%%%%%%%%%*/
/* Action Methods */
/*%%%%%%%%%%%%%%%%*/

/*********/
/* clear */
/*********/
- (IBAction) clear: (id) sender
  {
   [mainEnvironment doCommand: @"(clear)\n"];
  }

/*******************/
/* loadConstructs: */
/*******************/
- (IBAction) loadConstructs: (id) sender
  {
   [mainTerminal loadConstructs: self];
  }

/**************/
/* loadBatch: */
/**************/
- (IBAction) loadBatch: (id) sender
  {
   [mainTerminal loadBatch: self];
  }

/*****************/
/* setDirectory: */
/*****************/
- (IBAction) setDirectory: (id) sender
  {
   [mainTerminal setDirectory: self];
  }

/*********/
/* reset */
/*********/
- (IBAction) reset: (id) sender
  {
   [mainEnvironment doCommand: @"(reset)\n"];
  }

/*******/
/* run */
/*******/
- (IBAction) run: (id) sender
  {
   [mainEnvironment doCommand: @"(run)\n"];
  }

/**************/
/* haltRules: */
/**************/
- (IBAction) haltRules: (id) sender
  {
   EnvSetHaltRules([mainEnvironment environment],true);
  }

/******************/
/* haltExecution: */
/******************/
- (IBAction) haltExecution: (id) sender
  {
   /* Need to abort waitForChar */
   /* Need to abort batch */
   SetHaltCommandLoopBatch([mainEnvironment environment],true);
   EnvSetHaltExecution([mainEnvironment environment],true);
  }

/********************/
/* clearScrollback: */
/********************/
- (IBAction) clearScrollback: (id) sender
  {
   [mainTerminal clearScrollback: self];
  }

/************************/
/* showPreferencePanel: */
/************************/
- (IBAction) showPreferencePanel: (id) sender
  {
   if (! preferenceController)
     { preferenceController = [[PreferenceController alloc] init]; }
    
   [preferenceController showPanel];
  }

/*******************/
/* agendaBrowser: */
/*******************/
- (IBAction) agendaBrowser: (id) sender
  {
   CLIPSAgendaController *theController;
      
   theController = [[CLIPSAgendaController alloc] init];
   
   [self addAgendaController: theController];
      
   [theController showWindow: self];
  }

/************************/
/* factBrowser: */
/************************/
- (IBAction) factBrowser: (id) sender
  {
   CLIPSFactController *theController;
 
   theController = [[CLIPSFactController alloc] init];

   [self addFactController: theController];
   
   [theController showWindow: self];
  }

/****************************/
/* instanceBrowser: */
/****************************/
- (IBAction) instanceBrowser: (id) sender
  {
   CLIPSInstanceController *theController;
      
   theController = [[CLIPSInstanceController alloc] init]; 

   [self addInstanceController: theController];
      
   [theController showWindow: self];
  }

/***********************/
/* constructInspector: */
/***********************/
- (IBAction) constructInspector: (id) sender
  {
   if (! constructInspectorController)
     { constructInspectorController = [[CLIPSConstructInspectorController alloc] init]; }
    
   [constructInspectorController showPanel];
  }

/***************************************************************/
/* showCLIPSDocumentation: Opens the CLIPS Documentation Page. */
/***************************************************************/
- (IBAction) showCLIPSDocumentation: (id) sender
  {
   [[NSWorkspace sharedWorkspace] 
       openURL: [NSURL URLWithString: @"http://clipsrules.sourceforge.net/OnlineDocs.html"]];
  }

/*************************************************/
/* showCLIPSHomePage: Opens the CLIPS Home Page. */
/*************************************************/
- (IBAction) showCLIPSHomePage: (id) sender
  {
   [[NSWorkspace sharedWorkspace] 
       openURL: [NSURL URLWithString: @"http://clipsrules.sourceforge.net/"]];
  }

/*****************************************************/
/* showCLIPSExamples: Opens the CLIPS Examples Page. */
/*****************************************************/
- (IBAction) showCLIPSExamples: (id) sender
  {
   [[NSWorkspace sharedWorkspace] 
       openURL: [NSURL URLWithString: @"https://sourceforge.net/p/clipsrules/code/HEAD/tree/examples/"]];
  }

/*****************************************/
/* showCLIPSExpertSystemGroup: Opens the */
/*   CLIPS Expert System Group Web Page. */
/*****************************************/
- (IBAction) showCLIPSExpertSystemGroup: (id) sender
  {
   [[NSWorkspace sharedWorkspace] 
       openURL: [NSURL URLWithString: @"http://groups.google.com/group/CLIPSESG/"]];
  }

/********************************************/
/* showCLIPSSourceForgeForums: Opens the    */
/*   CLIPS SourceForge Discussion Web Page. */
/********************************************/
- (IBAction) showCLIPSSourceForgeDiscussion: (id) sender
  {
   [[NSWorkspace sharedWorkspace] 
       openURL: [NSURL URLWithString: @"http://sourceforge.net/p/clipsrules/discussion"]];
  }

/************************************/
/* showCLIPStackOverflow: Opens the */
/*   CLIPS Stack Overflow Page.     */
/************************************/
- (IBAction) showCLIPSStackOverflow: (id) sender
  {
   [[NSWorkspace sharedWorkspace] 
       openURL: [NSURL URLWithString: @"http://stackoverflow.com/questions/tagged/clips"]];
  }

/*%%%%%%%%%%%%%%%%%%*/
/* Delegate Methods */
/*%%%%%%%%%%%%%%%%%%*/

/**********************************/
/* applicationDidFinishLaunching: */
/**********************************/
- (void) applicationDidFinishLaunching: (NSNotification *) aNotification
  {
   mainEnvironment = [[CLIPSEnvironment alloc] init];
   mainTerminal = [[CLIPSTerminalController alloc] initWithEnvironment: mainEnvironment];
   
   [mainTerminal showWindow: self];
   [[mainTerminal window] makeKeyAndOrderFront: self];
  }

/***********************************************************/
/* applicationShouldOpenUntitledFile: This delegate method */
/*   is used to indicate that an untitled file should not  */
/*   be opened when the application is launched.           */
/***********************************************************/
- (BOOL) applicationShouldOpenUntitledFile: (NSApplication *) sender
  {
   return NO;
  }
        
/*******************************/
/* applicationShouldTerminate: */
/*******************************/
- (NSApplicationTerminateReply) applicationShouldTerminate: (NSApplication *) app
  {
   if (preferenceController != nil)
     { return [preferenceController reviewPreferencesBeforeQuitting]; }
     
   return NSTerminateNow;
  }
  
/*%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* Key-Value Coding Methods */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%*/
 
/****************************/
/* fileOpenLock: */
/****************************/
- (NSLock *) fileOpenLock
  {
   return fileOpenLock;
  }

@end
