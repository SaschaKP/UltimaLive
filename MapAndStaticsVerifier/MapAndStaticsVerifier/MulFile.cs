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
using System.Text;

namespace MapAndStaticsVerifier
{
  public class MulFile
  {
    public delegate void UpdateOperationStatusHandler(string operation, int percent); 
    public event UpdateOperationStatusHandler UpdateOperationStatusEvent;

    public delegate void StartNewOperationHandler(string operation); 
    public event StartNewOperationHandler StartNewOperationEvent;

    public delegate void StopOperationHandler(string operation);
    public event StopOperationHandler StopOperationEvent;

    public void StartOperation(string operation)
    {
      if (StartNewOperationEvent != null)
      {
        StartNewOperationEvent(operation);
      }
    }

    public void StopOperation(string operation)
    {
      if (StopOperationEvent != null)
      {
        StopOperationEvent(operation);
      }
    }

    public void UpdateOperationStatus(string operation, int percent)
    {
      if (UpdateOperationStatusEvent != null)
      {
        UpdateOperationStatusEvent(operation, percent);
      }
    }

    protected int m_index;
    public int Index { get { return m_index; } set { m_index = value; } }

    protected string m_Filename;
    public string Filename { get { return m_Filename; } set { m_Filename = value; } }

    protected string m_ShortFilename;
    public string ShortFilename { get { return m_ShortFilename; } set { m_ShortFilename = value; } }

    protected long m_Filesize = 0;
    public long Filesize { get { return m_Filesize; } set { m_Filesize = value; } }

    public MulFile(string filename)
    {
      m_Filename = filename;
      m_ShortFilename = System.IO.Path.GetFileName(m_Filename);
    }
  }
}
