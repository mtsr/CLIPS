package net.sf.clipsrules.jni.examples.ide;

import javax.swing.*; 
import javax.swing.border.*; 
import javax.swing.table.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.event.*;

import java.awt.*; 
import java.awt.event.*; 
import java.io.File;

import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;

import java.util.List;
import java.util.prefs.Preferences;
import java.util.Properties;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;

import java.util.HashMap;

import net.sf.clipsrules.jni.*;

// TBD
// Use lock for I/O
// argument error functions 
// external address user function
// call java method
// When environment destroyed, delete global context references

public class CLIPSIDE extends JFrame 
                   implements ActionListener, CommandExecutionListener,
                              MenuListener, InternalFrameListener
  {  
   private JDesktopPane ideDesktopPane;
            
   private DialogFrame dialogWindow;
   
   private static final String windowProperty = "windowProperty";
   private static final String menuItemProperty = "menuItemProperty";
   
   static final String clearAction = "Clear";
   static final String loadConstructsAction = "LoadConstructs";
   static final String loadBatchAction = "LoadBatch";
   static final String setDirectoryAction = "SetDirectory";
   static final String quitIDEAction = "QuitIDE";
   static final String resetAction = "Reset";
   static final String runAction = "Run";
   static final String stepAction = "Step";
   static final String haltRulesAction = "HaltRules";
   static final String haltExecutionAction = "HaltExecution";
   static final String clearScrollbackAction = "ClearScrollback";

   static final String agendaBrowserAction = "AgendaBrowser";
   static final String constructInspectorAction = "ConstructInspector";

   static final String clipsHomePageAction = "CLIPSHomePage";
   static final String onlineDocumentationAction = "OnlineDocumentation";
   static final String onlineExamplesAction = "OnlineExamples";
   static final String clipsExpertSystemGroupAction = "CLIPSExpertSystemGroup";
   static final String sourceForgeForumsAction = "SourceForgeForums";
   static final String stackOverflowQAAction = "StackOverflowQ&A";

   static final String selectWindowAction = "SelectWindow";

   static final String currentDirectoryPref = "currentDirectory";

   private File currentDirectory = null;
   
   private JMenu jmWindow;

   private JMenuItem jmiQuitIDE = null;

   private JMenuItem jmiCut = null;
   private JMenuItem jmiCopy = null;
   private JMenuItem jmiPaste = null;
   
   private JMenuItem jmiClear = null;
   private JMenuItem jmiLoadConstructs = null;
   private JMenuItem jmiLoadBatch = null;
   private JMenuItem jmiSetDirectory = null;
   private JMenuItem jmiReset = null;
   private JMenuItem jmiRun = null;
   private JMenuItem jmiHaltRules = null;
   private JMenuItem jmiHaltExecution = null;
   private JMenuItem jmiClearScrollback = null;

   private JMenuItem jmiAgendaBrowser = null;
   private JMenuItem jmiConstructInspector = null;
   
   private JMenuItem jmiCLIPSHomePage = null;
   private JMenuItem jmiOnlineDocumentation = null;   
   private JMenuItem jmiOnlineExamples = null;
   private JMenuItem jmiCLIPSExpertSystemGroup = null;
   private JMenuItem jmiSourceForgeForums = null;
   private JMenuItem jmiStackOverflowQA = null;
      
   private Preferences prefs = null;
   
   private List<AgendaBrowserFrame> agendaBrowsers = new ArrayList<AgendaBrowserFrame>();
   private ConstructInspectorFrame constructInspector = null;
   
   private int agendaChangeIndex = 0;
   private FocusStack focusStack;
   private HashMap<Focus,Agenda> agendaMap;

   private int windowStartX = 28;
   private int windowStartY = 28;
   private int windowCurrentX = -1;
   private int windowCurrentY = -1;
   private int windowXIncrement = 28;
   private int windowYIncrement = 28;

   /************/
   /* CLIPSIDE */
   /************/
   CLIPSIDE()
     {  
      super("CLIPS IDE");

      /*===================================*/
      /* Use cross platform look and feel. */
      /*===================================*/
      
      try  
        { UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName()); }
      catch (Exception e)
        { e.printStackTrace(); }
             
      /************************/
      /* Get the preferences. */
      /************************/
      
      prefs = Preferences.userNodeForPackage(net.sf.clipsrules.jni.examples.ide.CLIPSIDE.class);       
      String directoryPref = prefs.get(currentDirectoryPref,null);         

      /*==================================*/
      /* Determine the working directory. */
      /*==================================*/
      
      File workingDirectory;
      if (directoryPref == null)
        { workingDirectory = new File(System.getProperty("user.dir")); }
      else
        { 
         workingDirectory = new File(directoryPref); 
         if (! workingDirectory.exists())
           { workingDirectory = new File(System.getProperty("user.dir")); }
        }
      
      currentDirectory = workingDirectory.getAbsoluteFile();
            
      /*======================*/
      /* Create the menu bar. */
      /*======================*/

      createMenuBar();
      
      /*====================*/
      /* Display the frame. */
      /*====================*/
      
      ideDesktopPane = new JDesktopPane();
      ideDesktopPane.setDesktopManager(new BoundsDesktopManager());
      add(ideDesktopPane);
      
      setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      setLocation(50,50);
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      setSize((int) (screenSize.width * 0.80),(int) (screenSize.height * 0.80));
      setVisible(true);

      /*********/
      
      focusStack = new FocusStack();
      agendaMap = new HashMap<Focus,Agenda>();

      /*****************************/
      /* Create the dialog window. */
      /*****************************/
      
      createDialogWindow();
      dialogWindow.addCommandExecutionListener(this);
      
      /*==========================*/
      /* Add some user functions. */
      /*==========================*/

      addUserFunctions();
     }  
     
   /**********************/
   /* createDialogWindow */
   /**********************/  
   public void createDialogWindow()
     {
      dialogWindow = new DialogFrame(currentDirectory);
      dialogWindow.addInternalFrameListener(this);
      
      this.placeInternalFrame(dialogWindow);
      
      ideDesktopPane.add(dialogWindow);
      dialogWindow.setVisible(true);
     }
          
   /********/
   /* main */
   /********/  
   public static void main(String args[])
     {  
      CLIPSIDE ide = new CLIPSIDE();  
     } 
   
   /***************/
   /* fetchAgenda */
   /***************/
   private synchronized void fetchAgenda()
     {
      focusStack = dialogWindow.getEnvironment().getFocusStack();
      agendaMap = new HashMap<Focus,Agenda>();

      for (Iterator itr = focusStack.iterator(); itr.hasNext(); ) 
        { 
         Focus theFocus = (Focus) itr.next();
         Agenda theAgenda = dialogWindow.getEnvironment().getAgenda(theFocus);
         agendaMap.put(theFocus,theAgenda);
        }
     }

   /****************/
   /* assignAgenda */
   /****************/
   private synchronized void assignAgenda(
     AgendaBrowserFrame theBrowser)
     {
      theBrowser.assignData(focusStack,agendaMap);
     }

   /************************/
   /* updateAgendaBrowser: */
   /************************/
   private void updateAgendaBrowser(
     AgendaBrowserFrame theBrowser)
     {
      if (EventQueue.isDispatchThread())
        { 
         assignAgenda(theBrowser);
         return; 
        }
              
      try
        {
         SwingUtilities.invokeAndWait(
           new Runnable() 
             {  
              public void run() 
                { assignAgenda(theBrowser); }  
             });   
        }
      catch (Exception e) 
        { e.printStackTrace(); }
     }
  
   /****************************/
   /* updateAllAgendaBrowsers: */
   /****************************/
   private void updateAllAgendaBrowsers()
     {
      if (agendaBrowsers.size() == 0) return;
      
      fetchAgenda();
      
      for (Iterator itr = agendaBrowsers.iterator(); itr.hasNext(); ) 
        { 
         AgendaBrowserFrame theBrowser = (AgendaBrowserFrame) itr.next();
         updateAgendaBrowser(theBrowser);
        }
     }

   /*******************************/
   /* updateAgendaBrowserButtons: */
   /*******************************/
   private void updateAgendaBrowserButtons(
     boolean isExecuting)
     {
      if (agendaBrowsers.size() == 0) return;
            
      for (Iterator itr = agendaBrowsers.iterator(); itr.hasNext(); ) 
        { 
         AgendaBrowserFrame theBrowser = (AgendaBrowserFrame) itr.next();
         theBrowser.updateButtons(isExecuting);
        }
     }

   /*********************************/
   /* commandExecutionEventOccurred */
   /*********************************/  
   public void commandExecutionEventOccurred(
     CommandExecutionEvent theEvent)
     {    
      if (theEvent.getExecutionEvent().equals(CommandExecutionEvent.START_EVENT))
        { updateAgendaBrowserButtons(true); }
     
      if (theEvent.getExecutionEvent().equals(CommandExecutionEvent.FINISH_EVENT))
        { updateAgendaBrowserButtons(false); }
     
      if (theEvent.getExecutionEvent().equals(CommandExecutionEvent.PERIODIC_EVENT) ||
          theEvent.getExecutionEvent().equals(CommandExecutionEvent.FINISH_EVENT))
        {
         if (dialogWindow.getEnvironment().getAgendaChanged())
           {
            dialogWindow.getEnvironment().setAgendaChanged(false);
            updateAllAgendaBrowsers();
           } 
        }
     }  

   /*################*/
   /* Action Methods */
   /*################*/

   /*********************/
   /* onActionPerformed */
   /*********************/  
   public void onActionPerformed(
     ActionEvent ae) throws Exception 
     {     
      JInternalFrame theFrame = ideDesktopPane.getSelectedFrame();

      if (ae.getActionCommand().equals("Cut"))  
        { 
          if ((theFrame == null) || theFrame.isIcon())
            { /* Do Nothing */ }
          else if (theFrame instanceof DialogFrame)
            {
             DialogFrame theDialogFrame = (DialogFrame) theFrame;
             theDialogFrame.cut(); 
            }
        }
      else if (ae.getActionCommand().equals("Copy"))  
        { 
          if ((theFrame == null) || theFrame.isIcon())
            { /* Do Nothing */ }
          else if (theFrame instanceof DialogFrame)
            {
             DialogFrame theDialogFrame = (DialogFrame) theFrame;
             theDialogFrame.copy(); 
            }
        }
      else if (ae.getActionCommand().equals("Paste"))  
        { 
          if ((theFrame == null) || theFrame.isIcon())
            { /* Do Nothing */ }
          else if (theFrame instanceof DialogFrame)
            {
             DialogFrame theDialogFrame = (DialogFrame) theFrame;
             theDialogFrame.paste(); 
            }
        }
      else if (ae.getActionCommand().equals(clearAction))  
        { clear(); }
      else if (ae.getActionCommand().equals(loadConstructsAction))  
        { loadConstructs(); }
      else if (ae.getActionCommand().equals(loadBatchAction))  
        { loadBatch(); }
      else if (ae.getActionCommand().equals(setDirectoryAction))  
        { setDirectory(); }
      else if (ae.getActionCommand().equals(quitIDEAction))  
        { quitIDE(); }
      else if ((ae.getActionCommand().equals(resetAction)) ||
               (ae.getActionCommand().equals(AgendaBrowserFrame.RESET_ACTION))) 
        { reset(); }
      else if ((ae.getActionCommand().equals(runAction)) ||
               (ae.getActionCommand().equals(AgendaBrowserFrame.RUN_ACTION))) 
        { run(); }
      else if ((ae.getActionCommand().equals(stepAction)) ||
               (ae.getActionCommand().equals(AgendaBrowserFrame.STEP_ACTION))) 
        { step(); }
      else if ((ae.getActionCommand().equals(haltRulesAction)) ||
               (ae.getActionCommand().equals(AgendaBrowserFrame.HALT_RULES_ACTION))) 
        { haltRules(); }
      else if (ae.getActionCommand().equals(AgendaBrowserFrame.BROWSER_SELECTION_ACTION)) 
        { agendaBrowserSelection(ae); }
      else if (ae.getActionCommand().equals(haltExecutionAction))  
        { haltExecution(); }
      else if (ae.getActionCommand().equals(clearScrollbackAction))  
        { clearScrollback(); }
      else if (ae.getActionCommand().equals(agendaBrowserAction))  
        { agendaBrowser(); }
      else if (ae.getActionCommand().equals(constructInspectorAction))  
        { constructInspector(); }
      else if (ae.getActionCommand().equals(clipsHomePageAction))  
        { openCLIPSHomePage(); }
      else if (ae.getActionCommand().equals(onlineDocumentationAction))  
        { openOnlineDocumentation(); }
      else if (ae.getActionCommand().equals(onlineExamplesAction))  
        { openOnlineExamples(); }
      else if (ae.getActionCommand().equals(clipsExpertSystemGroupAction))  
        { openCLIPSExpertSystemGroup(); }
      else if (ae.getActionCommand().equals(sourceForgeForumsAction))  
        { openSourceForgeForums(); }
      else if (ae.getActionCommand().equals(stackOverflowQAAction))  
        { openStackOverflowQA(); }
      else if (ae.getActionCommand().equals(selectWindowAction))  
        { selectWindow(ae.getSource()); }
     }
  
   /******************/
   /* loadConstructs */
   /******************/  
   public void loadConstructs()
     {
      final JFileChooser fc = new JFileChooser();
      
      FileNameExtensionFilter filter 
         = new FileNameExtensionFilter("Constructs File","clp");
      fc.setFileFilter(filter);

      if (currentDirectory != null)
        { fc.setCurrentDirectory(currentDirectory); }

      fc.setApproveButtonText("Load");
      fc.setDialogTitle("Load Constructs");
      
      int returnVal = fc.showOpenDialog(this);
      
      if (returnVal != JFileChooser.APPROVE_OPTION) return;
      
      File file = fc.getSelectedFile();
            
      currentDirectory = fc.getCurrentDirectory();

      if (dialogWindow.setDirectory(currentDirectory))
        { 
         dialogWindow.replaceCommand("(load \"" + 
                                     file.getName() + 
                                     "\")\n");
         prefs.put(currentDirectoryPref,currentDirectory.getAbsolutePath());
        }
      else
        {
         dialogWindow.replaceCommand("(load \"" + 
                                     file.getAbsolutePath() + 
                                     "\")\n");
        }
     }
     
   /*************/
   /* loadBatch */
   /*************/  
   public void loadBatch()
     {
      final JFileChooser fc = new JFileChooser();

      FileNameExtensionFilter filter 
         = new FileNameExtensionFilter("Batch File","bat","tst");
      fc.setFileFilter(filter);

      if (currentDirectory != null)
        { fc.setCurrentDirectory(currentDirectory); }

      fc.setApproveButtonText("Load");
      fc.setDialogTitle("Load Batch");
      
      int returnVal = fc.showOpenDialog(this);
      
      if (returnVal != JFileChooser.APPROVE_OPTION) return;
      
      File file = fc.getSelectedFile();
            
      currentDirectory = fc.getCurrentDirectory();

      if (dialogWindow.setDirectory(currentDirectory))
        { 
         dialogWindow.replaceCommand("(batch \"" + 
                                     file.getName() + 
                                     "\")\n");
         prefs.put(currentDirectoryPref,currentDirectory.getAbsolutePath());
        }
      else
        {
         dialogWindow.replaceCommand("(batch \"" + 
                                     file.getAbsolutePath() + 
                                     "\")\n");
        }
     }

   /****************/
   /* setDirectory */
   /****************/  
   public void setDirectory()
     {
      final JFileChooser fc = new JFileChooser();

      if (currentDirectory != null)
        { fc.setCurrentDirectory(currentDirectory); }
        
      fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
      fc.setApproveButtonText("Set");
      fc.setDialogTitle("Set Directory");
      
      int returnVal = fc.showOpenDialog(this);
      
      if (returnVal != JFileChooser.APPROVE_OPTION) return;
      
      File file = fc.getSelectedFile();
            
      currentDirectory = file.getAbsoluteFile();
      
      if (dialogWindow.setDirectory(currentDirectory))
        { prefs.put(currentDirectoryPref,currentDirectory.getAbsolutePath()); }
     }

   /***********/
   /* quitIDE */
   /***********/  
   public void quitIDE()
     {
      System.exit(0);
     }

   /*********/
   /* clear */
   /*********/  
   public void clear()
     {
      dialogWindow.replaceCommand("(clear)\n");
     }
     
   /*********/
   /* reset */
   /*********/  
   public void reset()
     {
      dialogWindow.replaceCommand("(reset)\n");
     }

   /*******/
   /* run */
   /*******/  
   public void run()
     {
      dialogWindow.replaceCommand("(run)\n");
     }

   /********/
   /* step */
   /********/  
   public void step()
     {
      dialogWindow.replaceCommand("(run 1)\n");
     }

   /*************/
   /* haltRules */
   /*************/  
   public void haltRules()
     {
      dialogWindow.haltRules();
     }

   /*****************/
   /* haltExecution */
   /*****************/  
   public void haltExecution()
     {
      dialogWindow.haltExecution();
     }
     
   /*******************/
   /* clearScrollback */
   /*******************/  
   public void clearScrollback()
     {
      dialogWindow.clearScrollback();
     }
     
   /*****************/
   /* agendaBrowser */
   /*****************/  
   public void agendaBrowser()
     {
      AgendaBrowserFrame frame = new AgendaBrowserFrame();
      frame.addInternalFrameListener(this);
      frame.setActionTarget(this);
      agendaBrowsers.add(frame);
      
      frame.updateButtons(dialogWindow.isExecuting());
      
      this.placeInternalFrame(frame);
      
      ideDesktopPane.add(frame);

      frame.setVisible(true);
      
      if (! dialogWindow.isExecuting())
        { 
         if (agendaBrowsers.size() == 1)
           { fetchAgenda(); }
         assignAgenda(frame); 
        }      
     }

   /**********************/
   /* constructInspector */
   /**********************/  
   public void constructInspector()
     {
      String theText = "";
      
      if (constructInspector != null) return;

      JInternalFrame selectedPane = ideDesktopPane.getSelectedFrame();
      
      if (selectedPane instanceof AgendaBrowserFrame)
        { theText = agendaBrowserSelectionText((AgendaBrowserFrame) selectedPane); } 
      
      constructInspector = new ConstructInspectorFrame();

      constructInspector.setText(theText); 

      constructInspector.addInternalFrameListener(this);
             
      constructInspector.setLocation(10,10); // TBD Save Location?
      
      ideDesktopPane.add(constructInspector,JLayeredPane.PALETTE_LAYER);

      constructInspector.setVisible(true);
     }
     
   /*********************/
   /* openCLIPSHomePage */
   /*********************/  
   public void openCLIPSHomePage()
     {
      try
        { openWebpage(new URL("http://www.clipsrules.net")); }
      catch (Exception e)
        { e.printStackTrace(); }
     }
   
   /***************************/
   /* openOnlineDocumentation */
   /***************************/  
   public void openOnlineDocumentation()
     {
      try
        { openWebpage(new URL("http://www.clipsrules.net/?q=Documentation")); }
      catch (Exception e)
        { e.printStackTrace(); }
     }
     
   /**********************/
   /* openOnlineExamples */
   /**********************/  
   public void openOnlineExamples()
     {
      try
        { openWebpage(new URL("https://sourceforge.net/p/clipsrules/code/HEAD/tree/examples/")); }
      catch (Exception e)
        { e.printStackTrace(); }
     }
     
   /******************************/
   /* openCLIPSExpertSystemGroup */
   /******************************/  
   public void openCLIPSExpertSystemGroup()
     {
      try
        { openWebpage(new URL("http://groups.google.com/group/CLIPSESG/")); }
      catch (Exception e)
        { e.printStackTrace(); }
     }

   /*************************/
   /* openSourceForgeForums */
   /*************************/  
   public void openSourceForgeForums()
     {
      try
        { openWebpage(new URL("http://sourceforge.net/p/clipsrules/discussion")); }
      catch (Exception e)
        { e.printStackTrace(); }
     }

   /***********************/
   /* openStackOverflowQA */
   /***********************/  
   public void openStackOverflowQA()
     {
      try
        { openWebpage(new URL("http://stackoverflow.com/questions/tagged/clips")); }
      catch (Exception e)
        { e.printStackTrace(); }
     }

   /***************/
   /* openWebpage */
   /***************/  
    public static void openWebpage(URI uri) 
      {
       Desktop desktop = Desktop.isDesktopSupported() ? Desktop.getDesktop() : null;
       if (desktop != null && desktop.isSupported(Desktop.Action.BROWSE)) 
         {
          try
            { desktop.browse(uri); } 
          catch (Exception e)
            { e.printStackTrace(); }
         }
      }
      
   /******************************/
   /* agendaBrowserSelectionText */
   /******************************/  
   private String agendaBrowserSelectionText(
    AgendaBrowserFrame theFrame)
    {
     String ruleName = theFrame.selectedActivationRule();
     if (ruleName == null)
       { return ""; }
     else
       { return dialogWindow.getEnvironment().getDefruleText(ruleName); }
    }

   /**************************/
   /* agendaBrowserSelection */
   /**************************/  
   public void agendaBrowserSelection(
     ActionEvent ae)
     {
      if (constructInspector == null) return;
      
      AgendaBrowserFrame theFrame = (AgendaBrowserFrame) ae.getSource();
        
      constructInspector.setText(agendaBrowserSelectionText(theFrame));
     }

   /***************/
   /* openWebpage */
   /***************/  
   public static void openWebpage(URL url)
     {
      try
        { openWebpage(url.toURI()); } 
      catch (URISyntaxException e)
        { e.printStackTrace(); }
     }    

   /****************/
   /* selectWindow */
   /****************/  
   public void selectWindow(
     Object source)
     {
      JCheckBoxMenuItem jmiWindow = (JCheckBoxMenuItem) source;
      JInternalFrame theFrame = (JInternalFrame) jmiWindow.getClientProperty(windowProperty);

      if (theFrame.isSelected())
        {
         jmiWindow.setState(true);
         return;
        }

      try
        { theFrame.setSelected(true); }
      catch (Exception e) 
        { e.printStackTrace(); }
     }
   
   /**********************/
   /* placeInternalFrame */
   /**********************/  
   private void placeInternalFrame(
     JInternalFrame frame)
     {
      if (windowCurrentX == -1)
        {
         windowCurrentX = windowStartX;
         windowCurrentY = windowStartY;
         frame.setLocation(windowCurrentX,windowCurrentY);
         return;
        }
      
      windowCurrentX += windowXIncrement;
      windowCurrentY += windowYIncrement;

      if ((frame.getHeight() + windowCurrentY) > ideDesktopPane.getHeight())
        { windowCurrentY = windowStartY; }
      
      if ((frame.getWidth() + windowCurrentX) > ideDesktopPane.getWidth())
        { windowCurrentX = windowStartX; }

      frame.setLocation(windowCurrentX,windowCurrentY);
     }

   /*########################*/
   /* ActionListener Methods */
   /*########################*/

   /*******************/
   /* actionPerformed */
   /*******************/  
   public void actionPerformed(
     ActionEvent ae) 
     {
      try
        { onActionPerformed(ae); }
      catch (Exception e)
        { e.printStackTrace(); }
     }
     
   /*######################*/
   /* MenuListener Methods */
   /*######################*/
   
   /****************/
   /* menuCanceled */
   /****************/  
   public void menuCanceled(MenuEvent e)
     {
     }
   
   /****************/
   /* menuSelected */
   /****************/  
   public void menuSelected(MenuEvent e)
     {
      if (constructInspector == null)
        { jmiConstructInspector.setEnabled(true); }
      else
        { jmiConstructInspector.setEnabled(false); }
        
      if (dialogWindow.isExecuting())
        {
         jmiClear.setEnabled(false);
         jmiLoadConstructs.setEnabled(false);
         jmiLoadBatch.setEnabled(false);
         jmiSetDirectory.setEnabled(false);
         jmiReset.setEnabled(false);
         jmiRun.setEnabled(false);
         jmiHaltRules.setEnabled(true);
         jmiHaltExecution.setEnabled(true);
         jmiClearScrollback.setEnabled(false);
        }
      else
        {
         jmiClear.setEnabled(true);
         jmiLoadConstructs.setEnabled(true);
         jmiLoadBatch.setEnabled(true);
         jmiSetDirectory.setEnabled(true);
         jmiReset.setEnabled(true);
         jmiRun.setEnabled(true);
         jmiHaltRules.setEnabled(false);
         jmiHaltExecution.setEnabled(false);
         jmiClearScrollback.setEnabled(true);
        }
     
      JInternalFrame theFrame = ideDesktopPane.getSelectedFrame();
     
      if ((theFrame == null) || theFrame.isIcon())
        {
         jmiCut.setEnabled(false);
         jmiCopy.setEnabled(false);
         jmiPaste.setEnabled(false);
         return;
        }
       
      if (theFrame instanceof DialogFrame)
        {
         DialogFrame theDialogFrame = (DialogFrame) theFrame;
         if (theDialogFrame.hasCuttableSelection())
           { jmiCut.setEnabled(true); }
         else
           { jmiCut.setEnabled(false); }

         if (theDialogFrame.hasSelection())
           { jmiCopy.setEnabled(true); }
         else
           { jmiCopy.setEnabled(false); }

         if (theDialogFrame.hasPasteableSelection())
           { jmiPaste.setEnabled(true); }
         else
           { jmiPaste.setEnabled(false); }
        }
      else
        {
         jmiCut.setEnabled(false);
         jmiCopy.setEnabled(false);
         jmiPaste.setEnabled(false);
        }
     }
   
   /******************/
   /* menuDeselected */
   /******************/  
   public void menuDeselected(MenuEvent e)
     {
     }
          
   /*###############################*/
   /* InternalFrameListener Methods */
   /*###############################*/
     
   /************************/
   /* internalFrameClosing */
   /************************/
   public void internalFrameClosing(
     InternalFrameEvent e)
     {
     }

   /***********************/
   /* internalFrameClosed */
   /***********************/
   public void internalFrameClosed(
     InternalFrameEvent e) 
     {
      JInternalFrame theFrame = e.getInternalFrame();
      theFrame.removeInternalFrameListener(this);

      if (theFrame instanceof ConstructInspectorFrame)
        { 
         constructInspector = null;
         return; 
        }

      if (theFrame instanceof AgendaBrowserFrame)
        { agendaBrowsers.remove(theFrame); }

      JCheckBoxMenuItem jmiWindow = (JCheckBoxMenuItem) theFrame.getClientProperty(menuItemProperty);
      
      theFrame.putClientProperty(menuItemProperty,null);
      jmiWindow.putClientProperty(windowProperty,null);
      
      jmWindow.remove(jmiWindow);
     }

   /***********************/
   /* internalFrameOpened */
   /***********************/
   public void internalFrameOpened(
     InternalFrameEvent e)
     {
      JInternalFrame theFrame = e.getInternalFrame();
      
      if (theFrame instanceof ConstructInspectorFrame)
        { return; }

      JCheckBoxMenuItem jmiWindow = new JCheckBoxMenuItem(theFrame.getTitle());
      jmiWindow.setState(false);      
      jmiWindow.putClientProperty(windowProperty,theFrame);
      jmiWindow.addActionListener(this);
      jmiWindow.setActionCommand(selectWindowAction);
      jmWindow.add(jmiWindow);
      theFrame.putClientProperty(menuItemProperty,jmiWindow);
     }

   /**************************/
   /* internalFrameIconified */
   /**************************/
   public void internalFrameIconified(
     InternalFrameEvent e)
     {
      JInternalFrame theFrame = e.getInternalFrame();

      if (theFrame instanceof ConstructInspectorFrame)
        { return; }

      JCheckBoxMenuItem jmiWindow = (JCheckBoxMenuItem) theFrame.getClientProperty(menuItemProperty);
      jmiWindow.setState(false);
     }

   /****************************/
   /* internalFrameDeiconified */
   /****************************/
   public void internalFrameDeiconified(
     InternalFrameEvent e)
     {
      JInternalFrame theFrame = e.getInternalFrame();

      if (theFrame instanceof ConstructInspectorFrame)
        { return; }

      JCheckBoxMenuItem jmiWindow = (JCheckBoxMenuItem) theFrame.getClientProperty(menuItemProperty);
      jmiWindow.setState(true);
     }

   /**************************/
   /* internalFrameActivated */
   /**************************/
   public void internalFrameActivated(
     InternalFrameEvent e)
     {
      JInternalFrame theFrame = e.getInternalFrame();
      
      if (theFrame instanceof ConstructInspectorFrame)
        { return; }

      if ((constructInspector != null) && 
          (theFrame instanceof AgendaBrowserFrame))
        { 
         String theText = agendaBrowserSelectionText((AgendaBrowserFrame) theFrame); 
         constructInspector.setText(theText); 
        } 

      JCheckBoxMenuItem jmiWindow = (JCheckBoxMenuItem) theFrame.getClientProperty(menuItemProperty);
      jmiWindow.setState(true);
     }

   /****************************/
   /* internalFrameDeactivated */
   /****************************/
   public void internalFrameDeactivated(
     InternalFrameEvent e) 
     {
      JInternalFrame theFrame = e.getInternalFrame();

      if (theFrame instanceof ConstructInspectorFrame)
        { return; }

      JCheckBoxMenuItem jmiWindow = (JCheckBoxMenuItem) theFrame.getClientProperty(menuItemProperty);
      jmiWindow.setState(false);
     }
     
   /*#####################*/
   /* GUI Building Methods*/
   /*#####################*/
     
   /*****************/
   /* createMenuBar */
   /*****************/
   private void createMenuBar()
     {
      /*=================================================*/
      /* Get KeyStroke for copy/paste keyboard commands. */
      /*=================================================*/

      KeyStroke loadConstructs = KeyStroke.getKeyStroke(KeyEvent.VK_L,KeyEvent.CTRL_MASK);
      KeyStroke loadBatch = KeyStroke.getKeyStroke(KeyEvent.VK_L,KeyEvent.CTRL_MASK | KeyEvent.SHIFT_MASK);
      KeyStroke quitIDE = KeyStroke.getKeyStroke(KeyEvent.VK_Q,KeyEvent.CTRL_MASK);
      
      KeyStroke cut = KeyStroke.getKeyStroke(KeyEvent.VK_X,KeyEvent.CTRL_MASK);
      KeyStroke copy = KeyStroke.getKeyStroke(KeyEvent.VK_C,KeyEvent.CTRL_MASK);
      KeyStroke paste = KeyStroke.getKeyStroke(KeyEvent.VK_V,KeyEvent.CTRL_MASK);

      KeyStroke reset = KeyStroke.getKeyStroke(KeyEvent.VK_R,KeyEvent.CTRL_MASK);
      KeyStroke run = KeyStroke.getKeyStroke(KeyEvent.VK_R,KeyEvent.CTRL_MASK | KeyEvent.SHIFT_MASK);
      KeyStroke haltRules = KeyStroke.getKeyStroke(KeyEvent.VK_PERIOD,KeyEvent.CTRL_MASK);
      KeyStroke haltExecution = KeyStroke.getKeyStroke(KeyEvent.VK_PERIOD,KeyEvent.CTRL_MASK | KeyEvent.SHIFT_MASK);

      /*======================*/
      /* Create the menu bar. */
      /*======================*/
      
      JMenuBar jmb = new JMenuBar();

      /*===========*/
      /* File menu */
      /*===========*/
      
      JMenu jmFile = new JMenu("File");
      jmFile.addMenuListener(this);

      jmiQuitIDE = new JMenuItem("Quit CLIPS IDE");
      jmiQuitIDE.setActionCommand(quitIDEAction);
      jmiQuitIDE.setAccelerator(quitIDE);
      jmiQuitIDE.addActionListener(this);
      jmFile.add(jmiQuitIDE);

      jmb.add(jmFile);

      /*===========*/
      /* Edit menu */
      /*===========*/
         
      JMenu jmEdit = new JMenu("Edit");
      jmEdit.addMenuListener(this);
      
      jmiCut = new JMenuItem("Cut");
      jmiCut.addActionListener(this);
      jmiCut.setAccelerator(cut);
      jmEdit.add(jmiCut);

      jmiCopy = new JMenuItem("Copy");
      jmiCopy.addActionListener(this);
      jmiCopy.setAccelerator(copy);
      jmEdit.add(jmiCopy);

      jmiPaste = new JMenuItem("Paste");
      jmiPaste.addActionListener(this);      
      jmiPaste.setAccelerator(paste);      
      jmEdit.add(jmiPaste);
      
      jmb.add(jmEdit);

      /*==================*/
      /* Environment menu */
      /*==================*/
         
      JMenu jmEnvironment = new JMenu("Environment");
      jmEnvironment.addMenuListener(this);

      jmiClear = new JMenuItem("Clear");
      jmiClear.setActionCommand(clearAction);
      jmiClear.addActionListener(this);
      jmEnvironment.add(jmiClear);
 
      jmiLoadConstructs = new JMenuItem("Load Constructs...");
      jmiLoadConstructs.setActionCommand(loadConstructsAction);
      jmiLoadConstructs.setAccelerator(loadConstructs);
      jmiLoadConstructs.addActionListener(this);
      jmEnvironment.add(jmiLoadConstructs);

      jmiLoadBatch = new JMenuItem("Load Batch...");
      jmiLoadBatch.setActionCommand(loadBatchAction);
      jmiLoadBatch.setAccelerator(loadBatch);
      jmiLoadBatch.addActionListener(this);
      jmEnvironment.add(jmiLoadBatch);

      jmiSetDirectory = new JMenuItem("Set Directory...");
      jmiSetDirectory.setActionCommand(setDirectoryAction);
      jmiSetDirectory.addActionListener(this);
      jmEnvironment.add(jmiSetDirectory);

      jmEnvironment.addSeparator();

      jmiReset = new JMenuItem("Reset");
      jmiReset.setActionCommand(resetAction);
      jmiReset.setAccelerator(reset);
      jmiReset.addActionListener(this);
      jmEnvironment.add(jmiReset);

      jmiRun = new JMenuItem("Run"); 
      jmiRun.setActionCommand(runAction);
      jmiRun.setAccelerator(run);
      jmiRun.addActionListener(this);
      jmEnvironment.add(jmiRun);

      jmiHaltRules = new JMenuItem("Halt Rules"); 
      jmiHaltRules.setActionCommand(haltRulesAction);
      jmiHaltRules.setAccelerator(haltRules);
      jmiHaltRules.addActionListener(this);
      jmEnvironment.add(jmiHaltRules);

      jmiHaltExecution = new JMenuItem("Halt Execution"); 
      jmiHaltExecution.setActionCommand(haltExecutionAction);
      jmiHaltExecution.setAccelerator(haltExecution);
      jmiHaltExecution.addActionListener(this);
      jmEnvironment.add(jmiHaltExecution);

      jmEnvironment.addSeparator();
      
      jmiClearScrollback = new JMenuItem("Clear Scrollback");
      jmiClearScrollback.setActionCommand(clearScrollbackAction);
      jmiClearScrollback.addActionListener(this);
      jmEnvironment.add(jmiClearScrollback);
      
      jmb.add(jmEnvironment);

      /*============*/
      /* Debug menu */
      /*============*/
         
      JMenu jmDebug = new JMenu("Debug");
      jmDebug.addMenuListener(this);

      jmiAgendaBrowser = new JMenuItem("Agenda Browser"); 
      jmiAgendaBrowser.setActionCommand(agendaBrowserAction);
      jmiAgendaBrowser.addActionListener(this);
      jmDebug.add(jmiAgendaBrowser);

      jmiConstructInspector = new JMenuItem("Construct Inspector"); 
      jmiConstructInspector.setActionCommand(constructInspectorAction);
      jmiConstructInspector.addActionListener(this);
      jmDebug.add(jmiConstructInspector);

      jmb.add(jmDebug);
      
      /*=============*/
      /* Window menu */
      /*=============*/
         
      jmWindow = new JMenu("Window");
      jmWindow.addMenuListener(this);
      jmb.add(jmWindow);
      
      /*===========*/
      /* Help menu */
      /*===========*/
         
      JMenu jmHelp = new JMenu("Help");
      jmHelp.addMenuListener(this);
      
      jmiCLIPSHomePage = new JMenuItem("CLIPS Home Page");
      jmiCLIPSHomePage.setActionCommand(clipsHomePageAction);
      jmiCLIPSHomePage.addActionListener(this);
      jmHelp.add(jmiCLIPSHomePage);

      jmiOnlineDocumentation = new JMenuItem("Online Documentation");
      jmiOnlineDocumentation.setActionCommand(onlineDocumentationAction);
      jmiOnlineDocumentation.addActionListener(this);
      jmHelp.add(jmiOnlineDocumentation);

      jmiOnlineExamples = new JMenuItem("Online Examples");
      jmiOnlineExamples.setActionCommand(onlineExamplesAction);
      jmiOnlineExamples.addActionListener(this);
      jmHelp.add(jmiOnlineExamples);

      jmiCLIPSExpertSystemGroup = new JMenuItem("CLIPS Expert System Group");
      jmiCLIPSExpertSystemGroup.setActionCommand(clipsExpertSystemGroupAction);
      jmiCLIPSExpertSystemGroup.addActionListener(this);
      jmHelp.add(jmiCLIPSExpertSystemGroup);

      jmiSourceForgeForums = new JMenuItem("SourceForge Forums");
      jmiSourceForgeForums.setActionCommand(sourceForgeForumsAction);
      jmiSourceForgeForums.addActionListener(this);
      jmHelp.add(jmiSourceForgeForums);

      jmiStackOverflowQA = new JMenuItem("Stack Overflow Q&A");
      jmiStackOverflowQA.setActionCommand(stackOverflowQAAction);
      jmiStackOverflowQA.addActionListener(this);
      jmHelp.add(jmiStackOverflowQA);

      jmb.add(jmHelp);

      /*===================*/
      /* Add the menu bar. */
      /*===================*/
             
      this.setJMenuBar(jmb);
     }

   /*################*/
   /* User Functions */
   /*################*/
     
   /********************/
   /* addUserFunctions */
   /********************/
   
   public void addUserFunctions()
     {
      Environment theEnv = dialogWindow.getEnvironment();
      // (upcase "ßuzäöü")
      theEnv.removeUserFunction("upcase");
      theEnv.addUserFunction("upcase","11j",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  LexemeValue rv = (LexemeValue) arguments.get(0);
                                   
                  String urv = rv.getValue().toUpperCase();
                  if (rv.isString())
                    { return new StringValue(urv); }
                  else if (rv.isSymbol())
                    { return new SymbolValue(urv); }
                  else if (rv.isInstanceName())
                    { return new InstanceNameValue(urv); }
                                                             
                  return null;
                 }
              });
                               
      theEnv.removeUserFunction("lowcase");
      theEnv.addUserFunction("lowcase","11j",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  LexemeValue rv = (LexemeValue) arguments.get(0);
                                   
                  String lrv = rv.getValue().toLowerCase();
                  if (rv.isString())
                    { return new StringValue(lrv); }
                  else if (rv.isSymbol())
                    { return new SymbolValue(lrv); }
                  else if (rv.isInstanceName())
                    { return new InstanceNameValue(lrv); }
                                                             
                  return null;
                 }
              });

      theEnv.addUserFunction("cbrt","11n",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  NumberValue rv = (NumberValue) arguments.get(0);
                  return new FloatValue(Math.cbrt(rv.doubleValue()));
                 }
              });

       theEnv.addUserFunction("get-year","00",
             new UserFunction()
               {
                public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                  { 
                   Calendar theCalendar = new GregorianCalendar();
                   return new IntegerValue(theCalendar.get(theCalendar.YEAR));
                  }
               });
      
      theEnv.addUserFunction("get-properties","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  List<PrimitiveValue> values = new ArrayList<PrimitiveValue>();

                  Properties props = System.getProperties();
                  for (String key : props .stringPropertyNames())
                    { values.add(new SymbolValue(key)); }

                  return new MultifieldValue(values);
                 }
              });
      
      theEnv.addUserFunction("hello","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  theEnv.println("Hello World!");
                  return null;
                 }
              });

      theEnv.addUserFunction("clear-window","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  dialogWindow.clearScrollback();
                  return null;
                 }
              });

      theEnv.addUserFunction("make-instoid","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  try
                    {
                     return theEnv.eval("(instance-address (make-instance [hello] of INITIAL-OBJECT))");
                    }
                  catch (Exception e)
                    { return new SymbolValue("FALSE"); }
                 }
              });

      theEnv.addUserFunction("make-factoid","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  try
                    {
                     return theEnv.eval("(assert (hello world))");
                    }
                  catch (Exception e)
                    { return new SymbolValue("FALSE"); }
                 }
              });
     }
  }