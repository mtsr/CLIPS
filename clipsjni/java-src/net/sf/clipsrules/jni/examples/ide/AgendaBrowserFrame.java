package net.sf.clipsrules.jni.examples.ide;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;
import javax.swing.JTable;
import javax.swing.BoxLayout;
import javax.swing.JToggleButton;
import javax.swing.JInternalFrame;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import javax.swing.table.*;

public class AgendaBrowserFrame extends JInternalFrame
  {
   private JFrame browserFrame;
   private JToggleButton resetButton = null;
   private JToggleButton runButton = null;
   private JToggleButton stepButton = null;
   private JToggleButton haltRulesButton = null;
   private JTable moduleTable, activationTable;
   private DefaultTableModel moduleModel, activationModel;
   private JScrollPane modulePane, activationPane;
   private JSplitPane contentPane;
   private static int agendaCount = 1;

   /*****************/
   /* AgendaBrowser */
   /*****************/
   AgendaBrowserFrame()
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

      resetButton = new JToggleButton("Reset");
      resetButton.setEnabled(false);
      //resetButton.setActionCommand(pauseAction);
      //resetButton.addActionListener(this);
      buttonPanel.add(resetButton);

      runButton = new JToggleButton("Run");
      runButton.setEnabled(false);
      //resetButton.setActionCommand(pauseAction);
      //resetButton.addActionListener(this);
      buttonPanel.add(runButton);

      stepButton = new JToggleButton("Step");
      stepButton.setEnabled(false);
      //stepButton.setActionCommand(pauseAction);
      //stepButton.addActionListener(this);
      buttonPanel.add(stepButton);
      
      haltRulesButton = new JToggleButton("Halt Rules");
      haltRulesButton.setEnabled(false);
      //haltRulesButton.setActionCommand(pauseAction);
      //haltRulesButton.addActionListener(this);
      buttonPanel.add(haltRulesButton);
      
      // TBD layout for buttons?
      
      this.getContentPane().add(buttonPanel); 
      
      // Create the content area

      moduleModel = new DefaultTableModel();
      moduleTable = new JTable(moduleModel);
      moduleModel.setDataVector(new Object[][] { { "A" } , { "B" } },
                                new Object[] { "Focus Stack" });                        
      modulePane = new JScrollPane(moduleTable);

      activationModel = new DefaultTableModel();
      activationTable = new JTable(activationModel);
      activationModel.setDataVector(new Object[][] { { "-10000" , "get-key" , "f-1,f-2" } , 
                                                     { "10" , "move-to-chest" , "f-3,f-4" } },
                                  new Object[] { "Salience" , "Rule" , "Basis" });                        
      activationPane = new JScrollPane(activationTable);

      activationTable.getColumnModel().getColumn(0).setMinWidth(65);
      activationTable.getColumnModel().getColumn(0).setPreferredWidth(75);
      activationTable.getColumnModel().getColumn(1).setPreferredWidth(250);
      activationTable.getColumnModel().getColumn(2).setPreferredWidth(150);

      contentPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,modulePane, activationPane);
      contentPane.setOneTouchExpandable(true);
      contentPane.setDividerLocation(125);
      contentPane.setDividerSize(15);

      contentPane.setPreferredSize(new Dimension(600,160));
      contentPane.setAlignmentX(Component.CENTER_ALIGNMENT);

      this.getContentPane().add(contentPane); 

      /*====================*/
      /* Display the frame. */
      /*====================*/

      this.pack();
     }  
  }