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

#ifndef _ATLAS_H
#define _ATLAS_H

#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include <stdint.h>
#include <map>
#include <algorithm>
#include <codecvt>
#include "..\FileSystem\BaseFileManager.h"
#include "MapDefinition.h"
#include "..\LocalPeHelper32.hpp"

class UoLiveAppState;
class NetworkManager;
class LoginHandler;

class Atlas
{
  public:
    Atlas(BaseFileManager* pManager, UoLiveAppState* pAppState, NetworkManager* pNetManager);
    uint16_t* GetGroupOfBlockCrcs(uint32_t mapNumber, uint32_t blockNumber);
    void RegisterMapDefinitions(MapDefinition* aDefinitions, uint32_t numDefinitions);

    void init();

    static uint16_t fletcher16(uint8_t* pBlockData, uint8_t* pStaticsData, uint32_t staticsLength);

    void LoadMap(uint8_t map);
    uint8_t getCurrentMap();

  protected:
    void onBeforeMapChange(uint8_t& rMap);
    void onMapChange(uint8_t& rMap);

    void onHashQuery(uint32_t blockNumber, uint8_t mapNumber, uint16_t sequence);
    void onRefreshClientView();
    void onUpdateMapDefinitions(std::vector<MapDefinition> definitions);
    void onUpdateStatics(uint8_t mapNumber, uint32_t blockNumber, uint8_t* pData, uint32_t length);
    void onShardIdentifierUpdate(std::string shardIdentifier);
    void refreshClientLand(uint8_t mapNumber, uint32_t blockNumber);
    void refreshClientStatics(uint8_t mapNumber, uint32_t blockNumber);

    void onUpdateLand(uint8_t mapNumber, uint32_t blockNumber, uint8_t* pLandData);

    void onLogout();

    static int32_t BLOCK_POSITION_OFFSETS[5];

    uint16_t getBlockCrc(uint32_t mapNumber, uint32_t blockNumber);

    BaseFileManager* m_pFileManager;
    std::map<uint32_t, MapDefinition> m_mapDefinitions;
    uint8_t m_currentMap;
    UoLiveAppState* m_pAppState;
    NetworkManager* m_pNetManager;
    std::string m_shardIdentifier;
    bool m_firstMapLoad;

    unsigned char* m_pMapThingieTable;
    unsigned char* m_pClientMinDisplayX;
    unsigned char* m_pClientMinDisplayY;
    unsigned char* m_pClientBlockArray;
    unsigned char* m_pRefreshTerrainFunctionPtr;
    unsigned char* m_pMasterStaticsListPtr;
};

#endif