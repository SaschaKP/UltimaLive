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


#ifndef _UOP_STRUCTS_H
#define _UOP_STRUCTS_H

#include <stdint.h>
#include <iostream>

class UopHeader
{
  public:
    void unmarshal(uint8_t* pData);
    uint32_t FileIdentifier;  //myp\0
    uint32_t Version; //0x00000005
    uint32_t Signature; 
    uint64_t FileTableOffset; 
    uint32_t FileTableCapacity;
    uint32_t TotalFiles;
    uint32_t NumFileTables;
};

class FileEntry
{
  public:
    void unmarshal(uint8_t* pData);
    uint64_t UopFileOffset;
    uint32_t MetaDataSize;
    uint32_t CompressedDataSize;
    uint32_t UncompressedDataSize;
    uint64_t PathChecksum;
    uint32_t MetadataCrc;
    uint16_t CompressionMethod;
};

class FileTable
{
  public:
    void unmarshal(uint8_t* pData);
    uint32_t Capacity;
    uint64_t OffsetOfNextFileTable;
    FileEntry* pEntries;
};

class UopMetaData
{
  public:
    uint16_t unknown;
    uint16_t metaDataSize;
    uint16_t signatureId;
    uint16_t signatureLength;
    uint8_t* pSignature;
};

class UopFile
{
  public:
    UopMetaData MetaData;
    uint8_t* pData;
};
#endif
