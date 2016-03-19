package net.sf.clipsrules.jni.examples.ide;

import javax.swing.*;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.table.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.util.HashMap;
import java.awt.EventQueue;

import net.sf.clipsrules.jni.*;

public class AgendaBrowserFrame extends JInternalFrame
                             implements ActionListener, ListSelectionListener
  {
   private JFrame browserFrame;
   private JButton resetButton = null;
   private JButton runButton = null;
   private JButton stepButton = null;
   private JButton haltRulesButton = null;
   private JTable focusStackTable, activationTable;
   private FocusStackTableModel focusStackModel;
   private ActivationTableModel activationModel;
   private JScrollPane focusStackPane, activationPane;
   private JSplitPane contentPane;
   private static int agendaCount = 1;

   private FocusStack focusStack = null;
   private HashMap<Focus,Agenda> agendaMap = new HashMap<Focus,Agenda>();
   private int agendaChangeIndex = -1;
   
   static final String resetAction = "Reset";
   static final String runAction = "Run";
   static final String stepAction = "Step";
   static final String haltRulesAction = "HaltRules";
   
   private ActionListener actionTarget = null;

   /*****************/
   /* AgendaBrowser */
   /*****************/
   AgendaBrowserFrame()
     {      
      this(new FocusStack(),new HashMap<Focus,Agenda>());
     }

   /*****************/
   /* AgendaBrowser */
   /*****************/
   AgendaBrowserFrame(
     FocusStack theFocusStack,
     HashMap<Focus,Agenda> theAgendaMap)
     {  
      super("Agenda #" + agendaCount++,true,true,true,true);
            
      /*===================================*/
      /* Create a new JFrame container and */
      /* assign a layout manager to it.    */
      /*===================================*/

      this.getContentPane().setLayout(new BoxLayout(this.getContentPane(),BoxLayout.Y_AXIS));
      
      /*=================================*/
      /* Give the frame an initial size. */
      /*=================================*/
     
      this.setSize(600,200);  
      this.setMinimumSize(new Dimension(450,100));
      
      /*===========================================*/
      /* The close button closes just the browser. */
      /*===========================================*/
     
      this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);  
      
      /*==========================*/
      /* Create the button panel. */
      /*==========================*/
      
      JPanel buttonPanel = new JPanel(); 
      buttonPanel.setPreferredSize(new Dimension(600,40));
      buttonPanel.setLayout(new FlowLayout(FlowLayout.LEFT));

      resetButton = new JButton("Reset");
      resetButton.setEnabled(false);
      resetButton.setActionCommand(resetAction);
      resetButton.addActionListener(this);
      buttonPanel.add(resetButton);

      runButton = new JButton("Run");
      runButton.setEnabled(false);
      runButton.setActionCommand(runAction);
      runButton.addActionListener(this);
      buttonPanel.add(runButton);

      stepButton = new JButton("Step");
      stepButton.setEnabled(false);
      stepButton.setActionCommand(stepAction);
      stepButton.addActionListener(this);
      buttonPanel.add(stepButton);
      
      haltRulesButton = new JButton("Halt Rules");
      haltRulesButton.setEnabled(false);
      haltRulesButton.setActionCommand(haltRulesAction);
      haltRulesButton.addActionListener(this);
      buttonPanel.add(haltRulesButton);
      
      // TBD layout for buttons?
      
      this.getContentPane().add(buttonPanel); 

      /****************************/
      /* Create the content area. */
      /****************************/

      DefaultTableCellRenderer centerRenderer = new DefaultTableCellRenderer();
      centerRenderer.setHorizontalAlignment(JLabel.CENTER);

      focusStackModel = new FocusStackTableModel();
      focusStackTable = new JTable(focusStackModel);
      focusStackTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      focusStackTable.getSelectionModel().addListSelectionListener(this);
      focusStackTable.getTableHeader().setReorderingAllowed(false);
      focusStackTable.getColumnModel().getColumn(0).setCellRenderer(centerRenderer);
                 
      focusStackPane = new JScrollPane(focusStackTable);

      activationModel = new ActivationTableModel();
      activationTable = new JTable(activationModel);
      activationTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      activationTable.getSelectionModel().addListSelectionListener(this);

      activationTable.getTableHeader().setReorderingAllowed(false);

      activationPane = new JScrollPane(activationTable);

      activationTable.getColumnModel().getColumn(0).setMinWidth(65);
      activationTable.getColumnModel().getColumn(0).setPreferredWidth(75);
      activationTable.getColumnModel().getColumn(1).setPreferredWidth(250);
      activationTable.getColumnModel().getColumn(2).setPreferredWidth(150);

      activationTable.getColumnModel().getColumn(0).setCellRenderer(centerRenderer);
      activationTable.getColumnModel().getColumn(1).setCellRenderer(centerRenderer);
      activationTable.getColumnModel().getColumn(2).setCellRenderer(centerRenderer);

      contentPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,focusStackPane, activationPane);
      contentPane.setOneTouchExpandable(true);
      contentPane.setDividerLocation(125);
      contentPane.setDividerSize(15);

      contentPane.setPreferredSize(new Dimension(600,160));
      contentPane.setAlignmentX(Component.CENTER_ALIGNMENT);

      this.getContentPane().add(contentPane); 
      
      assignData(theFocusStack,theAgendaMap);

      /*====================*/
      /* Display the frame. */
      /*====================*/

      this.pack();
     }  
 
   /*******************/
   /* setActionTarget */
   /*******************/
   public void setActionTarget(
     ActionListener theActionTarget)
     {
      actionTarget = theActionTarget;
     }


   /***********************/
   /* updateButtonsDriver */
   /***********************/
   public void updateButtonsDriver(
     boolean isExecuting)
     {
      if (isExecuting)
        {
         resetButton.setEnabled(false);
         runButton.setEnabled(false);
         stepButton.setEnabled(false);
         haltRulesButton.setEnabled(true);        
        }
      else
        {
         resetButton.setEnabled(true);
         runButton.setEnabled(true);
         stepButton.setEnabled(true);
         haltRulesButton.setEnabled(false);        
        }
     }

   /*****************/
   /* updateButtons */
   /*****************/
   public void updateButtons(
     boolean isExecuting)
     {
      if (EventQueue.isDispatchThread())
        { 
         updateButtonsDriver(isExecuting);
         return; 
        }
              
      try
        {
         SwingUtilities.invokeAndWait(
           new Runnable() 
             {  
              public void run() 
                { updateButtonsDriver(isExecuting); }  
             });   
        }
      catch (Exception e) 
        { e.printStackTrace(); }
     }

   /**************/
   /* assignData */
   /**************/
   public void assignData(
     FocusStack theFocusStack,
     HashMap<Focus,Agenda> theAgendaMap)
     {
      focusStack = theFocusStack;
      agendaMap = theAgendaMap;

      focusStackModel.setStack(focusStack);
      if (focusStack.size() == 0)
        { activationModel.setAgenda(null); }
      else
        { activationModel.setAgenda(agendaMap.get(focusStack.get(0))); }

      if (focusStack.size() != 0)
        { 
         focusStackTable.setRowSelectionInterval(0,0); 
         if (agendaMap.get(focusStack.get(0)).size() != 0)
           { activationTable.setRowSelectionInterval(0,0); }
        }
     }

   /****************/
   /* valueChanged */
   /****************/
   public void valueChanged(ListSelectionEvent event) 
     {
      if (event.getValueIsAdjusting()) return;
      
      if (event.getSource().equals(focusStackTable.getSelectionModel()))
         {
          int viewRow = focusStackTable.getSelectedRow();
          
          if (viewRow == -1) return;

          if (focusStack.size() == 0)
            { activationModel.setAgenda(null); }
          else
            { 
             Agenda theAgenda;
             theAgenda = agendaMap.get(focusStack.get(viewRow));

             activationModel.setAgenda(theAgenda); 
             if (agendaMap.get(focusStack.get(viewRow)).size() != 0)
               { activationTable.setRowSelectionInterval(0,0); }
            }
         }
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
      if (actionTarget == null) return;
      
      try
        { actionTarget.actionPerformed(ae); }
      catch (Exception e)
        { e.printStackTrace(); }
     }
     

  }