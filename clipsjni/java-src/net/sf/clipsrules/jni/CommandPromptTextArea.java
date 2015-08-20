package net.sf.clipsrules.jni;

import java.awt.*; 
import java.awt.event.*; 
import java.lang.Thread;

public class CommandPromptTextArea extends RouterTextArea
                                   //implements CaretListener
  {   
   private boolean isExecuting = false;

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
   public void keyTyped(KeyEvent e) 
     {      
      if (getExecuting())
        { super.keyTyped(e); }
      else
        {
         int id = e.getID();
      
         if (id != KeyEvent.KEY_TYPED) return;

         if ((e.getModifiers() & (KeyEvent.ALT_MASK | KeyEvent.CTRL_MASK | KeyEvent.META_MASK)) != 0) return;
 
         /* moveSelectionToEnd(); */
         
         char c = e.getKeyChar();
         
         if ((c == KeyEvent.VK_BACK_SPACE) ||
             (c == KeyEvent.VK_DELETE))
           {
            if (clips.getInputBufferCount() <= 0) return;
            this.replaceRange("",this.getText().length() - 1,this.getText().length());
            expandInputBuffer(c);
            balanceParentheses();
           }
         else //if (caretOffset == 0) // TBD
           {
            this.append(String.valueOf(c));
            expandInputBuffer(c);
            balanceParentheses();
            commandCheck();
           }
                    
         e.consume();
        }
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
      //caretOffset = 0;
      
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


  }