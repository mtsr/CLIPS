package net.sf.clipsrules.jni;

import javax.swing.*; 
import javax.swing.border.*; 
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.BorderFactory;

import java.awt.*; 
import java.awt.event.*; 
import java.awt.dnd.*;
import java.awt.datatransfer.*;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.lang.Byte;

public class RouterTextArea extends JTextArea
                            implements Router, KeyListener, CaretListener,
                                       FocusListener, DropTargetListener
  {   
   protected Environment clips;
   
   private static int TextAreaRouterNameIndex = 0;
   private String routerName;

   private boolean charNeeded = false;
   private List<Byte> charList = new ArrayList<Byte>();
    
   /******************/
   /* RouterTextArea */
   /******************/
   public RouterTextArea(
     Environment theEnv) 
     {  
      clips = theEnv;
      
      this.setEditable(false);
      this.addKeyListener(this);
      this.addFocusListener(this);
      this.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.white,3),
                                                        BorderFactory.createEmptyBorder(5,5,5,0)));

      this.setBorder(BorderFactory.createEmptyBorder(5,5,5,0));
            
      this.setMargin(new Insets(5,5,5,0));
      this.setFont(new Font("monospaced",Font.PLAIN,12));
      
      routerName = "JTextAreaRouter" + TextAreaRouterNameIndex++;
      clips.addRouter(this);
      
      new DropTarget(this, this);

      this.getCaret().setVisible(true);
      this.addCaretListener(this);
     }  
     
   /*########################*/
   /* Router Support Methods */
   /*########################*/
     
   /***************/
   /* appendChars */
   /***************/
   public synchronized boolean appendChars(
     String theString) 
     {
      if (! charNeeded) return false;
      
      try
        {
         byte theBytes [] = theString.getBytes("UTF-8");
         for (int i = 0; i < theBytes.length; i++)
           { charList.add(new Byte(theBytes[i])); }
        }
      catch (Exception e)
        { e.printStackTrace(); }
           
      charNeeded = false;
      notifyAll();
      return true;
     }   
     
   /************/
   /* pushChar */
   /************/
   public synchronized int pushChar(
     int theChar) 
     {
      charList.add(0,new Byte((byte) theChar));
      return theChar;
     }
     
   /****************/
   /* requestChar: */
   /****************/
   public synchronized int requestChar() 
     {
      /*====================================================*/
      /* If there's no input in the CLIPS internal buffer,  */
      /* but we have input remaining in the buffer for this */
      /* object, then clear this buffer since the remaining */
      /* input was extraneous for the last input request.   */
      /*====================================================*/
      
      if ((clips.getInputBufferCount() == 0) &&
          (charList.size() != 0))
        { charList.clear(); }
      
      if (charList.size() == 0)
        {
         charNeeded = true;
         while (charNeeded) 
           {
            try 
              { wait(); } 
            catch (InterruptedException e) 
              { e.printStackTrace(); }
           }
        }
        
      Byte theByte = charList.get(0);
      charList.remove(0);
      return theByte.intValue();
     }
     
   /***************/
   /* supplyChar: */
   /***************/
   public synchronized boolean supplyChar(
     char theChar) 
     {
      if (! charNeeded) return false;
      
      String charString = String.valueOf(theChar);
      try
        {
         byte theBytes [] = charString.getBytes("UTF-8");
         for (int i = 0; i < theBytes.length; i++)
           { charList.add(new Byte(theBytes[i])); }
        }
      catch (Exception e)
        { e.printStackTrace(); }
           
      charNeeded = false;
      notifyAll();
      return true;
     }
     
   /*########################*/
   /* JTextComponent Methods */
   /*########################*/

   /*********/
   /* paste */
   /*********/
   @Override
   public void paste()
     {
      try
        {
         String clipboardText = (String) 
                                Toolkit.getDefaultToolkit()
                                       .getSystemClipboard()
                                       .getData(DataFlavor.stringFlavor); 
         if (appendChars(clipboardText))
            {
             this.setCaretPosition(this.getText().length()); 
             this.append(clipboardText); 
            }
        }
      catch (Exception e)
        { e.printStackTrace(); }
     }

   /*################*/
   /* Router Methods */
   /*#################*/

   /****************/
   /* getPriority: */
   /****************/
   @Override
   public int getPriority()
     {
      return 10;
     }

   /************/
   /* getName: */
   /************/
   @Override
   public String getName()
     {
      return routerName;
     }

   /**********/
   /* query: */
   /**********/
   @Override
   public boolean query(
     String routerName)
     {      
      if (routerName.equals("stdout") ||
          routerName.equals("stdin") ||
          routerName.equals("wwarning") ||
          routerName.equals("werror") ||
          routerName.equals("wtrace") ||
          routerName.equals("wdialog") ||
          routerName.equals("wclips") ||
          routerName.equals("wdisplay"))
      
        { return true; }

      return false;
     }

   /**********/
   /* print: */
   /**********/
   @Override
   public void print(
     String routerName,
     String printString)
     {
      if (EventQueue.isDispatchThread())
        { 
         this.append(printString);
         return; 
        }
        
      try
        {
         SwingUtilities.invokeAndWait(
           new Runnable() 
             {  
              public void run() { RouterTextArea.this.append(printString); }  
             });   
        }
      catch (Exception e) 
        { e.printStackTrace(); }
     }

   /************/
   /* getchar: */
   /************/
   @Override
   public int getchar(
     String routerName)
     {
      return requestChar();
     }

   /**************/
   /* ungetchar: */
   /**************/
   @Override
   public int ungetchar(
     String routerName,
     int theChar)
     {
      return pushChar(theChar);
     }

   /*********/
   /* exit: */
   /*********/
   @Override
   public boolean exit(
     int exitCode)
     {      
      return true;
     }
     
   /*#####################*/
   /* KeyListener Methods */
   /*#####################*/
   
   /**************/
   /* keyPressed */
   /**************/     
   @Override
   public void keyPressed(KeyEvent e) 
     {
      if ((e.getModifiers() & 
          (KeyEvent.ALT_MASK | KeyEvent.CTRL_MASK | KeyEvent.META_MASK)) != 0) 
        { return; }

      e.consume();
     }

   /***************/
   /* keyReleased */
   /***************/     
   @Override
   public void keyReleased(KeyEvent e) 
     { 
      if ((e.getModifiers() & 
          (KeyEvent.ALT_MASK | KeyEvent.CTRL_MASK | KeyEvent.META_MASK)) != 0) 
        { return; }

      e.consume();
     }

   /************/
   /* keyTyped */
   /************/
   @Override
   public void keyTyped(KeyEvent e) 
     {
      if ((e.getModifiers() & 
         (KeyEvent.ALT_MASK | KeyEvent.CTRL_MASK | KeyEvent.META_MASK)) != 0) 
        { return; }

      char theChar = e.getKeyChar();
      
      if ((theChar == KeyEvent.VK_BACK_SPACE) ||
           (theChar == KeyEvent.VK_DELETE))
        {
         if (clips.getInputBufferCount() == 0)
           {
            e.consume();
            return;
           }
        }
       
      if (supplyChar(theChar))
        {
         if ((theChar == KeyEvent.VK_BACK_SPACE) ||
             (theChar == KeyEvent.VK_DELETE))
           {
            this.replaceRange("",this.getText().length() - 1,this.getText().length()); 
           }
         else
           { this.append(String.valueOf(theChar)); }
         e.consume();
        } 
     }
     
   /*#######################*/
   /* FocusListener Methods */
   /*#######################*/

   /***************/
   /* focusGained */
   /***************/
   @Override
   public void focusGained(FocusEvent e)
     {
      this.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.darkGray,3),
                                                        BorderFactory.createEmptyBorder(5,5,5,0)));
      this.getCaret().setVisible(true);
     }

   /*************/
   /* focusLost */
   /*************/
   @Override
   public void focusLost(FocusEvent e)
     {
      this.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.white,3),
                                                        BorderFactory.createEmptyBorder(5,5,5,0)));
      this.getCaret().setVisible(false);
     }

   /*############################*/
   /* DropTargetListener Methods */
   /*############################*/
   
   /*************/
   /* dragEnter */
   /*************/
   @Override
   public void dragEnter(DropTargetDragEvent dtde) 
     {
     }
      
   /*************/
   /* dragOver */
   /*************/
   @Override
   public void dragOver(DropTargetDragEvent dtde) 
     {
     }
     
   /*************/
   /* dragExit */
   /*************/ 
   @Override
   public void dragExit(DropTargetEvent dte) 
     {
     }
       
   /*********************/
   /* dropActionChanged */
   /*********************/
   @Override
   public void dropActionChanged(DropTargetDragEvent dtde)   
     { 
     }
      
   /********/
   /* drop */
   /********/
   @Override
   public void drop(DropTargetDropEvent dtde) 
     { 
      try 
        {
         Transferable tr = dtde.getTransferable();
         DataFlavor[] flavors = tr.getTransferDataFlavors();
         for (int i = 0; i < flavors.length; i++) 
           {
            if (flavors[i].isFlavorTextType()) 
              {
               dtde.acceptDrop(DnDConstants.ACTION_COPY);
               String dropText = (String) tr.getTransferData(flavors[i]);
               if (appendChars(dropText))
                 {
                  this.setCaretPosition(this.getText().length()); 
                  this.append(dropText); 
                 }
               dtde.dropComplete(true);
               return;
              }
           }

         dtde.rejectDrop();
        } 
      catch (Exception e) 
        {
         e.printStackTrace();
         dtde.rejectDrop();
        }
     }
   
   /*#######################*/
   /* CaretListener Methods */
   /*#######################*/

   /***************/
   /* caretUpdate */
   /***************/  
   public void caretUpdate(
     CaretEvent ce) 
     {
      if (EventQueue.isDispatchThread())
        { 
         caretUpdateAction(ce.getDot(), ce.getMark()); 
         return;
        }
      try
        {
         SwingUtilities.invokeAndWait(
           new Runnable() 
             {  
              public void run() 
                 { RouterTextArea.this.caretUpdateAction(ce.getDot(),ce.getMark()); }  
             });   
        }
      catch (Exception e) 
        { e.printStackTrace(); }
     }

   /*********************/
   /* caretUpdateAction */
   /*********************/  
   protected void caretUpdateAction(
     final int dot,
     final int mark) 
     {
      /*==============================================*/
      /* Attempting to move the caret outside of the  */
      /* text for the current command is not allowed. */
      /*==============================================*/
            
      if (dot == mark) 
        { 
         int tl = this.getText().length();
               
         if (dot < tl)
           { this.getCaret().setDot(tl); }

         this.getCaret().setVisible(true);
        }
              
      /*======================================*/
      /* If text is selected, hide the caret. */
      /*======================================*/
            
      else
        { this.getCaret().setVisible(false); }
     }    
  }