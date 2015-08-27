package net.sf.clipsrules.jni.examples.ide;

import javax.swing.*; 
import javax.swing.border.*; 
import javax.swing.table.*;
import java.awt.*; 
import java.awt.event.*; 
 
import net.sf.clipsrules.jni.*;

class CLIPSIDE implements ActionListener
  {  
   private JFrame ideFrame;
         
   private Environment clips;
   
   private CommandPromptTextArea commandTextArea;

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
 
      /*=============================*/
      /* Create the text field area. */
      /*=============================*/
 
      clips = new Environment();
      try
        {
         commandTextArea = new CommandPromptTextArea(clips); 
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
      commandPane.setPreferredSize(new Dimension(800,550));
      commandPane.setViewportBorder(BorderFactory.createEmptyBorder(0,0,2,0));
      
      /*========================================*/
      /* Add the scroll pane to the main frame. */
      /*========================================*/
      
      ideFrame.getContentPane().add(commandPane); 
            
      /*=================================================*/
      /* Get KeyStroke for copy/paste keyboard commands. */
      /*=================================================*/

      KeyStroke copy = KeyStroke.getKeyStroke(KeyEvent.VK_C,KeyEvent.CTRL_MASK);
      KeyStroke paste = KeyStroke.getKeyStroke(KeyEvent.VK_V,KeyEvent.CTRL_MASK);

      /*==========================================================*/
      /* Override copy/paste for the JTextAreaCommandPromptRouter */
      /* so that we can later define our own menu accelerators.   */
      /*==========================================================*/

      String actionKey = "none";
      InputMap map = commandTextArea.getInputMap();
      map.put(copy,actionKey);
      map.put(paste,actionKey);

      /*======================*/
      /* Create the menu bar. */
      /*======================*/
      
      JMenuBar jmb = new JMenuBar();
      
      JMenu jmEdit = new JMenu("Edit");
      JMenuItem jmiCopy = new JMenuItem("Copy",KeyEvent.VK_C);
      JMenuItem jmiPaste = new JMenuItem("Paste",KeyEvent.VK_V);
      
      jmiCopy.setAccelerator(copy);
      jmiPaste.setAccelerator(paste);
      
      jmiCopy.addActionListener(this);
      jmiPaste.addActionListener(this);
      
      jmEdit.add(jmiCopy);
      jmEdit.add(jmiPaste);
      jmb.add(jmEdit);
      
      ideFrame.setJMenuBar(jmb);
      
      /*====================*/
      /* Display the frame. */
      /*====================*/

      ideFrame.pack();
      ideFrame.setVisible(true);  
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

   /*********************/
   /* onActionPerformed */
   /*********************/  
   public void onActionPerformed(
     ActionEvent ae) throws Exception 
     {      
      if (ae.getActionCommand().equals("Copy"))  
        { commandTextArea.copy(); }
      else if (ae.getActionCommand().equals("Paste"))  
        { commandTextArea.paste(); }
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
  }