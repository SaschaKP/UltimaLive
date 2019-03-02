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


#ifndef _FILE_MANAGER_7_0_29_2_H
#define _FILE_MANAGER_7_0_29_2_H

#include "..\BaseFileManager.h"
#include "..\Uop\UopStructs.h"
#include "..\Uop\UopUtility.h"
#include "..\..\Utils.h"
#include "..\..\LocalPeHelper32.hpp"

#include <Windows.h>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <stdint.h>

class FileManager_7_0_29_2 : public BaseFileManager
{
  public:
    FileManager_7_0_29_2();

    LPVOID WINAPI OnMapViewOfFile(
      __in  HANDLE hFileMappingObject,
      __in  DWORD dwDesiredAccess,
      __in  DWORD dwFileOffsetHigh,
      __in  DWORD dwFileOffsetLow,
      __in  SIZE_T dwNumberOfBytesToMap
    );

    bool updateLandBlock(uint8_t mapNumber, uint32_t blockNum, uint8_t* pData);
    unsigned char* readLandBlock(uint8_t mapNumber, uint32_t blockNum);
    
    void InitializeShardMaps(std::string shardIdentifier, std::map<uint32_t, MapDefinition> definitions);

    void Initialize();
    void LoadMap(uint8_t mapNumber);

    static const int MAP_MEMORY_SIZE = 100000000;
    static const int STAIDX_MEMORY_SIZE = 10000000;

  protected:
    std::map<uint32_t, FileEntry*> m_fileEntries; 
    std::map<std::string, uint32_t> m_neededFiles;
    unsigned char* seekLandBlock(uint8_t mapNumber, uint32_t blockNum);

    void parseMapFile(std::string filename);
};
#endif