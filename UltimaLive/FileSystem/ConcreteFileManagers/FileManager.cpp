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

#include "FileManager.h"
#include <cstdio>

#include "..\Uop\UopUtility.h"
#include "..\..\Maps\MapDefinition.h"

FileManager::FileManager()
  : BaseFileManager(),
    m_neededFiles()
{
  m_neededFiles["map0.mul"] = 0;
  m_neededFiles["staidx0.mul"] = 0;
  m_neededFiles["statics0.mul"] = 0;

  m_neededFiles["map1.mul"] = 0;
  m_neededFiles["staidx1.mul"] = 0;
  m_neededFiles["statics1.mul"] = 0;
  
  m_neededFiles["map2.mul"] = 0;
  m_neededFiles["staidx2.mul"] = 0;
  m_neededFiles["statics2.mul"] = 0;
  
  /*m_neededFiles["map3.mul"] = 0;
  m_neededFiles["staidx3.mul"] = 0;
  m_neededFiles["statics3.mul"] = 0;
  
  m_neededFiles["map4.mul"] = 0;
  m_neededFiles["staidx4.mul"] = 0;
  m_neededFiles["statics4.mul"] = 0;
  
  m_neededFiles["map5.mul"] = 0;
  m_neededFiles["staidx5.mul"] = 0;
  m_neededFiles["statics5.mul"] = 0;*/
}

void FileManager::LoadMap(uint8_t mapNumber)
{
  if (m_pMapFileStream->is_open())
  {
    m_pMapFileStream->close();
  }
  if (m_pStaidxFileStream->is_open())
  {
    m_pStaidxFileStream->close();
  }
  if (m_pStaticsFileStream->is_open())
  {
    m_pStaticsFileStream->close();
  }

  uint32_t currentByteIndexOfFile = 0;

  std::string filenameAndPath = BaseFileManager::getUltimaLiveSavePath();
  if (m_shardIdentifier != "")
  {
    filenameAndPath.append(m_shardIdentifier);
    filenameAndPath.append("\\");
  }

  std::string mapFileNameAndPath(filenameAndPath);
  char filename[32];
  sprintf_s(filename, "map%i.mul", mapNumber);
  mapFileNameAndPath.append(filename);

  std::string staidxFileNameAndPath(filenameAndPath);
  sprintf_s(filename, "staidx%i.mul", mapNumber);
  staidxFileNameAndPath.append(filename);

  std::string staticsFileNameAndPath(filenameAndPath);
  sprintf_s(filename, "statics%i.mul", mapNumber);
  staticsFileNameAndPath.append(filename);

#ifdef DEBUG
  printf("Loading Map: %s\n", mapFileNameAndPath.c_str());
#endif

  std::ifstream mapFile;
  mapFile.open(mapFileNameAndPath, std::ios::binary | std::ios::in);
  if (mapFile.is_open())
  {
    mapFile.seekg (0, mapFile.end);
    std::streamoff length = mapFile.tellg();
    if (length > 0)
    {
      mapFile.seekg (0, mapFile.beg);
      mapFile.read(reinterpret_cast<char*>(m_pMapPool), length);
    }

    mapFile.close();
  }

#ifdef DEBUG
  printf("Loading Staidx: %s\n", staidxFileNameAndPath.c_str());
#endif

  std::ifstream staidxFile;
  staidxFile.open(staidxFileNameAndPath, std::ios::binary | std::ios::in);

  if (staidxFile.is_open())
  {
    staidxFile.seekg (0, staidxFile.end);
    std::streamoff length = staidxFile.tellg();
    if (length > 0)
    {
      staidxFile.seekg (0, staidxFile.beg);
      staidxFile.read(reinterpret_cast<char*>(m_pStaidxPool), length);
    }
    staidxFile.close();
  }

#ifdef DEBUG
  printf("Loading Statics: %s\n", staticsFileNameAndPath.c_str());
#endif

  std::ifstream staticsFile;
  staticsFile.open(staticsFileNameAndPath, std::ios::binary | std::ios::in);
  if (staticsFile.is_open())
  {
    staticsFile.seekg (0, staticsFile.end);
    std::streamoff length = staticsFile.tellg();
    if (length > 0)
    {
      staticsFile.seekg (0, staticsFile.beg);
      staticsFile.read(reinterpret_cast<char*>(m_pStaticsPool), length);
    }
    m_pStaticsPoolEnd = m_pStaticsPool + length;
    staticsFile.close();
  }

  m_pMapFileStream->open(mapFileNameAndPath, std::ios::out | std::ios::in | std::ios::binary);
  m_pStaidxFileStream->open(staidxFileNameAndPath, std::ios::out | std::ios::in | std::ios::binary);
  m_pStaticsFileStream->open(staticsFileNameAndPath, std::ios::out | std::ios::in | std::ios::binary);

#ifdef DEBUG
  printf("Finished Loading Map!\n");
#endif
}

