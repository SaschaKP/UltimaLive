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

#include <windows.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "Atlas.h"
#include "..\UoLiveAppState.h"

Atlas::Atlas(BaseFileManager* pManager, UoLiveAppState* pAppState, NetworkManager* pNetManager)
  : m_pFileManager(pManager),
  m_mapDefinitions(),
  m_currentMap(0),
  m_pAppState(pAppState),
  m_pNetManager(pNetManager),
  m_shardIdentifier(),
  m_firstMapLoad(true),
  m_pMapThingieTable(NULL),
  m_pClientMinDisplayX(NULL),
  m_pClientMinDisplayY(NULL),
  m_pClientBlockArray(NULL),
  m_pRefreshTerrainFunctionPtr(NULL),
  m_pMasterStaticsListPtr(NULL)
{
  //do nothing
}

///////////////////////////////////////////////////////////////////////////////////
struct FindWindowData {
    FindWindowData( TCHAR const * windowTitle )
        : WindowTitle( windowTitle )
        , ResultHandle( 0 )
    {}

    std::basic_string<TCHAR> WindowTitle;
    HWND ResultHandle;
};

BOOL CALLBACK FindWindowImpl( HWND hWnd, LPARAM lParam ) {
    FindWindowData * p = reinterpret_cast<FindWindowData*>( LongToPtr( lParam ) );
    if( !p ) {
        // Finish enumerating we received an invalid parameter
        return FALSE;
    }

    int length = GetWindowTextLength( hWnd ) + 1;
    if( length > 0 ) {
        std::vector<TCHAR> buffer( std::size_t( length ), 0 );      
        if( GetWindowText( hWnd, &buffer[0], length ) ) {
                    // Comparing the string - If you want to add some features you can do it here
            if( _strnicmp( &buffer[0], p->WindowTitle.c_str(), min( buffer.size(), p->WindowTitle.size() )  ) == 0 ) {
                p->ResultHandle = hWnd;
                // Finish enumerating we found what we need
                return FALSE;
            }
        }
    }
    // Continue enumerating
    return TRUE;
}

// Returns the window handle when found if it returns 0 GetLastError() will return more information
BOOL FindWindowStart( TCHAR const * windowTitle ) {

    if( !windowTitle ) {
        return FALSE;
    }

    FindWindowData data( windowTitle );
    if( !EnumWindows( FindWindowImpl, PtrToLong(&data) ) && data.ResultHandle != 0 ) {
        #ifdef DEBUG
		SetLastError( ERROR_SUCCESS );
		#endif
        return TRUE;//data.ResultHandle;
    }

    // Return ERROR_FILE_NOT_FOUND in GetLastError
	#ifdef DEBUG
    SetLastError( ERROR_FILE_NOT_FOUND );
	#endif
    return FALSE;//0;
}

/////////////////////////////////////////////////////////////


void Atlas::onShardIdentifierUpdate(std::string shardIdentifier)
{
  m_shardIdentifier = shardIdentifier;
}

void Atlas::onLogout()
{
  m_pFileManager->onLogout();
}

void Atlas::LoadMap(uint8_t map)
{
#ifdef DEBUG
  printf("ON BEFORE LOAD MAP: %i\n", map);
#endif

  if (m_firstMapLoad)
  {
    m_firstMapLoad = false;
    m_pFileManager->InitializeShardMaps(m_shardIdentifier, m_mapDefinitions);
  }

  if (m_mapDefinitions.find((uint32_t)map) != m_mapDefinitions.end())
  {
    *reinterpret_cast<uint16_t*>(m_pAppState->m_pMapDimensions) = m_mapDefinitions[map].mapWidthInTiles;
    *reinterpret_cast<uint16_t*>(m_pAppState->m_pMapDimensions + 4) = m_mapDefinitions[map].mapHeightInTiles;
    *reinterpret_cast<uint16_t*>(m_pAppState->m_pMapDimensions + 8) = m_mapDefinitions[map].mapWrapWidthInTiles;
    *reinterpret_cast<uint16_t*>(m_pAppState->m_pMapDimensions + 12) = m_mapDefinitions[map].mapWrapHeightInTiles;
    m_pFileManager->LoadMap(map);
    m_currentMap = map;
  }
#ifdef DEBUG
  else 
  {
    printf("MAP DEFINITION NOT FOUND: %i\n", m_mapDefinitions.size());

    for (std::map<uint32_t, MapDefinition>::iterator itr = m_mapDefinitions.begin(); itr != m_mapDefinitions.end(); itr++)
    {
      printf("Map Definition: %i/%i\n", itr->first, itr->second.mapNumber);
    }
  }
#endif
}

