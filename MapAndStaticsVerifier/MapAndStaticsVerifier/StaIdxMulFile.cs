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
using System.IO;

namespace MapAndStaticsVerifier
{
  public class IndexForStaticBlock
  {
    public IndexForStaticBlock(UInt32 offset, UInt32 len, UInt32 ver, UInt32 block)
    {
      m_Offset = offset;
      m_Length = len;
      m_Version = ver;
      m_Block = block;
      m_Errors = new List<string>();
    }

    private UInt32 m_Block;
    public UInt32 Block { get { return m_Block; } set { m_Block = value; } }

    private UInt32 m_Offset;
    public UInt32 Offset { get { return m_Offset; } set { m_Offset = value; } }

    private UInt32 m_Length;
    public UInt32 Length { get { return m_Length; } set { m_Length = value; } }

    private UInt32 m_Version;
    public UInt32 Version { get { return m_Version; } set { m_Version = value; } }

    private List<string> m_Errors;
    public List<string> Errors { get { return m_Errors; } set { m_Errors = value; } }
  }


  public class StaIdxMulFile : MulFile
  {

    public StaIdxMulFile(string filename)
      : base(filename)
    {
    }

    public List<IndexForStaticBlock> m_badIndices = new List<IndexForStaticBlock>();
    public List<IndexForStaticBlock> BadIndices { get { return m_badIndices; } set { m_badIndices = value; } }

    public void CheckForBlockErrors()
    {
      string operationString = string.Format("Checking staidx{0}.mul for Block Errors", m_index);

      StartOperation(operationString);

      m_badIndices.Clear();

      string staticsFilename = Path.Combine(m_Filename.Substring(0, m_Filename.Length - m_ShortFilename.Length), string.Format("statics{0}.mul", m_index));
      FileInfo staticsFile = new FileInfo(staticsFilename);

      UInt32 blockNumber = 0;
      using (FileStream fs = new FileStream(m_Filename, FileMode.Open))
      {
        using (BinaryReader reader = new BinaryReader(fs))
        {
          Dictionary<UInt32, List<string>> errors = new Dictionary<UInt32, List<string>>();

          List<string> errorMessages = new List<string>();

          int lastPercent = 0;

          while (fs.Position + 12 < fs.Length)
          {
            int currentPercent = (int)(((double)fs.Position / (double)fs.Length) * 100);
            if ( currentPercent> lastPercent)
            {
              lastPercent = currentPercent;
              UpdateOperationStatus(operationString, currentPercent);
            }

            UInt32 Offset = reader.ReadUInt32();
            UInt32 Length = reader.ReadUInt32();
            UInt32 Version = reader.ReadUInt32();

            if (Offset != UInt32.MaxValue && staticsFile.Exists && Offset > staticsFile.Length)
            {
              errorMessages.Add(String.Format("Offset (0x{3}) points beyond the end of statics{0}.mul ({1} / {2})", m_index, Offset, staticsFile.Length, Offset.ToString("X")));
            }

            if (Offset != UInt32.MaxValue && staticsFile.Exists && (Offset + Length - 1) >= staticsFile.Length)
            {
              errorMessages.Add(String.Format("Static Length extends beyond the end of statics{0}.mul ({1} / {2})", m_index, Offset + Length - 1, staticsFile.Length));
            }

            if (errorMessages.Count > 0)
            {
              IndexForStaticBlock idx = new IndexForStaticBlock(Offset, Length, Version, blockNumber);
              foreach (string message in errorMessages)
              {
                idx.Errors.Add(message);
              }

              m_badIndices.Add(idx);
              errorMessages.Clear();
            }

            blockNumber++;
          }
        }
      }

      StopOperation(operationString);
    }
  }
}