void FileManager::Initialize()
{
  BaseFileManager::Initialize();
  m_pMapPool = reinterpret_cast<uint8_t*>(VirtualAlloc(NULL, MAP_MEMORY_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
  m_pStaticsPool = reinterpret_cast<uint8_t*>(VirtualAlloc(NULL, STATICS_MEMORY_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE));
  m_pStaticsPoolEnd = m_pStaticsPool;
  m_pStaidxPool = reinterpret_cast<uint8_t*>(VirtualAlloc(NULL, STAIDX_MEMORY_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE));

#ifdef DEBUG
  printf("Map 0x%x\n", m_pMapPool);
  printf("Statics 0x%x\n", m_pStaticsPool);
  printf("Staidx 0x%x\n", m_pStaidxPool);
#endif
}

LPVOID WINAPI FileManager::OnMapViewOfFile(
  __in  HANDLE hFileMappingObject,
  __in  DWORD dwDesiredAccess,
  __in  DWORD dwFileOffsetHigh,
  __in  DWORD dwFileOffsetLow,
  __in  SIZE_T dwNumberOfBytesToMap
  )
{
  ClientFileHandleSet* pMatchingFileset = NULL;

  for (std::map<std::string, ClientFileHandleSet*>::const_iterator iterator = m_files.begin(); iterator != m_files.end(); iterator++)
  {
    if (iterator->second->m_createFileMappingHandle == hFileMappingObject)
    {
      pMatchingFileset = iterator->second;
      break;
    }
  }

  HANDLE handleToReturn = INVALID_HANDLE_VALUE;

  if (pMatchingFileset != NULL)
  {
    std::string shortFilename = Utils::getFilenameFromPath(pMatchingFileset->m_filename);
    std::map<std::string, uint32_t>::iterator iterator = m_neededFiles.find(shortFilename);

    if (iterator != m_neededFiles.end())
    {
      if (shortFilename.find("map") != std::string::npos)
      {
          handleToReturn = m_pMapPool;
      }
      else if (shortFilename.find("statics") != std::string::npos)
      {
        handleToReturn = m_pStaticsPool;
      }
      else if (shortFilename.find("staidx") != std::string::npos)
      {
        handleToReturn = m_pStaidxPool;
      }
    }
  }

  if (handleToReturn == INVALID_HANDLE_VALUE)
  {
    handleToReturn = MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
    if (pMatchingFileset != NULL)
    {
      pMatchingFileset->m_mapFileViewHandle = handleToReturn;
    }
  }

	return handleToReturn;
}

unsigned char* FileManager::seekLandBlock(uint8_t mapNumber, uint32_t blockNum)
{
  uint8_t* pData = reinterpret_cast<unsigned char*>(m_pMapPool + (blockNum * 196) + 4);
  return pData;
}

unsigned char* FileManager::readLandBlock(uint8_t mapNumber, uint32_t blockNum)
{
  unsigned char* pBlockPosition = seekLandBlock(mapNumber, blockNum);

  unsigned char* pData = new uint8_t[192];

  if (pBlockPosition != NULL)
  {
    for (int i = 0; i < 192; ++i)
    {
      pData[i] = reinterpret_cast<unsigned char*>(pBlockPosition)[i];
    }
  }

  return pData;
}
 
bool FileManager::updateLandBlock(uint8_t mapNumber, uint32_t blockNum, uint8_t* pLandData)
{
  //update block in memory
  unsigned char* pBlockPosition = seekLandBlock(mapNumber, blockNum);

  #ifdef DEBUG
    printf("Land Block Memory Location: 0x%x\n", (int)pBlockPosition);
  #endif


  if (pBlockPosition != NULL)
  {
    for (int i = 0; i < 192; ++i)
    {
      pBlockPosition[i] = pLandData[i];
    }
  }
  #ifdef DEBUG
  else
  {
    printf("Unable to update land block!\n");
  }
  #endif

  if (m_pMapFileStream->is_open())
  {
    //update block on disk
    uint32_t blockSeekLocation = (blockNum * 196) + 4;
#ifdef DEBUG
    printf("Updating Land block on disk at location: 0x%x\n", blockSeekLocation);
#endif
    m_pMapFileStream->seekp(blockSeekLocation, std::ios::beg);
  
#ifdef DEBUG
    if (m_pMapFileStream->bad())
    {
      printf("Failed to Seek!!!!!!!!!!!!!! (%i)\n", GetLastError());
    }
    else
    {
      printf("Seek successful\n");
    }
#endif
  
    m_pMapFileStream->write(const_cast<const char*>(reinterpret_cast<char*>(pLandData)), 192);
#ifdef DEBUG  
    if (m_pMapFileStream->bad())
    {
      printf("Failed to Write!!!!!!!!!!!!!! (%i)\n", GetLastError());
    }
    else
    {
      printf("Wrote successfully\n");
    }
#endif
  }

  m_pMapFileStream->flush();

#ifdef DEBUG  
      printf("Flushed successfully\n");
#endif

  return true;
}