void Atlas::init()
{
  m_pNetManager->subscribeToOnBeforeMapChange(std::bind(&Atlas::onBeforeMapChange, this, std::placeholders::_1));
  m_pNetManager->subscribeToOnMapChange(std::bind(&Atlas::onMapChange, this, std::placeholders::_1));
  m_pNetManager->subscribeToRefreshClient(std::bind(&Atlas::onRefreshClientView, this));
  m_pNetManager->subscribeToBlockQueryRequest(std::bind(&Atlas::onHashQuery, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  m_pNetManager->subscribeToStaticsUpdate(std::bind(&Atlas::onUpdateStatics, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  m_pNetManager->subscribeToMapDefinitionUpdate(std::bind(&Atlas::onUpdateMapDefinitions, this, std::placeholders::_1));
  m_pNetManager->subscribeToLandUpdate(std::bind(&Atlas::onUpdateLand, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  m_pNetManager->subscribeToUltimaLiveLoginComplete(std::bind(&Atlas::onShardIdentifierUpdate, this, std::placeholders::_1));
  m_pNetManager->subscribeToLogout(std::bind(&Atlas::onLogout, this));

  m_pMapThingieTable = MasterControlUtils::GetDrawMapThingieTable();
  m_pClientMinDisplayX = MasterControlUtils::GetMinClientDisplayX();
  m_pClientMinDisplayY = MasterControlUtils::GetMinClientDisplayY();
  m_pClientBlockArray = MasterControlUtils::GetClientDisplayedBlocksTable();

  m_pRefreshTerrainFunctionPtr = MasterControlUtils::GetRefreshTerrainFunctionPointer();

  m_pMasterStaticsListPtr = MasterControlUtils::GetMasterStaticsListPointer();

#ifdef DEBUG
  printf("m_pMapThingieTable: 0x%08x\n", m_pMapThingieTable);
  printf("m_pClientMinDisplayX: 0x%08x\n", m_pClientMinDisplayX);
  printf("m_pClientMinDisplayY: 0x%08x\n", m_pClientMinDisplayY);
  printf("m_pClientBlockArray: 0x%08x\n", m_pClientBlockArray);

  printf("RefreshTerrainFunction1: 0x%08x\n", m_pRefreshTerrainFunctionPtr);
  printf("Master Statics List: 0x%08x\n", m_pMasterStaticsListPtr);
#endif
}

#pragma pack(1)
struct CDrawItem
{
 /* 0x00 */ uint32_t vtable;
 /* 0x04 */ uint32_t BitPattern;
 /* 0x08 */ uint32_t Unknown; //field 8
 /* 0x0C */ uint32_t Unknown2; //field c
 /* 0x10 */ uint32_t Unknown3; //field 10
 /* 0x14 */ uint32_t Unknown4; //field 14
 /* 0x18 */ CDrawItem* nextDraw; //field 
 /* 0x1C */ CDrawItem* prevDraw;
 /* 0x20 */ uint32_t InDrawList;
 /* 0x24 */ uint16_t X;
 /* 0x26 */ uint16_t Y;
 /* 0x28 */ uint16_t Z;
};

#pragma pack(1)
struct StaticObject
{
 /* 0x00 */ uint32_t vtable;
 /* 0x04 */ uint32_t BitPattern;
 /* 0x08 */ uint32_t field8; //field 8
 /* 0x0C */ uint32_t fieldC; //field c
 /* 0x10 */ uint32_t field10; //field 10
 /* 0x14 */ uint32_t field14; //field 14
 /* 0x18 */ CDrawItem* nextDraw; //field 
 /* 0x1C */ CDrawItem* prevDraw;
 /* 0x20 */ uint32_t InDrawList;
 /* 0x24 */ uint16_t X;
 /* 0x26 */ uint16_t Y;
 /* 0x28 */ uint16_t Z;
 /* Start Static Members */
 /* 0x2A */ uint32_t xPlusMapWidthMinusY;
 /* 0x2E */ uint32_t xPlusY;
 /* 0x32 */ uint32_t zOrder;
 /* 0x36 */ uint32_t field36;
 /* 0x3A */ uint16_t ItemID;
 /* 0x3C */ uint16_t OriginalItemID;
 /* 0x3E */ uint8_t  StackID;
 /* 0x3F */ uint8_t Amount;
 /* 0x40    uint8_t field16  */
 /* 0x41 */ uint16_t Hue;
 /* 0x43 */ uint16_t Unknown10;
 /* 0x45 */ StaticObject* pNextStaticObject;
 /* 0x49 */ StaticObject* pPrevStaticObject;
 /* 0x4D */ StaticObject* pNextLightSource;
 /* 0x51 */ StaticObject* pPrevLightSource;
 /* 0x55 */ StaticObject* pNextAnimationStatic;
 /* 0x59 */ StaticObject* pPrevAnimationStatic;
 /* 0x5D */ StaticObject* pNextCanopyStaticObject;
 /* 0x61 */ StaticObject* pPrevCanopyStaticObject;
 /* 0x65 */ StaticObject* pNextMultiComponent;
 /* 0x69 */ StaticObject* pPrevMultiComponent;
};

void Atlas::refreshClientLand(uint8_t mapNumber, uint32_t blockNumber)
{
  if (m_mapDefinitions.find(mapNumber) != m_mapDefinitions.end())
  {
    MapDefinition def = m_mapDefinitions[mapNumber];

    for (int x = 0; x < 64; ++x)
    {
      for (int y = 0; y < 64; ++y)
      {
        CDrawItem** pDrawItem = reinterpret_cast<CDrawItem**>(m_pMapThingieTable + ( (((y & 0x3F) * 64) + (x & 0x3F)) * sizeof(uint32_t)));

        if (pDrawItem != NULL && *pDrawItem != NULL)
        {
          for (CDrawItem* pDrawItr = *pDrawItem; pDrawItr != NULL && pDrawItr->BitPattern == 0xFEEDBEEF; pDrawItr = pDrawItr->nextDraw) 
          {
            int (__thiscall* pIsTerrain)(void* This);
            pIsTerrain = reinterpret_cast<int(__thiscall *)(void* This)>(reinterpret_cast<uint32_t*>(pDrawItr->vtable)[7]);
            int result = pIsTerrain(pDrawItr);

            if (result != 0)
            {
              int (__thiscall* pRemoveFromDrawList)(void* pThis);    
              pRemoveFromDrawList = reinterpret_cast<int(__thiscall*)(void*)>(reinterpret_cast<uint32_t*>(pDrawItr->vtable)[5]);
              result = pRemoveFromDrawList(pDrawItr);
            }
          }
        }
      }
    }

    int (__cdecl* pReloadTerrain)();
    pReloadTerrain = reinterpret_cast<int(__cdecl *)()>(m_pRefreshTerrainFunctionPtr);
    int result = pReloadTerrain();
  }
}

void Atlas::onUpdateStatics(uint8_t mapNumber, uint32_t blockNumber, uint8_t* pData, uint32_t length)
{
  m_pFileManager->writeStaticsBlock(mapNumber, blockNumber, pData, length);
  refreshClientStatics(mapNumber, blockNumber);
}

void Atlas::onUpdateLand(uint8_t mapNumber, uint32_t blockNumber, uint8_t* pLandData)
{
  m_pFileManager->updateLandBlock(mapNumber, blockNumber, pLandData);
  refreshClientLand(mapNumber, blockNumber);
}

void Atlas::onHashQuery(uint32_t blockNumber, uint8_t mapNumber, uint16_t sequence)
{
#ifdef DEBUG
  printf("Atlas: Got Hash Query\n");
#endif

  uint16_t* crcs = GetGroupOfBlockCrcs(mapNumber, blockNumber);
  
  uint8_t* pResponse = new uint8_t[71];
  
  pResponse[0] = 0x3F;                                               //byte 000              -  cmd
  *reinterpret_cast<uint16_t*>(pResponse + 1) = 71;                  //byte 001 through 002  -  packet size
  *reinterpret_cast<uint32_t*>(pResponse + 3) = htonl(blockNumber);  //byte 003 through 006  -  central block number for the query (block that player is standing in)
  *reinterpret_cast<uint32_t*>(pResponse + 7) = htonl(8);            //byte 007 through 010  -  number of statics in the packet (8 for a query response)
  *reinterpret_cast<uint16_t*>(pResponse + 11) = htons(sequence);           //byte 011 through 012  -  UltimaLive sequence number
  pResponse[13] = 0xFF;                                              //byte 013              -  UltimaLive command (0xFF is a block Query Response)
  pResponse[14] = mapNumber;                                         //byte 014              -  UltimaLive mapnumber
                                                                     //byte 015 through 64   -  25 block CRCs
  for (int i = 0; i < 25; i++)
  {
    *reinterpret_cast<uint16_t*>(pResponse + 15 + (i * 2)) = htons(crcs[i]);
  }
  
  pResponse[65] = 0xFF;                                           //byte 065              -  padding
  pResponse[66] = 0xFF;                                           //byte 066              -  padding
  pResponse[67] = 0xFF;                                           //byte 067              -  padding
  pResponse[68] = 0xFF;                                           //byte 068              -  padding
  pResponse[69] = 0xFF;                                           //byte 069              -  padding
  pResponse[70] = 0xFF;                                           //byte 070              -  padding
  
  m_pNetManager->sendPacketToServer(pResponse);
  delete pResponse;
}

void Atlas::onUpdateMapDefinitions(std::vector<MapDefinition> definitions)
{
  m_mapDefinitions.clear();

  for (std::vector<MapDefinition>::iterator itr = definitions.begin(); itr != definitions.end(); itr++)
  {
    m_mapDefinitions[itr->mapNumber] = *itr;
#ifdef DEBUG
    printf("Registering Map #%i, dim=%ix%i, wrap=%ix%i\n", itr->mapNumber, itr->mapWidthInTiles, itr->mapHeightInTiles, itr->mapWrapWidthInTiles, itr->mapWrapHeightInTiles);
#endif
  }
}

void Atlas::refreshClientStatics(uint8_t mapNumber, uint32_t blockNumber)
{
  if (m_mapDefinitions.find(mapNumber) != m_mapDefinitions.end())
  {
    MapDefinition def = m_mapDefinitions[mapNumber];
    int32_t upperLeftX = ((int32_t)blockNumber /  (def.mapHeightInTiles >> 3)) * 8;
    int32_t upperLeftY = ((int32_t)blockNumber % (def.mapHeightInTiles >> 3)) * 8;

    CDrawItem* pDrawItem = reinterpret_cast<CDrawItem*>(m_pMapThingieTable + ( (((upperLeftY & 0x3F) * 64) + (upperLeftX & 0x3F)) * sizeof(uint32_t)));
    
    //This may need to be added back in later, it was causing a client crash; It's using a hard coded address, which needs a signature anyway
    //StaticObject* pPrevStatic = NULL;
    //PURGE MASTER STATIC LIST
    for (StaticObject* pStaticItem = *(StaticObject**)m_pMasterStaticsListPtr; pStaticItem != NULL && pStaticItem->BitPattern == 0xFEEDBEEF; pStaticItem = pStaticItem->pNextStaticObject)
    {
      int (__thiscall* pIsDynamic)(void* This);
      pIsDynamic = reinterpret_cast<int(__thiscall *)(void* This)>(reinterpret_cast<uint32_t*>(pStaticItem->vtable)[11]);
      int isDynamic = pIsDynamic(pStaticItem);
    
      uint32_t staticBlockNumber = ((pStaticItem->X >> 3) * (def.mapHeightInTiles >> 3)) + (pStaticItem->Y >> 3);
    
      if (staticBlockNumber == blockNumber && !isDynamic)
      {
        if (pStaticItem->InDrawList != 0)
        {
    
          int (__thiscall* pRemoveFromDrawList)(void* pThis);    
          pRemoveFromDrawList = reinterpret_cast<int(__thiscall*)(void*)>(reinterpret_cast<uint32_t*>(pStaticItem->vtable)[5]);
          int result = pRemoveFromDrawList(pStaticItem);
        }       
        }
      }

    //Clear Client Blocks Array
    for (int i = 0; i < 36; i++)
    {
      if ( reinterpret_cast<int*>(m_pClientBlockArray)[i] == blockNumber)
      {
        reinterpret_cast<int*>(m_pClientBlockArray)[i] = -1;
      }
    }
    
    //Set Client Minimum Displayed X to -9999
    *reinterpret_cast<int*>(m_pClientMinDisplayX) = -9999;
    *reinterpret_cast<int*>(m_pClientMinDisplayY) = -9999;

    ClientRedirections::UpdateStaticBlocks();
  }
}

void Atlas::onRefreshClientView()
{
#ifdef DEBUG
  printf("Atlas: Refreshing Client View\n");
#endif
  uint8_t aPacketData[11];
  aPacketData[0] = (byte)0x21; //char move reject command
  aPacketData[1] = (byte)0xFF;  //seq num
  PlayerLocation loc = m_pAppState->getPlayerLocation();
  *reinterpret_cast<uint16_t*>(&aPacketData[2]) = htons(loc.X);
  *reinterpret_cast<uint16_t*>(&aPacketData[4]) = htons(loc.Y);
  aPacketData[6] = loc.Facing;
  aPacketData[7] = loc.Z;
  aPacketData[8] = (byte)0x22;
  aPacketData[9] = (byte)0x00;
  aPacketData[10] = (byte)0x00;
  m_pNetManager->sendPacketToClient(aPacketData);
  //delete(aPacketData);
  uint8_t aPacketData2[3];
  aPacketData2[0] = 0x22;
  aPacketData2[1] = 0xFF; //sequence number
  aPacketData2[2] = 0x00;
  m_pNetManager->sendPacketToClient(aPacketData2);
  //delete(aPacketData2);
}

void Atlas::onBeforeMapChange(uint8_t& rMap)
{
  //send a packet to tell the client to change to map 1
  uint8_t packet[6];
  packet[0] = 0xBF;
  packet[1] = 0x06;
  packet[2] = 0x00;
  packet[3] = 0x00;
  packet[4] = 0x08;
  packet[5] = 0x01;
  m_pNetManager->sendPacketToClient(packet);
  //delete(packet);
}


void Atlas::onMapChange(uint8_t& rMap)
{
  //modify map change packet to point to map 0
  LoadMap(rMap);
  rMap = 0x00;
}

uint8_t Atlas::getCurrentMap()
{
  return m_currentMap;
}

uint16_t Atlas::fletcher16(uint8_t* pBlockData, uint8_t* pStaticsData, uint32_t staticsLength)
{
  uint16_t sum1 = 0;
  uint16_t sum2 = 0;
  
  if (pBlockData != NULL)
  {
    for (int index = 0; index < 192; ++index)
    {
      sum1 = (uint16_t)((sum1 + pBlockData[index]) % 255);
      sum2 = (uint16_t)((sum2 + sum1) % 255);
    }
  }

  if (pStaticsData != NULL)
  {
    for (uint32_t index = 0; index < staticsLength; ++index)
    {
      sum1 = (uint16_t)((sum1 + pStaticsData[index]) % 255);
      sum2 = (uint16_t)((sum2 + sum1) % 255);
    }
  }

  return (uint16_t)((sum2 << 8) | sum1);
}

int32_t Atlas::BLOCK_POSITION_OFFSETS[5] = { -2, -1, 0, 1, 2 };

uint16_t* Atlas::GetGroupOfBlockCrcs(uint32_t mapNumber, uint32_t blockNumber)
{
  uint16_t* pCrcs = NULL;
  if (m_mapDefinitions.find(mapNumber) != m_mapDefinitions.end())
  {
    pCrcs = new uint16_t[25];
    MapDefinition def = m_mapDefinitions[mapNumber];
    int32_t blockX = ((int32_t)blockNumber /  (def.mapHeightInTiles >> 3));
    int32_t blockY = ((int32_t)blockNumber % (def.mapHeightInTiles >> 3));

    int32_t wrapWidthInBlocks = (def.mapWrapWidthInTiles >> 3);
    int32_t wrapHeightInBlocks = (def.mapWrapHeightInTiles >> 3);
    int32_t mapWidthInBlocks = (def.mapWidthInTiles >> 3);
    int32_t mapHeightInBlocks = (def.mapHeightInTiles >> 3);

    if (blockX < 0 || blockX >= mapWidthInBlocks|| blockY < 0 || blockY >= mapHeightInBlocks)
    {
      memset(pCrcs, 0x00, sizeof(uint16_t) * 25);
    }
    else
    {
      for (int x = -2; x <= 2; x++)
      {
        int xBlockItr = -1;
        if (blockX < wrapWidthInBlocks)
        {
          xBlockItr = (blockX + x) % wrapWidthInBlocks;
          if (xBlockItr < 0 && xBlockItr > -3)
          {
            xBlockItr += wrapWidthInBlocks;
          }
        }
        else
        {
          xBlockItr = (blockX + x) % mapWidthInBlocks;
          if (xBlockItr < 0 && xBlockItr > -3)
          {
            xBlockItr += mapWidthInBlocks;
          }
        }
        for (int y = -2; y <= 2; y++)
        {
          int yBlockItr = 0;
          if (blockY < wrapHeightInBlocks)
          {
            yBlockItr = (blockY + y) % wrapHeightInBlocks;
            if (yBlockItr < 0)
            {
              yBlockItr += wrapHeightInBlocks;
            }
          }
          else
          {
            yBlockItr = (blockY + y) % mapHeightInBlocks;
            if (yBlockItr < 0)
            {
              yBlockItr += mapHeightInBlocks;
            }
          }
    
          int32_t currentBlock = (xBlockItr * mapHeightInBlocks) + yBlockItr;
    
          if (currentBlock >= 0 && currentBlock <= (mapHeightInBlocks * mapWidthInBlocks))
          {
            pCrcs[((x + 2) * 5) + (y + 2)] = getBlockCrc (mapNumber, currentBlock);
          }
          else
          {
            pCrcs[((x + 2) * 5) + (y + 2)] = (uint16_t)0x0;
          }
        }
      }
    }
  }

  return pCrcs;
}

uint16_t Atlas::getBlockCrc(uint32_t mapNumber, uint32_t blockNumber)
{
  uint16_t crc = 0; 
  if (m_mapDefinitions.find(mapNumber) != m_mapDefinitions.end())
  {
    uint8_t* pBlockData = m_pFileManager->readLandBlock(mapNumber, blockNumber);
    uint32_t staticsLength = 0;

    uint8_t* pStaticsData = m_pFileManager->readStaticsBlock(mapNumber, blockNumber, staticsLength);

    if (pBlockData != NULL)
    {
      crc = fletcher16(pBlockData, pStaticsData, staticsLength);
      delete pBlockData;
    }

    if (pStaticsData != NULL)
    {
      delete pStaticsData;
    }
  }

  return crc;
}
