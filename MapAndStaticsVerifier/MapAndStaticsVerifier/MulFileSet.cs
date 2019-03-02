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
  public class MulFileSet
  {
    public int Index { get; set; }
    public MulFileSet(int idx)
    {
      Index = idx;
    }

    public List<MulFile> m_MulFiles = new List<MulFile>();
    public List<MulFile> MulFiles { get { return m_MulFiles; } set { m_MulFiles = value; } }

  }
}
