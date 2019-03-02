/* Copyright (C) 2013 Ian Karlinsey
 * 
 * UltimeLive is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * UltimaLive is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with UltimaLive.  If not, see <http://www.gnu.org/licenses/>. 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Threading;

namespace MapAndStaticsVerifier
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private BindingList<MulFileSet> m_mapFiles = new BindingList<MulFileSet>();
    public BindingList<MulFileSet> MapFiles { get { return m_mapFiles; } set { m_mapFiles = value; } }

    private Thread m_workerThread = null;

    public MainWindow()
    {
      InitializeComponent();
    }

    private void MenuItem_Click(object sender, RoutedEventArgs e)
    {
      m_workerThread = new Thread(new ThreadStart(ParseFiles));
      m_workerThread.SetApartmentState(ApartmentState.STA);
      m_workerThread.Start();
    }

    private void ParseFiles()
    {
      Dictionary<int, List<MulFile>> mapSets = new Dictionary<int, List<MulFile>>();

      System.Windows.Forms.FolderBrowserDialog folder = new System.Windows.Forms.FolderBrowserDialog();

      string appDataPath = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData);
      folder.SelectedPath = System.IO.Path.Combine(appDataPath, "UltimaLive");

      System.Windows.Forms.DialogResult result = folder.ShowDialog();

      if (result == System.Windows.Forms.DialogResult.OK)
      {
        string[] files = System.IO.Directory.GetFiles(folder.SelectedPath);

        foreach (string filepath in files)
        {
          string filename = System.IO.Path.GetFileName(filepath);

          if (filename.StartsWith("map") && filename.EndsWith(".mul"))
          {
            try
            {
              string numString = filename.Substring(3, filename.Length - 7);
              int index = Convert.ToInt32(numString);
              //int index = Convert.ToInt32(filename.Substring(3, filename.Length - 4));
              if (!mapSets.ContainsKey(index))
              {
                mapSets.Add(index, new List<MulFile>());
              }
              MapMulFile mapMul = new MapMulFile(filepath);
              mapMul.Index = index;
              mapMul.StartNewOperationEvent += StartNewOperation;
              mapMul.UpdateOperationStatusEvent += UpdateOperationStatus;
              mapMul.StopOperationEvent += StopOperation;
              mapMul.CheckForBlockErrors();
              mapSets[index].Add(mapMul);
            }
            catch (Exception mapException)
            {
              System.Windows.Forms.MessageBox.Show(mapException.Message + " ::: " + mapException.StackTrace);
            }
          }

          if (filename.StartsWith("statics") && filename.EndsWith(".mul"))
          {
            try
            {
              int index = Convert.ToInt32(filename.Substring(7, filename.Length - 11));
              if (!mapSets.ContainsKey(index))
              {
                mapSets.Add(index, new List<MulFile>());
              }
              StaticsMulFile staticsMul = new StaticsMulFile(filepath);
              staticsMul.Index = index;

              mapSets[index].Add(staticsMul);
            }
            catch (Exception staticsException)
            {
              System.Windows.Forms.MessageBox.Show(staticsException.Message + " ::: " + staticsException.StackTrace);
            }

          }

          if (filename.StartsWith("staidx") && filename.EndsWith(".mul"))
          {
            try
            {
              int index = Convert.ToInt32(filename.Substring(6, filename.Length - 10));
              if (!mapSets.ContainsKey(index))
              {
                mapSets.Add(index, new List<MulFile>());
              }

              StaIdxMulFile staidxMul = new StaIdxMulFile(filepath);
              staidxMul.Index = index;
              staidxMul.StartNewOperationEvent += StartNewOperation;
              staidxMul.UpdateOperationStatusEvent += UpdateOperationStatus;
              staidxMul.StopOperationEvent += StopOperation;
              staidxMul.CheckForBlockErrors();
              mapSets[index].Add(staidxMul);
            }
            catch (Exception staidxException)
            {
              System.Windows.Forms.MessageBox.Show(staidxException.Message + " ::: " + staidxException.StackTrace);
            }
          }
        }

        updateTreeview(mapSets);
      }
    }

    public void StartNewOperation(string op)
    {
      StartNewOperation(new object[] {op});
    }
    public void StartNewOperation(object[] op)
    {
      if (!Dispatcher.Thread.Equals(Thread.CurrentThread))
      {
        Dispatcher.BeginInvoke(new action(StartNewOperation), new object[] { op });
      }
      else
      {
       OperationLabel.Text = (string)op[0];
       OperationPercentDone.Value = 0;

       OperationLabel.Visibility = Visibility.Visible;
       OperationPercentDone.Visibility = Visibility.Visible;
      }
    }

    public void StopOperation(string op)
    {
      StopOperation(new object[] { op });
    }
    public void StopOperation(object[] op)
    {
      if (!Dispatcher.Thread.Equals(Thread.CurrentThread))
      {
        Dispatcher.BeginInvoke(new action(StopOperation), new object[] { op });
      }
      else
      {
        if (OperationLabel.Text == (string)op[0])
        {
          OperationPercentDone.Value = 100;
          OperationLabel.Visibility = Visibility.Collapsed;
          OperationPercentDone.Visibility = Visibility.Collapsed;
        }
      }
    }

    public void UpdateOperationStatus(string operation, int percent)
    {
      UpdateOperationStatus(new object[] { operation, percent });
    }

    public void UpdateOperationStatus(object[] objs)
    {
      if (!Dispatcher.Thread.Equals(Thread.CurrentThread))
      {
        Dispatcher.BeginInvoke(new action(UpdateOperationStatus), new object[] { objs });
      }
      else
      {
        if (OperationLabel.Text == (string)objs[0])
        {
          OperationPercentDone.Value = (int)objs[1];
        }
      }
    }

    private void updateTreeview(Dictionary<int, List<MulFile>> mapSets)
    {
      updateTreeview(new object[] {mapSets});
    }

    delegate void action(object[] parameters);
    private void updateTreeview(object[] mapSetsObject)
    {
      if ( !Dispatcher.Thread.Equals( Thread.CurrentThread ) )
      {
          Dispatcher.BeginInvoke(new action(updateTreeview), new object[] {mapSetsObject});
      }
      else
      {
        m_mapFiles.Clear();
        Dictionary<int, List<MulFile>> sets = (Dictionary<int, List<MulFile>>)mapSetsObject[0];
        foreach (KeyValuePair<int, List<MulFile>> kvp in sets)
        {
          MulFileSet set = new MulFileSet(kvp.Key);
          foreach (MulFile mulFile in kvp.Value)
          {
            FileInfo fi = new FileInfo(mulFile.Filename);
            mulFile.Filesize = fi.Length;
            set.MulFiles.Add(mulFile);
          }
          m_mapFiles.Add(set);
        }
        mnuSaveReport.IsEnabled = true;
      }
    }

    private void MenuItem_Click_1(object sender, RoutedEventArgs e)
    {
      string report = "";

      foreach (MulFileSet set in m_mapFiles)
      {
        foreach (MulFile file in set.MulFiles)
        {
          if (file != null && file is StaIdxMulFile)
          {
            StaIdxMulFile staidxMul = (StaIdxMulFile)file;
            if (staidxMul.BadIndices.Count > 0)
            {
              report += string.Format("Staidx{0}.mul had the following errors:\r\n", file.Index);
              foreach (IndexForStaticBlock idx in staidxMul.BadIndices)
              {
                foreach(string s in idx.Errors)
                {
                  report += string.Format("    Block {0} - {1}\r\n", idx.Block, s);
                }
              }
            }
          }
        }
      }

      if (report != "")
      {
        System.Windows.Forms.SaveFileDialog saveDialogue = new System.Windows.Forms.SaveFileDialog();
        saveDialogue.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";

        System.Windows.Forms.DialogResult result = saveDialogue.ShowDialog();

        if (result == System.Windows.Forms.DialogResult.OK)
        {
          using (System.IO.StreamWriter writer = new System.IO.StreamWriter(saveDialogue.FileName))
          {
            writer.Write(report);
          }
        }
      }
    }
  }
}
