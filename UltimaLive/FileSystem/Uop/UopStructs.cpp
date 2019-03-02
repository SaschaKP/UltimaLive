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

#include "UopStructs.h"

void UopHeader::unmarshal(uint8_t* pData)
{
  FileIdentifier = *reinterpret_cast<uint32_t*>(pData);  //myp\0
  Version = *reinterpret_cast<uint32_t*>(pData + 4); //0x00000005
  Signature = *reinterpret_cast<uint32_t*>(pData + 8); 
  FileTableOffset = *reinterpret_cast<uint64_t*>(pData + 12);
  FileTableCapacity = *reinterpret_cast<uint32_t*>(pData + 20);
  TotalFiles = *reinterpret_cast<uint32_t*>(pData + 24);
  NumFileTables = *reinterpret_cast<uint32_t*>(pData + 28);
}

void FileEntry::unmarshal(uint8_t* pData)
{
  UopFileOffset = *reinterpret_cast<uint64_t*>(pData);
  MetaDataSize = *reinterpret_cast<uint32_t*>(pData + 8);
  CompressedDataSize = *reinterpret_cast<uint32_t*>(pData + 12);
  UncompressedDataSize = *reinterpret_cast<uint32_t*>(pData + 16);
  PathChecksum = *reinterpret_cast<uint64_t*>(pData + 20);
  MetadataCrc = *reinterpret_cast<uint32_t*>(pData + 28);
  CompressionMethod = *reinterpret_cast<uint16_t*>(pData + 32);
}

void FileTable::unmarshal(uint8_t* pData)
{
    //uint32_t Capacity  = *reinterpret_cast<uint32_t*>(pData);
    uint64_t OffsetOfNextFileTable  = *reinterpret_cast<uint64_t*>(pData + 4);
    int count = 0;

    //34
    for (uint8_t* i = pData + 12; *reinterpret_cast<uint64_t*>(i) != 0; i += 34)
    {
      count++;
    }

    pEntries = new FileEntry[count];
    for (int i = 0; i < count; ++i)
    {
      pEntries[i] = FileEntry();
      pEntries[i].unmarshal(pData + i * 34 + 12);
    }
}
