package net.sf.clipsrules.jni.examples.ide;

import javax.swing.*; 
import javax.swing.border.*; 
import javax.swing.table.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.event.*;

import java.awt.*; 
import java.awt.event.*; 
import java.io.File;

import net.sf.clipsrules.jni.*;

// TBD
// Disable menu commands when executing
// Pause
// Halt Execution
// Prompt to close

class CLIPSIDE implements ActionListener, MenuListener
  {  
   private JFrame ideFrame;
         
   private Environment clips;
   
   private CommandPromptTextArea commandTextArea;
   
   private JLabel currentDirectoryLabel;
   
   static final String loadConstructsAction = "LoadConstructs";
   static final String loadBatchAction = "LoadBatch";
   static final String setDirectoryAction = "SetDirectory";
   
   private File currentDirectory = null;

   private JMenuItem jmiLoadConstructs = null;
   private JMenuItem jmiLoadBatch = null;
   private JMenuItem jmiSetDirectory = null;

   private JMenuItem jmiCut = null;
   private JMenuItem jmiCopy = null;
   private JMenuItem jmiPaste = null;

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
  
      /*=============================================================*/
      /* Terminate the program when the user closes the application. */
      /*=============================================================*/
     
      ideFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);  
 
      /*===============================*/
      /* Create the clips environment. */
      /*===============================*/
      
      clips = new Environment();

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
      
      JButton pauseButton = new JButton("Pause");
      pauseButton.setEnabled(false);
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
        { commandTextArea = new CommandPromptTextArea(clips); }
      catch (Exception e)
        { 
         e.printStackTrace();
         return;
        }       
      
      /*=======================================*/
      /* Put the text area into a scroll pane. */
      /*=======================================*/

      JScrollPane commandPane = new JScrollPane(commandTextArea);
      commandPane.setPreferredSize(new Dimension(800,510));
      commandPane.setViewportBorder(BorderFactory.createEmptyBorder(0,0,2,0));
      
      /*========================================*/
      /* Add the scroll pane to the main frame. */
      /*========================================*/
      
      ideFrame.getContentPane().add(commandPane); 
            
      /*=================================================*/
      /* Get KeyStroke for copy/paste keyboard commands. */
      /*=================================================*/

      KeyStroke cut = KeyStroke.getKeyStroke(KeyEvent.VK_X,KeyEvent.CTRL_MASK);
      KeyStroke copy = KeyStroke.getKeyStroke(KeyEvent.VK_C,KeyEvent.CTRL_MASK);
      KeyStroke paste = KeyStroke.getKeyStroke(KeyEvent.VK_V,KeyEvent.CTRL_MASK);
      KeyStroke loadConstructs = KeyStroke.getKeyStroke(KeyEvent.VK_L,KeyEvent.CTRL_MASK);

      /*======================================================*/
      /* Override copy/paste for the CommandPromptTextArea so */
      /* that we can later define our own menu accelerators.  */
      /*======================================================*/

      InputMap map = commandTextArea.getInputMap();
      map.put(cut,"none");
      map.put(copy,"none");
      map.put(paste,"none");

      /*======================*/
      /* Create the menu bar. */
      /*======================*/
      
      JMenuBar jmb = new JMenuBar();

      /*===========*/
      /* File menu */
      /*===========*/
      
      JMenu jmFile = new JMenu("File");
      jmFile.addMenuListener(this);

      jmiLoadConstructs = new JMenuItem("Load Constructs...",KeyEvent.VK_L);
      jmiLoadConstructs.setActionCommand(loadConstructsAction);
      jmiLoadConstructs.setAccelerator(loadConstructs);
      jmiLoadConstructs.addActionListener(this);
      jmFile.add(jmiLoadConstructs);

      jmiLoadBatch = new JMenuItem("Load Batch...");
      jmiLoadBatch.setActionCommand(loadBatchAction);
      jmiLoadBatch.addActionListener(this);
      jmFile.add(jmiLoadBatch);

      jmiSetDirectory = new JMenuItem("Set Directory...");
      jmiSetDirectory.setActionCommand(setDirectoryAction);
      jmiSetDirectory.addActionListener(this);
      jmFile.add(jmiSetDirectory);

      jmb.add(jmFile);

      /*===========*/
      /* Edit menu */
      /*===========*/
         
      JMenu jmEdit = new JMenu("Edit");
      jmEdit.addMenuListener(this);
      
      jmiCut = new JMenuItem("Cut",KeyEvent.VK_X);
      jmiCut.addActionListener(this);
      jmiCut.setAccelerator(cut);
      jmEdit.add(jmiCut);

      jmiCopy = new JMenuItem("Copy",KeyEvent.VK_C);
      jmiCopy.addActionListener(this);
      jmiCopy.setAccelerator(copy);
      jmEdit.add(jmiCopy);

      jmiPaste = new JMenuItem("Paste",KeyEvent.VK_V);
      jmiPaste.addActionListener(this);      
      jmiPaste.setAccelerator(paste);      
      jmEdit.add(jmiPaste);
      
      jmb.add(jmEdit);
      
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
        }
      else
        {
         jmiLoadConstructs.setEnabled(true);
         jmiLoadBatch.setEnabled(true);
         jmiSetDirectory.setEnabled(true);
        }
     }
   
   /******************/
   /* menuDeselected */
   /******************/  
   public void menuDeselected(MenuEvent e)
     {
     }
  }