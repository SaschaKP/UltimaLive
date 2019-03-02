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
using System.Text;
using System.IO;

namespace MapAndStaticsVerifier
{
  public class MapMulFile : MulFile
  {
    public MapMulFile(string filename)
      : base(filename)
    {
    }

    public List<string> m_badBlocks = new List<string>();
    public List<string> BadBlocks { get { return m_badBlocks; } set { m_badBlocks = value; } }

    public void CheckForBlockErrors()
    {
      string operationString = string.Format("Checking Map{0}.mul for Block Errors", m_index);
      StartOperation(operationString);
      m_badBlocks.Clear();

      int blockNumber = 0;
      using (FileStream fs = new FileStream(m_Filename, FileMode.Open))
      {
        using (BinaryReader reader = new BinaryReader(fs))
        {
          int lastPercent = 0;
          while (fs.Position + 196 < fs.Length)
          {
            int currentPercent = (int)(((double)fs.Position / (double)fs.Length) * 100);
            if (currentPercent > lastPercent)
            {
              lastPercent = currentPercent;
              UpdateOperationStatus(operationString, currentPercent);
            }

            UInt32 blockHeader = reader.ReadUInt32();
            for (int i = 0; i < 64; ++i)
            {
              UInt16 tileId = reader.ReadUInt16();
              sbyte altitude = reader.ReadSByte();
            }

            if (blockHeader == UInt32.MaxValue)
            {
              m_badBlocks.Add(String.Format("{0} - block version is bad", blockNumber));
            }

            blockNumber++;
          }
        }
      }

      StopOperation(operationString);
    }
  }
}
