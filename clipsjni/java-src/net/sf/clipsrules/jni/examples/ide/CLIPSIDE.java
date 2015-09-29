package net.sf.clipsrules.jni.examples.ide;

import javax.swing.*; 
import javax.swing.border.*; 
import javax.swing.table.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.event.*;

import java.awt.*; 
import java.awt.event.*; 
import java.io.File;

import java.util.List;
import java.util.Properties;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;

import net.sf.clipsrules.jni.*;

// TBD
// Use lock for I/O
// argument error functions 
// external address user function
// call java method
// When environment destroyed, delete global context references
// Refactor JNI code

public class CLIPSIDE implements ActionListener, MenuListener, 
                                 CommandExecutionListener
  {  
   private JFrame ideFrame;
         
   private Environment clips;
   
   private CommandPromptTextArea commandTextArea;
   
   private JLabel currentDirectoryLabel;
   
   static final String loadConstructsAction = "LoadConstructs";
   static final String loadBatchAction = "LoadBatch";
   static final String setDirectoryAction = "SetDirectory";
   static final String quitIDEAction = "QuitIDE";
   static final String resetAction = "Reset";
   static final String runAction = "Run";
   static final String haltRulesAction = "HaltRules";
   static final String haltExecutionAction = "HaltExecution";
   static final String pauseAction = "Pause";
   
   private File currentDirectory = null;

   private JMenuItem jmiLoadConstructs = null;
   private JMenuItem jmiLoadBatch = null;
   private JMenuItem jmiSetDirectory = null;
   private JMenuItem jmiQuitIDE = null;

   private JMenuItem jmiCut = null;
   private JMenuItem jmiCopy = null;
   private JMenuItem jmiPaste = null;
   
   private JMenuItem jmiReset = null;
   private JMenuItem jmiRun = null;
   private JMenuItem jmiHaltRules = null;
   private JMenuItem jmiHaltExecution = null;
   
   private JToggleButton pauseButton = null;

   /************/
   /* CLIPSIDE */
   /************/
   CLIPSIDE()
     {  
      /*===================================*/
      /* Create a new JFrame container and */
      /* assign a layout manager to it.    */
      /*===================================*/

      ideFrame = new JFrame("CLIPS IDE");          
      ideFrame.getContentPane().setLayout(new BoxLayout(ideFrame.getContentPane(),BoxLayout.Y_AXIS));
    
      /*=================================*/
      /* Give the frame an initial size. */
      /*=================================*/
     
      ideFrame.setSize(800,600);  
      ideFrame.setMinimumSize(new Dimension(550,250));
  
      /*=============================================================*/
      /* Terminate the program when the user closes the application. */
      /*=============================================================*/
     
      ideFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
 
      /*===============================*/
      /* Create the clips environment. */
      /*===============================*/
      
      clips = new Environment();

      /*==========================*/
      /* Add some user functions. */
      /*==========================*/

      // (upcase "ßuzäöü")
      clips.removeUserFunction("upcase");
      clips.addUserFunction("upcase","11j",
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
                               
      clips.removeUserFunction("lowcase");
      clips.addUserFunction("lowcase","11j",
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

      clips.addUserFunction("cbrt","11n",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  NumberValue rv = (NumberValue) arguments.get(0);
                  return new FloatValue(Math.cbrt(rv.doubleValue()));
                 }
              });

       clips.addUserFunction("get-year","00",
             new UserFunction()
               {
                public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                  { 
                   Calendar theCalendar = new GregorianCalendar();
                   return new IntegerValue(theCalendar.get(theCalendar.YEAR));
                  }
               });
      
      clips.addUserFunction("get-properties","00",
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
      
      clips.addUserFunction("hello","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  clips.println("Hello World!");
                  return null;
                 }
              });

      clips.addUserFunction("clear-window","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  commandTextArea.clear();
                  return null;
                 }
              });

      clips.addUserFunction("make-instoid","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  try
                    {
                     return clips.eval("(instance-address (make-instance [hello] of INITIAL-OBJECT))");
                    }
                  catch (Exception e)
                    { return new SymbolValue("FALSE"); }
                 }
              });

      clips.addUserFunction("make-factoid","00",
            new UserFunction()
              {
               public PrimitiveValue evaluate(List<PrimitiveValue> arguments)
                 {
                  try
                    {
                     return clips.eval("(assert (hello world))");
                    }
                  catch (Exception e)
                    { return new SymbolValue("FALSE"); }
                 }
              });
                                     
      /*==================================*/
      /* Determine the working directory. */
      /*==================================*/
      
      File workingDirectory = new File(System.getProperty("user.dir")); 
      currentDirectory = workingDirectory.getAbsoluteFile();
      
      int dirChanged = clips.changeDirectory(currentDirectory.getAbsolutePath());

      /*==========================*/
      /* Create the status panel. */
      /*==========================*/
      
      JPanel statusPanel = new JPanel(); 
      statusPanel.setPreferredSize(new Dimension(800,40));
      
      currentDirectoryLabel = new JLabel("Dir: ");
      if (dirChanged == 0)
        { currentDirectoryLabel.setText("Dir: " + currentDirectory.getAbsolutePath()); }
      else
        { currentDirectoryLabel.setText("Dir: "); }
 
      statusPanel.add(currentDirectoryLabel);
      
      pauseButton = new JToggleButton("Pause");
      pauseButton.setEnabled(false);
      pauseButton.setActionCommand(pauseAction);
      pauseButton.addActionListener(this);
      statusPanel.add(pauseButton);
      
      GroupLayout layout = new GroupLayout(statusPanel);
      statusPanel.setLayout(layout);
      layout.setAutoCreateGaps(true);
      layout.setAutoCreateContainerGaps(true);
      
      layout.setHorizontalGroup(layout.createSequentialGroup()
                                      .addComponent(currentDirectoryLabel)
                                      .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                      .addComponent(pauseButton));
                                      
      layout.setVerticalGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                                    .addComponent(currentDirectoryLabel)
                                    .addComponent(pauseButton));

      ideFrame.getContentPane().add(statusPanel); 

      /*=============================*/
      /* Create the text field area. */
      /*=============================*/
 
      try
        { 
         commandTextArea = new CommandPromptTextArea(clips); 
         commandTextArea.addCommandExecutionListener(this);
        }
      catch (Exception e)
        { 
         e.printStackTrace();
         return;
        }       
      
      /*=======================================*/
      /* Put the text area into a scroll pane. */
      /*=======================================*/

      JScrollPane commandPane = new JScrollPane(commandTextArea);
      commandPane.setPreferredSize(new Dimension(800,560));
      commandPane.setViewportBorder(BorderFactory.createEmptyBorder(0,0,2,0));
      
      /*========================================*/
      /* Add the scroll pane to the main frame. */
      /*========================================*/
      
      ideFrame.getContentPane().add(commandPane); 
            
      /*=================================================*/
      /* Get KeyStroke for copy/paste keyboard commands. */
      /*=================================================*/

      KeyStroke loadConstructs = KeyStroke.getKeyStroke(KeyEvent.VK_L,KeyEvent.CTRL_MASK);
      KeyStroke loadBatch = KeyStroke.getKeyStroke(KeyEvent.VK_L,KeyEvent.CTRL_MASK | KeyEvent.SHIFT_MASK);
      KeyStroke quitIDE = KeyStroke.getKeyStroke(KeyEvent.VK_Q,KeyEvent.CTRL_MASK);
      
      KeyStroke cut = KeyStroke.getKeyStroke(KeyEvent.VK_X,KeyEvent.CTRL_MASK);
      KeyStroke copy = KeyStroke.getKeyStroke(KeyEvent.VK_C,KeyEvent.CTRL_MASK);
      KeyStroke paste = KeyStroke.getKeyStroke(KeyEvent.VK_V,KeyEvent.CTRL_MASK);

      KeyStroke reset = KeyStroke.getKeyStroke(KeyEvent.VK_E,KeyEvent.CTRL_MASK);
      KeyStroke run = KeyStroke.getKeyStroke(KeyEvent.VK_R,KeyEvent.CTRL_MASK);
      KeyStroke haltRules = KeyStroke.getKeyStroke(KeyEvent.VK_PERIOD,KeyEvent.CTRL_MASK);
      KeyStroke haltExecution = KeyStroke.getKeyStroke(KeyEvent.VK_PERIOD,KeyEvent.CTRL_MASK | KeyEvent.SHIFT_MASK);

      /*======================================================*/
      /* Override copy/paste for the CommandPromptTextArea so */
      /* that we can later define our own menu accelerators.  */
      /*======================================================*/

      InputMap map = commandTextArea.getInputMap();
      map.put(cut,"none");
      map.put(copy,"none");
      map.put(paste,"none");
      map.put(reset,"none");

      /*======================*/
      /* Create the menu bar. */
      /*======================*/
      
      JMenuBar jmb = new JMenuBar();

      /*===========*/
      /* File menu */
      /*===========*/
      
      JMenu jmFile = new JMenu("File");
      jmFile.addMenuListener(this);

      jmiLoadConstructs = new JMenuItem("Load Constructs...");
      jmiLoadConstructs.setActionCommand(loadConstructsAction);
      jmiLoadConstructs.setAccelerator(loadConstructs);
      jmiLoadConstructs.addActionListener(this);
      jmFile.add(jmiLoadConstructs);

      jmiLoadBatch = new JMenuItem("Load Batch...");
      jmiLoadBatch.setActionCommand(loadBatchAction);
      jmiLoadBatch.setAccelerator(loadBatch);
      jmiLoadBatch.addActionListener(this);
      jmFile.add(jmiLoadBatch);

      jmiSetDirectory = new JMenuItem("Set Directory...");
      jmiSetDirectory.setActionCommand(setDirectoryAction);
      jmiSetDirectory.addActionListener(this);
      jmFile.add(jmiSetDirectory);

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

      /*================*/
      /* Execution menu */
      /*================*/
         
      JMenu jmExecution = new JMenu("Execution");
      jmExecution.addMenuListener(this);
      
      jmiReset = new JMenuItem("Reset");
      jmiReset.setActionCommand(resetAction);
      jmiReset.setAccelerator(reset);
      jmiReset.addActionListener(this);
      jmExecution.add(jmiReset);

      jmiRun = new JMenuItem("Run"); 
      jmiRun.setActionCommand(runAction);
      jmiRun.setAccelerator(run);
      jmiRun.addActionListener(this);
      jmExecution.add(jmiRun);

      jmiHaltRules = new JMenuItem("Halt Rules"); 
      jmiHaltRules.setActionCommand(haltRulesAction);
      jmiHaltRules.setAccelerator(haltRules);
      jmiHaltRules.addActionListener(this);
      jmExecution.add(jmiHaltRules);

      jmiHaltExecution = new JMenuItem("Halt Execution"); 
      jmiHaltExecution.setActionCommand(haltExecutionAction);
      jmiHaltExecution.setAccelerator(haltExecution);
      jmiHaltExecution.addActionListener(this);
      jmExecution.add(jmiHaltExecution);
      
      jmb.add(jmExecution);

      /*===================*/
      /* Add the menu bar. */
      /*===================*/
             
      ideFrame.setJMenuBar(jmb);
      
      /*====================*/
      /* Display the frame. */
      /*====================*/

      ideFrame.pack();
      ideFrame.setVisible(true);  
     }  
          
   /********/
   /* main */
   /********/  
   public static void main(String args[])
     {  
      /*===================================================*/
      /* Create the frame on the event dispatching thread. */
      /*===================================================*/
      
      SwingUtilities.invokeLater(
        new Runnable() 
          {  
           public void run() { new CLIPSIDE(); }  
          });   
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
      if (ae.getActionCommand().equals("Cut"))  
        { commandTextArea.cut(); }
      else if (ae.getActionCommand().equals("Copy"))  
        { commandTextArea.copy(); }
      else if (ae.getActionCommand().equals("Paste"))  
        { commandTextArea.paste(); }
      else if (ae.getActionCommand().equals(loadConstructsAction))  
        { loadConstructs(); }
      else if (ae.getActionCommand().equals(loadBatchAction))  
        { loadBatch(); }
      else if (ae.getActionCommand().equals(setDirectoryAction))  
        { setDirectory(); }
      else if (ae.getActionCommand().equals(quitIDEAction))  
        { quitIDE(); }
      else if (ae.getActionCommand().equals(resetAction))  
        { reset(); }
      else if (ae.getActionCommand().equals(runAction))  
        { run(); }
      else if (ae.getActionCommand().equals(haltRulesAction))  
        { haltRules(); }
      else if (ae.getActionCommand().equals(haltExecutionAction))  
        { haltExecution(); }
      else if (ae.getActionCommand().equals(pauseAction))  
        { pause(); }
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
      
      int returnVal = fc.showOpenDialog(ideFrame);
      
      if (returnVal != JFileChooser.APPROVE_OPTION) return;
      
      File file = fc.getSelectedFile();
            
      currentDirectory = fc.getCurrentDirectory();

      int dirChanged = clips.changeDirectory(currentDirectory.getAbsolutePath());

      if (dirChanged == 0)
        { 
         commandTextArea.replaceCommand("(load \"" + 
                                        file.getName() + 
                                        "\")\n");
         currentDirectoryLabel.setText("Dir: " + currentDirectory.getAbsolutePath());
        }
      else
        {
         commandTextArea.replaceCommand("(load \"" + 
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
      
      int returnVal = fc.showOpenDialog(ideFrame);
      
      if (returnVal != JFileChooser.APPROVE_OPTION) return;
      
      File file = fc.getSelectedFile();
            
      currentDirectory = fc.getCurrentDirectory();

      int dirChanged = clips.changeDirectory(currentDirectory.getAbsolutePath());

      if (dirChanged == 0)
        { 
         commandTextArea.replaceCommand("(batch \"" + 
                                        file.getName() + 
                                        "\")\n");
         currentDirectoryLabel.setText("Dir: " + currentDirectory.getAbsolutePath());
        }
      else
        {
         commandTextArea.replaceCommand("(batch \"" + 
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
      
      int returnVal = fc.showOpenDialog(ideFrame);
      
      if (returnVal != JFileChooser.APPROVE_OPTION) return;
      
      File file = fc.getSelectedFile();
            
      currentDirectory = file.getAbsoluteFile();
      
      int dirChanged = clips.changeDirectory(currentDirectory.getAbsolutePath());
      
      if (dirChanged == 0)
        { currentDirectoryLabel.setText("Dir: " + currentDirectory.getAbsolutePath()); }
     }

   /***********/
   /* quitIDE */
   /***********/  
   public void quitIDE()
     {
      System.exit(0);
     }
     
   /*********/
   /* reset */
   /*********/  
   public void reset()
     {
      commandTextArea.replaceCommand("(reset)\n");
     }

   /*******/
   /* run */
   /*******/  
   public void run()
     {
      commandTextArea.replaceCommand("(run)\n");
     }

   /*************/
   /* haltRules */
   /*************/  
   public void haltRules()
     {
      clips.setHaltRules(true);
     }

   /*****************/
   /* haltExecution */
   /*****************/  
   public void haltExecution()
     {
      clips.setHaltExecution(true);
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
      if (commandTextArea.hasCuttableSelection())
        { jmiCut.setEnabled(true); }
      else
        { jmiCut.setEnabled(false); }

      if (commandTextArea.hasSelection())
        { jmiCopy.setEnabled(true); }
      else
        { jmiCopy.setEnabled(false); }

      if (commandTextArea.hasPasteableSelection())
        { jmiPaste.setEnabled(true); }
      else
        { jmiPaste.setEnabled(false); }
        
      if (commandTextArea.getExecuting())
        {
         jmiLoadConstructs.setEnabled(false);
         jmiLoadBatch.setEnabled(false);
         jmiSetDirectory.setEnabled(false);
         jmiReset.setEnabled(false);
         jmiRun.setEnabled(false);
         jmiHaltRules.setEnabled(true);
         jmiHaltExecution.setEnabled(true);
        }
      else
        {
         jmiLoadConstructs.setEnabled(true);
         jmiLoadBatch.setEnabled(true);
         jmiSetDirectory.setEnabled(true);
         jmiReset.setEnabled(true);
         jmiRun.setEnabled(true);
         jmiHaltRules.setEnabled(false);
         jmiHaltExecution.setEnabled(false);
        }
     }
   
   /******************/
   /* menuDeselected */
   /******************/  
   public void menuDeselected(MenuEvent e)
     {
     }
     
   /*##################################*/
   /* CommandExecutionListener Methods */
   /*##################################*/
   
   /*********/
   /* pause */
   /*********/  
   public void pause()
     {
      if (commandTextArea.isPaused())
        { commandTextArea.setPaused(false); }
      else
        { commandTextArea.setPaused(true); }
     }

   /*********************/
   /* updatePauseButton */
   /*********************/  
   public void updatePauseButton(boolean value)
     {
      if (EventQueue.isDispatchThread())
        { 
         pauseButton.setEnabled(value);
         return;
        }
      try
        {
         SwingUtilities.invokeAndWait(
           new Runnable() 
             {  
              public void run() 
                 { pauseButton.setEnabled(value);  }  
             });   
        }
      catch (Exception e) 
        { e.printStackTrace(); }
     }
     
   /*********************************/
   /* commandExecutionEventOccurred */
   /*********************************/  
   public void commandExecutionEventOccurred(
     CommandExecutionEvent theEvent)
     {    
      if (theEvent.getExecutionEvent().equals(CommandExecutionEvent.START_EVENT))
        { updatePauseButton(true); }
      else if (theEvent.getExecutionEvent().equals(CommandExecutionEvent.FINISH_EVENT))
        { updatePauseButton(false);  }
     }  
  }