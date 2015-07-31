using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

using CLIPSNET;
using RouterExample.Properties;

namespace RouterExample
  {
   public partial class RouterForm : Form
     {
      private CLIPSNET.Environment autoEnv = new CLIPSNET.Environment();
      private CLIPSNET.Environment animalEnv = new CLIPSNET.Environment();

      public void RunExample(
        CLIPSNET.Environment theEnv)
        {
         theEnv.Reset();
         theEnv.Run();
        }

      private void OnLoad(object sender, EventArgs e)
        {
         //CLIPSNET.Environment autoEnv = new CLIPSNET.Environment();
         this.autoTextBox.AttachRouter(autoEnv,10);
         autoEnv.LoadFromResource("RouterExample", "RouterExample.auto.clp");
         autoEnv.LoadFromResource("RouterExample", "RouterExample.auto_en.clp");

         //CLIPSNET.Environment animalEnv = new CLIPSNET.Environment();
         this.animalTextBox.AttachRouter(animalEnv, 10);
         animalEnv.LoadFromResource("RouterExample", "RouterExample.bcengine.clp");
         animalEnv.LoadFromResource("RouterExample", "RouterExample.animal.clp");
         animalEnv.LoadFromResource("RouterExample", "RouterExample.animal_en.clp");

         //Thread autoThread = new Thread(() => RunExample(autoEnv));
         //autoThread.Start();
         
         //Thread animalThread = new Thread(() => RunExample(animalEnv));
         //animalThread.Start();
         autoBackgroundWorker.RunWorkerAsync();
         animalBackgroundWorker.RunWorkerAsync();
        }

      public RouterForm()
        {
         InitializeComponent();
        }

      private void AutoDoWork(object sender, DoWorkEventArgs e)
         {
          RunExample(autoEnv);
         }

      private void AnimalDoWork(object sender, DoWorkEventArgs e)
         {
          RunExample(animalEnv);
         }

      private void AutoWorkCompleted(object sender, RunWorkerCompletedEventArgs e)
         {
          this.RestartAuto.Enabled = true;
         }

      private void AnimalWorkCompleted(object sender, RunWorkerCompletedEventArgs e)
         {
          this.RestartAnimal.Enabled = true;
         }

      private void RestartAutoClicked(object sender, EventArgs e)
         {
          this.RestartAuto.Enabled = false;
          this.autoTextBox.Clear();
          this.autoTextBox.Focus();
          autoBackgroundWorker.RunWorkerAsync();
         }

      private void RestartAnimalClicked(object sender, EventArgs e)
         {
          this.RestartAnimal.Enabled = false;
          this.animalTextBox.Clear();
          this.animalTextBox.Focus();
          animalBackgroundWorker.RunWorkerAsync();
         }
      }
  }
