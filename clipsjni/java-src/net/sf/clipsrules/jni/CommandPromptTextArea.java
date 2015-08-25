package net.sf.clipsrules.jni;

import java.awt.*; 
import java.awt.event.*; 
import java.awt.datatransfer.*;
import java.awt.dnd.*;

import javax.swing.*;
import javax.swing.event.*; 

import java.lang.Thread;

public class CommandPromptTextArea extends RouterTextArea
  {   
   private boolean isExecuting = false;
   private int caretOffset;

   /*************************/
   /* CommandPromptTextArea */
   /*************************/
   public CommandPromptTextArea(
     Environment theEnv) 
     {  
      super(theEnv);
      
      theEnv.printBanner();
      theEnv.printPrompt();
      theEnv.setInputBufferCount(0);
      
      caretOffset = 0;
      
      this.getCaret().setVisible(true);
      this.addCaretListener(this);
     }  
     
   /**************/
   /* keyPressed */
   /**************/
   @Override
   public void keyPressed(KeyEvent e) 
     {
      if (getExecuting())
        { 
         super.keyPressed(e); 
         return;
        }
     }

   /************/
   /* keyTyped */
   /************/
   @Override
   public void keyTyped(KeyEvent e) 
     {      
      if (getExecuting())
        { 
         super.keyTyped(e); 
         return;
        }

      if ((e.getModifiers() & 
          (KeyEvent.ALT_MASK | KeyEvent.CTRL_MASK | KeyEvent.META_MASK)) != 0) 
        { return; }
          
      char c = e.getKeyChar();
         
      if ((c == KeyEvent.VK_BACK_SPACE) ||
          (c == KeyEvent.VK_DELETE))
        { modifyCommand("",true); }
      else 
        {
         modifyCommand(String.valueOf(c),false);
         commandCheck();
        }
                    
      e.consume();
     }
     
   /*****************/
   /* modifyCommand */
   /*****************/
   protected void modifyCommand(
     String replaceString,
     boolean isDelete) 
     {
      int textLength = this.getText().length();
      int commandLength = (int) clips.getInputBufferCount();  
      int lockedLength = textLength - commandLength;
      
      /*========================================*/
      /* Determine the left and right positions */
      /* of the current selection.              */
      /*========================================*/
            
      int left = Math.min(this.getCaret().getDot(),this.getCaret().getMark());
      int right = Math.max(this.getCaret().getDot(),this.getCaret().getMark());
      
      if (isDelete && (left == right) && (left > lockedLength))
        { left--; }
      
      /*************************************************/
      /* If the selection falls within text that can't */
      /* be modified (the output from prior commands), */
      /* then set the caret to the end of the command  */
      /* being edited and do nothing else.             */
      /*************************************************/

      if (left < lockedLength)
        {
         this.getCaret().setDot(textLength); 
         return;
        }
                    
      String newCommand = this.getText().substring(lockedLength,left) + 
                          replaceString +
                          this.getText().substring(right);
      this.replaceRange(replaceString,left,right);
      
      clips.setInputBuffer(newCommand);   
      
      caretOffset = this.getText().length() - this.getCaret().getDot();
      
      balanceParentheses();
     }
     
   /**********************/
   /* balanceParentheses */
   /**********************/
   public void balanceParentheses() 
     {
      long commandLength;
   
      /*=======================================================*/
      /* Don't balance the parentheses if there is no command. */
      /*=======================================================*/
   
      commandLength = clips.getInputBufferCount();
      if (commandLength <= 0) 
        { return; }

      /*=================================*/
      /* Retrieve the current selection. */
      /*=================================*/
          
      int selStart = this.getSelectionStart();
      int selEnd = this.getSelectionEnd();

      /*======================*/
      /* Where is the cursor? */
      /*======================*/
    
      int cursorLocation = this.getCaretPosition();
   
      if (cursorLocation == 0) return;
   
      cursorLocation--;
      
      /*===============================================*/
      /* What is the character at the cursor location? */
      /*===============================================*/
    
      char characterToCheck = this.getText().charAt(cursorLocation);
      
      /*======================================*/
      /* We only balance a right parenthesis. */
      /*======================================*/
   
      if (characterToCheck != ')') return;

      /*======================================================================*/
      /* The nesting depth will start at zero. Each time a ')' is encountered */
      /* the nesting depth is incremented by one and each time a '(' is       */
      /* encountered the nesting depth is decremented by one. If a '(' is     */
      /* encountered when the nesting depth is zero (the starting value), the */
      /* matching parenthesis has been found.                                 */
      /*======================================================================*/
   
      int nestingDepth = 0;

      /*==================================================*/
      /* Start looking for the matching left parenthesis. */
      /*==================================================*/

      while ((cursorLocation-- != 0) && (commandLength-- != 0)) 
        {
         characterToCheck = this.getText().charAt(cursorLocation);
         if (characterToCheck == '(') 
           {
            if (nestingDepth == 0) 
              {
               /*======================================*/
               /* Select the matching left parenthesis */
               /* and hide the carete.                 */
               /*======================================*/
               
               this.getCaret().setVisible(false);
               this.setSelectionStart(cursorLocation);
               this.setSelectionEnd(cursorLocation + 1);

               /*========================================*/
               /* Force an update to occur otherwise the */
               /* changed selection won't be visible.    */
               /*========================================*/
               
               this.update(this.getGraphics());

               /*============================================*/
               /* Pause momentarily so the selected matching */
               /* parenthesis can be observed.               */
               /*============================================*/
               
               try
		         { Thread.sleep(200); }
		       catch (Exception e)
		         { e.printStackTrace(); }

               /*===========================*/
               /* Restore the selection and */
               /* make the caret visible.   */
               /*===========================*/
               
               this.setSelectionStart(selStart);
               this.setSelectionEnd(selEnd);
               this.getCaret().setVisible(true);
		       
               return;
              }
            else
		      { nestingDepth--; }
           }
         else if (characterToCheck == ')') 
           { nestingDepth++; }
        }

      /*================================================*/
      /* Beep to indicate a matching ')' was not found. */
      /*================================================*/
   
      Toolkit.getDefaultToolkit().beep();
     }
     
   /*********************/
   /* expandInputBuffer */
   /*********************/
   public void expandInputBuffer(
     char theChar)
     {
      String charString = String.valueOf(theChar);
      try
        {
         byte theBytes [] = charString.getBytes("UTF-8");
         for (int i = 0; i < theBytes.length; i++)
           { clips.expandInputBuffer(Byte.valueOf(theBytes[i]).intValue()); }
        }
      catch (Exception e)
        { e.printStackTrace(); }
     }
     
   /****************/
   /* commandCheck */
   /****************/
   public void commandCheck() 
     {
      if (clips.inputBufferContainsCommand())
        { 
         //updateCommandHistory();
         executeCommand(); 
        }
     }
         
   /****************/
   /* getExecuting */
   /****************/
   public synchronized boolean getExecuting() 
     {
      return isExecuting;
     }

   /****************/
   /* setExecuting */
   /****************/
   public synchronized void setExecuting(
     boolean value) 
     {
      isExecuting = value;
     }
   
   /******************/
   /* executeCommand */
   /******************/  
   public void executeCommand()
     {
      caretOffset = 0;
      
      setExecuting(true);
      
      Runnable runThread = 
         new Runnable()
           {
            public void run()
              { 
               clips.commandLoopOnceThenBatch(); 
               setExecuting(false);
              }
           };
      
      Thread executionThread = new Thread(runThread);
      
      executionThread.start();
     }
     
   /*########################*/
   /* JTextComponent Methods */
   /*########################*/

   /*******/
   /* cut */
   /*******/
   @Override
   public void cut()
     {
      if (getExecuting())
        { 
         super.cut(); 
         return;
        }
        
      this.copy();
      modifyCommand("",true);
     }
     
   /*********/
   /* paste */
   /*********/
   @Override
   public void paste()
     {
      if (getExecuting())
        { 
         super.paste(); 
         return;
        }
        
      try
        {
         String clipboardText = (String) 
                                Toolkit.getDefaultToolkit()
                                       .getSystemClipboard()
                                       .getData(DataFlavor.stringFlavor); 
         modifyCommand(clipboardText,false);
        }
      catch (Exception e)
        { e.printStackTrace(); }
     }
     
   /*#######################*/
   /* CaretListener Methods */
   /*#######################*/

   /*********************/
   /* caretUpdateAction */
   /*********************/  
   @Override
   protected void caretUpdateAction(
     final int dot,
     final int mark) 
     {      
      if (getExecuting())
        { 
         super.caretUpdateAction(dot,mark); 
         return;
        }
              
      /*==============================================*/
      /* Attempting to move the caret outside of the  */
      /* text for the current command is not allowed. */
      /*==============================================*/
            
      if (dot == mark) 
        { 
         int tl = this.getText().length();
         int il = (int) clips.getInputBufferCount();
               
         if (dot < (tl -il))
           { this.getCaret().setDot(tl-caretOffset); }
         else
           { caretOffset = tl - dot; }

         this.getCaret().setVisible(true);
        }
              
      /*======================================*/
      /* If text is selected, hide the caret. */
      /*======================================*/
            
      else
        { this.getCaret().setVisible(false); }
     }  
       
   /*############################*/
   /* DropTargetListener Methods */
   /*############################*/
   
   /************************/
   /* dragTargetCaretStart */
   /************************/
   @Override
   protected int dragTargetCaretStart()
     {
      if (getExecuting())
        { return super.dragTargetCaretStart(); }

      return this.getText().length() - (int) clips.getInputBufferCount();
     }
  
   /********/
   /* drop */
   /********/
   @Override
   public void drop(DropTargetDropEvent dtde) 
     { 
      if (getExecuting())
        { 
         super.drop(dtde); 
         return;
        }
        
      if ((dtde.getDropAction() & DnDConstants.ACTION_COPY_OR_MOVE) == 0) 
        {
         dtde.dropComplete(false);
         return; 
        }
      try 
        {
         Transferable tr = dtde.getTransferable();
         DataFlavor[] flavors = tr.getTransferDataFlavors();
         for (int i = 0; i < flavors.length; i++) 
           {
            if (flavors[i].equals(DataFlavor.stringFlavor)) 
              {
               dtde.acceptDrop(dtde.getDropAction());
               String dropText = (String) tr.getTransferData(flavors[i]);
               modifyCommand(dropText,false);
               this.requestFocus();
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
  }
  
  
